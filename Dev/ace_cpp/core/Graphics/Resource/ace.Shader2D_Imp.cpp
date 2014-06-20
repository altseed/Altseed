	
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Shader2D_Imp.h"
#include "ace.NativeShader_Imp.h"
#include "../ace.Graphics_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

static const char* shader2d_dx_vs = R"(
struct VS_Input
{
	float3 Pos		: Pos0;
	float2 UV		: UV0;
	float4 Color	: COLOR0;
};

struct VS_Output
{
	float4 Pos_		: SV_POSITION;
	float4 Pos		: Pos0;
	float2 UV		: TEXCOORD0;
	float4 Color	: COLOR0;
};

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;
	Output.Pos_ = float4( Input.Pos.x, Input.Pos.y, Input.Pos.z, 1.0 );
	Output.Pos = float4( Input.Pos.x, Input.Pos.y, Input.Pos.z, 1.0 );
	Output.UV = Input.UV;
	Output.Color = Input.Color;
	return Output;
}

)";

static const char* shader2d_gl_vs = R"(
#version 330

in vec3 Pos;
in vec2 UV;
in vec4 Color;

out vec4 inPos;
out vec2 inUV;
out vec4 inColor;

void main()
{
	gl_Position = vec4(Pos.x,Pos.y,Pos.z,1.0);
	inPos = gl_Position;
	inUV = UV;
	inColor = Color;

	// gl only
	inUV.y = 1.0 - inUV.y;
}

)";

static const char* shader2d_dx_ps_pre = R"(

struct PS_Input
{
	float4 Pos_		: SV_POSITION;
	float4 Pos		: Pos0;
	float2 UV		: TEXCOORD0;
	float4 Color	: COLOR0;
};


)";

/*
float4 main( const PS_Input Input ) : SV_Target
{
float4 Output = g_texture.Sample(g_sampler, Input.UV);
if(Output.a == 0.0f) discard;
return Output;
}
*/

static const char* shader2d_gl_ps_pre = R"(
#version 330
in vec4 inPos;
in vec2 inUV;
in vec4 inColor;

out vec4 outOutput;

)";

/*
void main()
{
gl_FragColor = texture2D(g_texture, inUV.xy);
}
*/

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Shader2D_Imp::Shader2D_Imp(
		Graphics* graphics,
		std::shared_ptr<NativeShader_Imp> shader,
		std::vector<ShaderVariableProperty> vertexVariableProperties,
		std::vector<ShaderVariableProperty> pixelVariableProperties)
	{
		auto calcSize = [](decltype(vertexVariableProperties)& prop)->int32_t
		{
			int32_t constantSize = 0;
			for (auto p : prop)
			{
				if (p.Type == SHADER_VARIABLE_TYPE_FLOAT)
				{
					auto s = p.Offset + sizeof(float) * 4;
					constantSize = Max(constantSize, s);
				}
				else if (p.Type == SHADER_VARIABLE_TYPE_VECTOR2DF)
				{
					auto s = p.Offset + sizeof(float) * 4;
					constantSize = Max(constantSize, s);
				}
				else if (p.Type == SHADER_VARIABLE_TYPE_VECTOR3DF)
				{
					auto s = p.Offset + sizeof(float) * 4;
					constantSize = Max(constantSize, s);
				}
			}

			return constantSize;
		};

		int32_t vConstantSize = calcSize(vertexVariableProperties);
		int32_t pConstantSize = calcSize(pixelVariableProperties);

		auto genInfo = [](decltype(vertexVariableProperties)& prop)->std::vector<ConstantBufferInformation>
		{
			std::vector<ConstantBufferInformation> info;
			for (auto p : prop)
			{
				if (p.Type == SHADER_VARIABLE_TYPE_FLOAT)
				{
					ConstantBufferInformation i;
					i.Name = ToUtf8String(p.Name.c_str());
					i.Offset = p.Offset;
					i.Format = CONSTANT_BUFFER_FORMAT_FLOAT1;
					info.push_back(i);
				}
				else if (p.Type == SHADER_VARIABLE_TYPE_VECTOR2DF)
				{
					ConstantBufferInformation i;
					i.Name = ToUtf8String(p.Name.c_str());
					i.Offset = p.Offset;
					i.Format = CONSTANT_BUFFER_FORMAT_FLOAT2;
					info.push_back(i);
				}
				else if (p.Type == SHADER_VARIABLE_TYPE_VECTOR3DF)
				{
					ConstantBufferInformation i;
					i.Name = ToUtf8String(p.Name.c_str());
					i.Offset = p.Offset;
					i.Format = CONSTANT_BUFFER_FORMAT_FLOAT3;
					info.push_back(i);
				}
				else if (p.Type == SHADER_VARIABLE_TYPE_TEXTURE2D)
				{
					ConstantBufferInformation i;
					i.Name = ToUtf8String(p.Name.c_str());
					i.Offset = p.Offset;
					i.Format = CONSTANT_BUFFER_FORMAT_UNKNOWN;
					info.push_back(i);
				}
			}
			return info;
		};
	
		auto vInfo = genInfo(vertexVariableProperties);
		auto pInfo = genInfo(pixelVariableProperties);

		//shader->CreateVertexConstantBuffer(vConstantSize, vInfo);
		//shader->CreatePixelConstantBuffer(pConstantSize, pInfo);

		m_shader = shader;
		m_vertexVariableProperties = vertexVariableProperties;
		m_pixelVariableProperties = pixelVariableProperties;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Shader2D_Imp::~Shader2D_Imp()
	{

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Shader2D_Imp* Shader2D_Imp::Create(
		Graphics* graphics,
		const achar* shaderText,
		const achar* shaderFileName,
		Log* log)
	{
		std::vector <ShaderVariableProperty> vertexVariableProperties;
		std::vector <ShaderVariableProperty> pixelVariableProperties;

		auto g = (Graphics_Imp*) graphics;

		std::vector<ace::VertexLayout> vl;
		vl.push_back(ace::VertexLayout("Pos", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));

		std::vector<ace::Macro> macro;

		std::shared_ptr<NativeShader_Imp> shader;

		if (g->GetGraphicsType() == GraphicsType::DirectX11)
		{
			shader = g->CreateShader_Imp(
				shader2d_dx_vs,
				"shader2d_dx_vs",
				(std::string(shader2d_dx_ps_pre) + ToUtf8String(shaderText)).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro);
		}
		else if (g->GetGraphicsType() == GraphicsType::OpenGL)
		{
			shader = g->CreateShader_Imp(
				shader2d_gl_vs,
				"shader2d_gl_vs",
				(std::string(shader2d_gl_ps_pre) + ToUtf8String(shaderText)).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro);
		}
		else
		{
			assert(0);
		}

		if (shader == nullptr) return nullptr;

		return new Shader2D_Imp(
			g, 
			shader, 
			vertexVariableProperties,
			pixelVariableProperties);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

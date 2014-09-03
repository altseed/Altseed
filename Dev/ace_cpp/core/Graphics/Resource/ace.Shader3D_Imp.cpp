
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Shader3D_Imp.h"
#include "ace.NativeShader_Imp.h"
#include "../ace.Graphics_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	static const char* shader3d_dx_vs = R"(
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

	static const char* shader3d_gl_vs = R"(

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

	static const char* shader3d_dx_ps_pre = R"(

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

	static const char* shader3d_gl_ps_pre = R"(

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
	Shader3D_Imp::Shader3D_Imp(
		Graphics* graphics,
		std::shared_ptr<NativeShader_Imp> shader)
	{
		m_shader = shader;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Shader3D_Imp::~Shader3D_Imp()
	{

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Shader3D_Imp* Shader3D_Imp::Create(
		Graphics* graphics,
		const achar* shaderText,
		const achar* shaderFileName,
		Log* log)
	{
		auto g = (Graphics_Imp*) graphics;

		std::vector<ace::VertexLayout> vl;
		vl.push_back(ace::VertexLayout("Pos", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));

		std::vector<ace::Macro> macro;

		std::shared_ptr<NativeShader_Imp> shader;

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
			shader = g->CreateShader_Imp(
				shader3d_dx_vs,
				"shader2d_dx_vs",
				(std::string(shader3d_dx_ps_pre) + ToUtf8String(shaderText)).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro);
		}
		else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			shader = g->CreateShader_Imp(
				shader3d_gl_vs,
				"shader3d_gl_vs",
				(std::string(shader3d_gl_ps_pre) + ToUtf8String(shaderText)).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro);
		}
		else
		{
			assert(0);
		}

		if (shader == nullptr) return nullptr;

		return new Shader3D_Imp(
			g,
			shader);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

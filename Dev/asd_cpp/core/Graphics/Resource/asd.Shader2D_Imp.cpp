	
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Shader2D_Imp.h"
#include "asd.NativeShader_Imp.h"
#include "../asd.Graphics_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {

static const char* shader2d_dx_vs = R"(
struct VS_Input
{
	float3 Pos		: Pos0;
	float2 UV		: UV0;
	float4 Color	: COLOR0;
};

struct VS_Output
{
	float4 SV_Position	: SV_POSITION;
	float4 Position		: POSITION;
	float2 UV			: UV;
	float4 Color		: COLOR;
};

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;
	Output.SV_Position = float4( Input.Pos.x, Input.Pos.y, Input.Pos.z, 1.0 );
	Output.Position = float4( Input.Pos.x, Input.Pos.y, Input.Pos.z, 1.0 );
	Output.UV = Input.UV;
	Output.Color = Input.Color;
	return Output;
}

)";

static const char* shader2d_gl_vs = R"(

in vec3 Pos;
in vec2 UV;
in vec4 Color;

out vec4 inPosition;
out vec2 inUV;
out vec4 inColor;

void main()
{
	gl_Position = vec4(Pos.x,Pos.y,Pos.z,1.0);
	inPosition = gl_Position;
	inUV = UV;
	inColor = Color;

	// gl only
	inUV.y = 1.0 - inUV.y;
}

)";

/*
static const char* shader2d_dx_ps_pre = R"(

struct PS_Input
{
float4 SV_Position	: SV_POSITION;
float4 Position		: POSITION;
float2 UV			: UV;
float4 Color		: COLOR;
};

)";
*/

/*
float4 main( const PS_Input Input ) : SV_Target
{
float4 Output = g_texture.Sample(g_sampler, Input.UV);
if(Output.a == 0.0f) discard;
return Output;
}
*/

/*
static const char* shader2d_gl_ps_pre = R"(

in vec4 inPosition;
in vec2 inUV;
in vec4 inColor;

out vec4 outOutput;

)";
*/

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
		std::shared_ptr<NativeShader_Imp> shader)
	{
		m_shader = shader;
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
		auto g = (Graphics_Imp*) graphics;

		std::vector<asd::VertexLayout> vl;
		vl.push_back(asd::VertexLayout("Pos", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("UV", asd::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(asd::VertexLayout("Color", asd::VertexLayoutFormat::R8G8B8A8_UNORM));

		std::vector<asd::Macro> macro;

		std::shared_ptr<NativeShader_Imp> shader;

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
			shader = g->CreateShader_Imp(
				shader2d_dx_vs,
				"shader2d_dx_vs",
				ToUtf8String(shaderText).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				true,
				macro);
		}
		else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			shader = g->CreateShader_Imp(
				shader2d_gl_vs,
				"shader2d_gl_vs",
				ToUtf8String(shaderText).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				true,
				macro);
		}
		else
		{
			assert(0);
		}

		if (shader == nullptr) return nullptr;

		return new Shader2D_Imp(
			g, 
			shader);
	}

	Shader2D_Imp* Shader2D_Imp::Create(Graphics* graphics, const achar* path, Log* log)
	{
		auto g = (Graphics_Imp*)graphics;

		std::vector<asd::VertexLayout> vl;
		vl.push_back(asd::VertexLayout("Pos", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("UV", asd::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(asd::VertexLayout("Color", asd::VertexLayoutFormat::R8G8B8A8_UNORM));

		std::vector<asd::Macro> macro;

		std::shared_ptr<NativeShader_Imp> shader;

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
			return false;
		}
		else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			return false;
		}
		else
		{
			assert(0);
		}

		if (shader == nullptr) return nullptr;

		return new Shader2D_Imp(g, shader);
	}

}

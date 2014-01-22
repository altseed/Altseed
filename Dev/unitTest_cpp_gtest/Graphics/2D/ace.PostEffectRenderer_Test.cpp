
#include "../ace.Graphics_Test_Utls.h"

#include <Graphics/Common/2D/ace.PostEffectRenderer.h>
#include <Graphics/Common/2D/ace.Renderer2D_Imp.h>
#include <Graphics/Common/Resource/ace.Material2D_Imp.h>
#include <Graphics/Common/Resource/ace.Shader2D.h>

static const char* shader2d_dx_ps = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float4 main( const PS_Input Input ) : SV_Target
{
	float4 output = g_texture.Sample(g_sampler, Input.UV) * float4(1.0,(Input.Pos.x+1.0)/2.0,(Input.Pos.y+1.0)/2.0,1.0);
	if(output.a == 0.0f) discard;
	return output;
}

)";

static const char* shader2d_gl_ps = R"(

uniform sampler2D g_texture;

void main()
{
	gl_FragColor = texture2D(g_texture, inUV.xy) * vec4(1.0,(inPos.x+1.0)/2.0,(inPos.y+1.0)/2.0,1.0);
}

)";

struct Vertex
{
	ace::Vector3DF Pos;
	ace::Vector2DF UV;

	Vertex() {}
	Vertex(ace::Vector3DF pos, ace::Vector2DF uv)
	{
		Pos = pos;
		UV = uv;
	}
};

void Graphics_PostEffectRenderer(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	ace::Log* log = ace::Log_Imp::Create(L"graphics.html", L"ポストエフェクト");

	auto window = ace::Window_Imp::Create(640, 480, ace::ToAString(L"PostEffect").c_str());
	ASSERT_TRUE(window != nullptr);

	auto graphics = ace::Graphics_Imp::Create(window, isOpenGLMode, log, false);
	ASSERT_TRUE(graphics != nullptr);

	auto rtex = graphics->CreateRenderTexture(640, 480, ace::TEXTURE_FORMAT_RGBA8888);
	ASSERT_TRUE(rtex != nullptr);

	auto renderer = ace::PostEffectRenderer::Create(graphics);
	ASSERT_TRUE(renderer != nullptr);

	auto renderer2d = new ace::Renderer2D_Imp(graphics, log, window->GetSize());
	ASSERT_TRUE(renderer2d != nullptr);

	auto texture = graphics->CreateTexture2D(ace::ToAString(L"Data/Texture/Sample1.png").c_str());
	ASSERT_TRUE(texture != nullptr);

	std::shared_ptr<ace::Shader2D> shader;

	std::vector<ace::ShaderVariableProperty> props;
	auto prop_tex = ace::ShaderVariableProperty();
	prop_tex.Name = ace::ToAString("g_texture").c_str();
	prop_tex.Offset = 0;
	prop_tex.Type = ace::SHADER_VARIABLE_TYPE_TEXTURE2D;
	props.push_back(prop_tex);

	if (graphics->GetGraphicsType() == ace::GRAPHICS_TYPE_DX11)
	{
		shader = graphics->CreateShader2D(
			ace::ToAString(shader2d_dx_ps).c_str(),
			props
			);
	}
	else if (graphics->GetGraphicsType() == ace::GRAPHICS_TYPE_GL)
	{
		std::vector<ace::ShaderVariableProperty> prop;
		shader = graphics->CreateShader2D(
			ace::ToAString(shader2d_gl_ps).c_str(),
			props
			);
	}
	else
	{
		assert(0);
	}

	ASSERT_TRUE(shader != nullptr);

	auto material = graphics->CreateMaterial2D(shader);

	int32_t time = 0;
	while (window->DoEvent())
	{
		graphics->Begin();
		graphics->Clear(true, false, ace::Color(0, 0, 0, 255));

		material->SetTexture2D(ace::ToAString("g_texture").c_str(), texture);

		auto command = ((ace::Material2D_Imp*)material.get())->GenerateShaderCommand();
		command->SetTarget(rtex.get());
		renderer->DrawOnTexture2DWithMaterialWithCommand(command);

		graphics->SetRenderTarget(nullptr, nullptr);

		ace::Vector2DF positions[4];
		ace::Color colors[4];
		ace::Vector2DF uvs[4];

		colors[0] = ace::Color(255, 255, 255, 255);
		colors[1] = ace::Color(255, 255, 255, 255);
		colors[2] = ace::Color(255, 255, 255, 255);
		colors[3] = ace::Color(255, 255, 255, 255);

		positions[0].X = 0;
		positions[0].Y = 0;
		positions[1].X = 640;
		positions[1].Y = 0;
		positions[2].X = 640;
		positions[2].Y = 480;
		positions[3].X = 0;
		positions[3].Y = 480;

		uvs[0].X = 0;
		uvs[0].Y = 0;
		uvs[1].X = 1;
		uvs[1].Y = 0;
		uvs[2].X = 1;
		uvs[2].Y = 1;
		uvs[3].X = 0;
		uvs[3].Y = 1;

		renderer2d->AddSprite(positions, colors, uvs, rtex.get(), ace::eAlphaBlend::ALPHA_BLEND_BLEND, 0);

		renderer2d->DrawCache();
		renderer2d->ClearCache();

		graphics->Present();

		graphics->End();

		if (time == 10)
		{
			SAVE_SCREEN_SHOT(graphics, 0);
		}

		if (time == 11)
		{
			window->Close();
		}
		time++;
	}

	delete renderer2d;
	renderer->Release();
	graphics->Release();
	
	window->Release();
	delete log;
}

TEST(Graphics, PostEffectRenderer_DX)
{
	Graphics_PostEffectRenderer(false);
}

TEST(Graphics, PostEffectRenderer_GL)
{
	Graphics_PostEffectRenderer(true);
}


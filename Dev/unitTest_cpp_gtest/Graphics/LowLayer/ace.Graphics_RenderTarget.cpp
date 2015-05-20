
#include "../ace.Graphics_Test_Utls.h"

static const char* dx_vs = R"(
struct VS_Input
{
	float3 Pos		: Pos0;
	float2 UV		: UV0;
};

struct VS_Output
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
};

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;
	Output.Pos = float4( Input.Pos.x, Input.Pos.y, Input.Pos.z, 1.0 );
	Output.UV = Input.UV;
	return Output;
}

)";

static const char* dx_ps = R"(

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );


struct PS_Input
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
};


float4 main( const PS_Input Input ) : SV_Target
{
	float4 Output = g_texture.Sample(g_sampler, Input.UV);
	if(Output.a == 0.0f) discard;
	return Output;
}

)";

static const char* gl_vs = R"(

in vec3 Pos;
in vec2 UV;

out vec4 vaTexCoord;

void main()
{
	gl_Position = vec4(Pos.x,Pos.y,Pos.z,1.0);
	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);
}

)";

static const char* gl_ps = R"(

in vec4 vaTexCoord;
out vec4 outColor;
uniform sampler2D g_texture;

void main() 
{
outColor = texture(g_texture, vaTexCoord.xy);
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

void Graphics_RenderTarget(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	ace::Log* log = ace::Log_Imp::Create(L"graphics.html", L"描画");
	
	auto window = ace::Window_Imp::Create(640, 480, ace::ToAString(L"SingleTexture").c_str());
	ASSERT_TRUE(window != nullptr);

	auto file = ace::File_Imp::Create();
	ASSERT_TRUE(file != nullptr);

	auto graphics = ace::Graphics_Imp::Create(window, isOpenGLMode ? ace::GraphicsDeviceType::OpenGL : ace::GraphicsDeviceType::DirectX11, log, file, false, false);
	ASSERT_TRUE(graphics != nullptr);

	auto texture = graphics->CreateTexture2D(ace::ToAString(L"Data/Texture/Sample1.png").c_str());
	ASSERT_TRUE(texture != nullptr);

	auto vertexBuffer = graphics->CreateVertexBuffer_Imp(sizeof(Vertex), 4, false);
	ASSERT_TRUE(vertexBuffer != nullptr);

	auto vertexBufferSmall = graphics->CreateVertexBuffer_Imp(sizeof(Vertex), 4, false);
	ASSERT_TRUE(vertexBufferSmall != nullptr);

	auto indexBuffer = graphics->CreateIndexBuffer_Imp(6, false, false);
	ASSERT_TRUE(indexBuffer != nullptr);
	
	std::vector<ace::VertexLayout> vl;
	vl.push_back(ace::VertexLayout("Pos", ace::VertexLayoutFormat::R32G32B32_FLOAT));
	vl.push_back(ace::VertexLayout("UV", ace::VertexLayoutFormat::R32G32_FLOAT));

	auto renderTexture = graphics->CreateRenderTexture2D_Imp(320, 240, ace::TextureFormat::R8G8B8A8_UNORM);
	ASSERT_TRUE(renderTexture != nullptr);

	auto depthBuffer = graphics->CreateDepthBuffer_Imp(320, 240);
	ASSERT_TRUE(depthBuffer != nullptr);

	std::shared_ptr<ace::NativeShader_Imp> shader;
	std::vector<ace::Macro> macro;
	if (isOpenGLMode)
	{
		shader = graphics->CreateShader_Imp(
			gl_vs,
			"vs",
			gl_ps,
			"ps",
			vl,
			macro);
	}
	else
	{
		shader = graphics->CreateShader_Imp(
			dx_vs,
			"vs",
			dx_ps,
			"ps",
			vl,
			macro);
	}
	
	ASSERT_TRUE(shader != nullptr);
	
	{
		vertexBuffer->Lock();
		auto vb = vertexBuffer->GetBuffer<Vertex>(4);
		vb[0] = Vertex(ace::Vector3DF(-1.0f, 1.0f, 0.5f), ace::Vector2DF(0.0f, 0.0f));
		vb[1] = Vertex(ace::Vector3DF(0.5f, 1.0f, 0.5f), ace::Vector2DF(1.0f, 0.0f));
		vb[2] = Vertex(ace::Vector3DF(0.5f, -1.0f, 0.5f), ace::Vector2DF(1.0f, 1.0f));
		vb[3] = Vertex(ace::Vector3DF(-1.0f, -1.0f, 0.5f), ace::Vector2DF(0.0f, 1.0f));
		vertexBuffer->Unlock();
	}
	
	{
		vertexBufferSmall->Lock();
		auto vb = vertexBufferSmall->GetBuffer<Vertex>(4);
		vb[0] = Vertex(ace::Vector3DF(-1.0f, 0.0f, 0.5f), ace::Vector2DF(0.0f, 0.0f));
		vb[1] = Vertex(ace::Vector3DF(0.5f, 0.0f, 0.5f), ace::Vector2DF(1.0f, 0.0f));
		vb[2] = Vertex(ace::Vector3DF(0.5f, -1.0f, 0.5f), ace::Vector2DF(1.0f, 1.0f));
		vb[3] = Vertex(ace::Vector3DF(-1.0f, -1.0f, 0.5f), ace::Vector2DF(0.0f, 1.0f));
		vertexBufferSmall->Unlock();
	}

	{
		indexBuffer->Lock();
		auto ib = indexBuffer->GetBuffer<uint16_t>(6);
		ib[0] = 0;
		ib[1] = 1;
		ib[2] = 2;
		ib[3] = 0;
		ib[4] = 2;
		ib[5] = 3;
		indexBuffer->Unlock();
	}

	shader->SetTexture("g_texture", texture.get(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp, 0);

	int32_t time = 0;
	while (window->DoEvent())
	{
		graphics->Begin();
		
		graphics->SetRenderTarget(renderTexture, depthBuffer);
		{
			graphics->Clear(true, false, ace::Color(64, 32, time % 255, 255));

			shader->SetTexture("g_texture", texture.get(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp, 0);
			
			graphics->SetVertexBuffer(vertexBuffer.get());
			graphics->SetIndexBuffer(indexBuffer.get());
			graphics->SetShader(shader.get());

			ace::RenderState state;
			state.DepthTest = false;
			state.DepthWrite = false;
			graphics->SetRenderState(state);

			graphics->DrawPolygon(2);
		}

		
		graphics->SetRenderTarget(nullptr,nullptr);
		{
			graphics->Clear(true, false, ace::Color(0, 0, 0, 255));
			shader->SetTexture("g_texture", renderTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp, 0);
			graphics->SetVertexBuffer(vertexBufferSmall.get());
			graphics->SetIndexBuffer(indexBuffer.get());
			graphics->SetShader(shader.get());

			ace::RenderState state;
			state.DepthTest = false;
			state.DepthWrite = false;

			graphics->SetRenderState(state);
			graphics->DrawPolygon(2);
		}

		graphics->Present();

		graphics->End();

		if (time == 10)
		{
			SAVE_SCREEN_SHOT(graphics,0);
		}

		if (time == 11)
		{
			window->Close();
		}
		time++;
	}

	depthBuffer->Release();
	renderTexture->Release();
	graphics->Release();
	file->Release();
	texture.reset();
	vertexBuffer.reset();
	indexBuffer.reset();
	shader.reset();

	window->Release();
	delete log;
}

#ifdef _WIN32
TEST(Graphics, RenderTarget_DX)
{
	Graphics_RenderTarget(false);
}
#endif

TEST(Graphics, RenderTarget_GL)
{
	Graphics_RenderTarget(true);
}


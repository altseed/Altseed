
#include "../ace.Graphics_Test_Utls.h"
#include <Graphics/ace.RenderingThread.h>
#include <Graphics/Command/ace.RenderingCommand.h>
#include <Graphics/Command/ace.RenderingCommandExecutor.h>
#include <Graphics/Command/ace.RenderingCommandFactory.h>
#include <Graphics/Command/ace.RenderingCommandHelper.h>

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

attribute vec3 Pos;
attribute vec2 UV;

varying vec4 vaTexCoord;

void main()
{
	gl_Position = vec4(Pos.x,Pos.y,Pos.z,1.0);
	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);

	
}

)";

static const char* gl_ps = R"(

varying vec4 vaTexCoord;
uniform sampler2D g_texture;

void main() 
{
	// varying(in) は変更不可(Radeon)

	vec4 uv = vaTexCoord;
	uv.y = 1.0 - uv.y; 
	gl_FragColor = texture2D(g_texture, uv.xy);
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

static ace::RenderingCommandFactory* factory = nullptr;
static ace::RenderingCommandExecutor* executor = nullptr;
static std::vector<ace::RenderingCommand*> commands;
static std::shared_ptr<ace::Texture2D> texture;
static std::shared_ptr<ace::VertexBuffer_Imp> vertexBuffer;
static std::shared_ptr<ace::IndexBuffer_Imp> indexBuffer;
static std::shared_ptr<ace::NativeShader_Imp> shader;
static ace::RenderTexture2D_Imp* renderTexture = nullptr;

static void Rendering()
{
	using h = ace::RenderingCommandHelper;

	auto helper = ace::RenderingCommandHelper(commands, factory);

	helper.Clear(true, false, ace::Color(64, 32, 16, 255));

	helper.SetRenderTarget(renderTexture, nullptr);

	for (auto i = 0; i < 100; i++)
	{
		ace::RenderState state;
		state.DepthTest = false;
		state.DepthWrite = false;

		helper.Draw(2, vertexBuffer.get(), indexBuffer.get(), shader.get(), state,
			h::GenValue("g_texture", h::Texture2DPair(texture.get(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp))
			);
	}

	helper.SetRenderTarget(nullptr, nullptr);

	for (auto i = 0; i < 100; i++)
	{
		ace::RenderState state;
		state.DepthTest = false;
		state.DepthWrite = false;
	
		helper.Draw(2, vertexBuffer.get(), indexBuffer.get(), shader.get(), state,
			h::GenValue("g_texture", h::Texture2DPair(texture.get(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp))
			);
	}
}

class RenderingEvent
	: public ace::RenderingThreadEvent
{
public:
	RenderingEvent(){}
	virtual ~RenderingEvent(){}
	void Event()
	{
		Rendering();
	}
};

void Graphics_RenderingThread(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	ace::Log* log = ace::Log_Imp::Create(L"graphics.html", L"描画");

	auto window = ace::Window_Imp::Create(640, 480, ace::ToAString(L"SingleTexture").c_str());
	ASSERT_TRUE(window != nullptr);

	auto graphics = ace::Graphics_Imp::Create(window, isOpenGLMode ? ace::GraphicsDeviceType::OpenGL : ace::GraphicsDeviceType::DirectX11, log, false);
	ASSERT_TRUE(graphics != nullptr);

	texture = graphics->CreateTexture2D(ace::ToAString(L"Data/Texture/Sample1.png").c_str());
	ASSERT_TRUE(texture != nullptr);

	vertexBuffer = graphics->CreateVertexBuffer_Imp(sizeof(Vertex), 4, false);
	ASSERT_TRUE(vertexBuffer != nullptr);

	indexBuffer = graphics->CreateIndexBuffer_Imp(6, false, false);
	ASSERT_TRUE(indexBuffer != nullptr);

	renderTexture = graphics->CreateRenderTexture2D_Imp(320, 240, ace::TextureFormat::R8G8B8A8_UNORM);
	ASSERT_TRUE(renderTexture != nullptr);

	std::vector<ace::VertexLayout> vl;
	vl.push_back(ace::VertexLayout("Pos", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
	vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));

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

	factory = new ace::RenderingCommandFactory();
	executor = new ace::RenderingCommandExecutor();

	int32_t time = 0;
	RenderingEvent e;

	while (window->DoEvent())
	{
		graphics->Begin();
		graphics->GetRenderingThread()->AddEvent(&e);
		
		while (!e.IsExited())
		{
			ace::Sleep(1);
		}

		executor->Execute(graphics, nullptr, nullptr, commands);

		for (auto& command : commands)
		{
			command->~RenderingCommand();
		}
		commands.clear();
		factory->Reset();

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

	delete factory;
	delete executor;

	renderTexture->Release();
	graphics->Release();
	texture.reset();
	vertexBuffer.reset();
	indexBuffer.reset();
	shader.reset();

	window->Release();
	delete log;
}

TEST(Graphics, RenderingThread_DX)
{
	Graphics_RenderingThread(false);
}


TEST(Graphics, RenderingThread_GL)
{
	Graphics_RenderingThread(true);
}


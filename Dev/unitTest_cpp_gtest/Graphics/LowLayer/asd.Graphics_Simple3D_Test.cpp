
#include "../asd.Graphics_Test_Utls.h"

static const char* dx_vs = R"(

float4x4 matMCP		: register( c0 );

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
	Output.Pos = mul( matMCP, float4( Input.Pos.x, Input.Pos.y, Input.Pos.z, 1.0 ) );
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

uniform mat4 matMCP;

out vec4 vaTexCoord;

void main()
{
	gl_Position = matMCP * vec4(Pos.x,Pos.y,Pos.z,1.0);
	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);
}

)";

static const char* gl_ps = R"(

in vec4 vaTexCoord;
out vec4 outColor;
uniform sampler2D g_texture;

void main() 
{
	// varying(in) は変更不可(Radeon)

	vec4 uv = vaTexCoord;
	uv.y = 1.0 - uv.y; 
	outColor = texture(g_texture, uv.xy);
}

)";

struct Vertex
{
	asd::Vector3DF Pos;
	asd::Vector2DF UV;

	Vertex() {}
	Vertex(asd::Vector3DF pos, asd::Vector2DF uv)
	{
		Pos = pos;
		UV = uv;
	}
};

void Graphics_Simple3D(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	asd::Log* log = asd::Log_Imp::Create(u"graphics.html", u"描画");

	auto window = asd::Window_Imp::Create(640, 480, asd::ToAString(u"Simple3D").c_str(), log, asd::WindowPositionType::Default, isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11, asd::ColorSpaceType::LinearSpace, false);
	ASSERT_TRUE(window != nullptr);

	auto file = asd::File_Imp::Create();
	ASSERT_TRUE(file != nullptr);

	asd::GraphicsOption go;
	go.IsFullScreen = false;
	go.IsReloadingEnabled = false;
	go.ColorSpace = asd::ColorSpaceType::LinearSpace;
	go.GraphicsDevice = isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11;
	auto graphics = asd::Graphics_Imp::Create(window, isOpenGLMode ? asd::GraphicsDeviceType::OpenGL : asd::GraphicsDeviceType::DirectX11, log, file, go);
	ASSERT_TRUE(graphics != nullptr);

	auto texture = graphics->CreateTexture2D(asd::ToAString(u"Data/Texture/Sample1.png").c_str());
	ASSERT_TRUE(texture != nullptr);

	auto vertexBuffer = graphics->CreateVertexBuffer_Imp(sizeof(Vertex), 8, false);
	ASSERT_TRUE(vertexBuffer != nullptr);

	auto indexBuffer = graphics->CreateIndexBuffer_Imp(36, false, false);
	ASSERT_TRUE(indexBuffer != nullptr);

	std::vector<asd::VertexLayout> vl;
	vl.push_back(asd::VertexLayout("Pos", asd::VertexLayoutFormat::R32G32B32_FLOAT));
	vl.push_back(asd::VertexLayout("UV", asd::VertexLayoutFormat::R32G32_FLOAT));

	bool const is32bit = false;
	std::shared_ptr<asd::NativeShader_Imp> shader;
	std::vector<asd::Macro> macro;
	if (isOpenGLMode)
	{
		shader = graphics->CreateShader_Imp(
			gl_vs,
			"vs",
			gl_ps,
			"ps",
			vl,
			is32bit,
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
			is32bit,
			macro);
	}

	ASSERT_TRUE(shader != nullptr);

	{
		vertexBuffer->Lock();
		auto vb = vertexBuffer->GetBuffer<Vertex>(8);
		vb[0] = Vertex(asd::Vector3DF(-1.0f, 1.0f, -1.0f), asd::Vector2DF(0.0f, 0.0f));
		vb[1] = Vertex(asd::Vector3DF(1.0f, 1.0f, -1.0f), asd::Vector2DF(1.0f, 0.0f));
		vb[2] = Vertex(asd::Vector3DF(1.0f, 1.0f, 1.0f), asd::Vector2DF(1.0f, 1.0f));
		vb[3] = Vertex(asd::Vector3DF(-1.0f, 1.0f, 1.0f), asd::Vector2DF(0.0f, 1.0f));
		vb[4] = Vertex(asd::Vector3DF(-1.0f, -1.0f, -1.0f), asd::Vector2DF(0.0f, 0.0f));
		vb[5] = Vertex(asd::Vector3DF(1.0f, -1.0f, -1.0f), asd::Vector2DF(1.0f, 0.0f));
		vb[6] = Vertex(asd::Vector3DF(1.0f, -1.0f, 1.0f), asd::Vector2DF(1.0f, 1.0f));
		vb[7] = Vertex(asd::Vector3DF(-1.0f, -1.0f, 1.0f), asd::Vector2DF(0.0f, 1.0f));
		vertexBuffer->Unlock();
	}

	{
		indexBuffer->Lock();
		auto ib = indexBuffer->GetBuffer<uint16_t>(36);
		auto setBuf = [&](int32_t i0, int32_t i1, int32_t i2, int32_t i3, int32_t offset) -> void
		{
			ib[offset + 0] = i0; ib[offset + 1] = i1; ib[offset + 2] = i2; ib[offset + 3] = i0; ib[offset + 4] = i2; ib[offset + 5] = i3;
		};

		setBuf(0, 1, 2, 3, 0);
		setBuf(3, 2, 6, 7, 6);
		setBuf(2, 1, 5, 6, 12);
		setBuf(0, 3, 7, 4, 18);
		setBuf(1, 0, 4, 5, 24);
		setBuf(5, 4, 7, 6, 30);
		
		indexBuffer->Unlock();
	}

	int32_t time = 0;
	while (window->DoEvent())
	{
		graphics->Begin();

		graphics->Clear(true, true, asd::Color(64, 32, 16, 255));

		asd::Matrix44 matP, matC, matM, mat;
		matM.SetRotationY((float) time * 0.002f);
		if (isOpenGLMode)
		{
			matP.SetPerspectiveFovRH_OpenGL(3.14f / 4.0f, 4.0f / 3.0f, 0.1f, 20.0f);
		}
		else
		{
			matP.SetPerspectiveFovRH(3.14f / 4.0f, 4.0f / 3.0f, 0.1f, 20.0f);
		}
		matC.SetLookAtRH(asd::Vector3DF(2.0f, 2.0f, 5.0f), asd::Vector3DF(), asd::Vector3DF(0.0f, 1.0f, 0.0f));
		
		asd::Matrix44::Mul(mat, matC, matM);
		asd::Matrix44::Mul(mat, matP, mat);

		shader->SetTexture("g_texture", texture.get(), asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp, 0);
		shader->SetMatrix44("matMCP", mat);
		graphics->SetVertexBuffer(vertexBuffer.get());
		graphics->SetIndexBuffer(indexBuffer.get());
		graphics->SetShader(shader.get());

		asd::RenderState state;
		
		state.DepthTest = true;
		state.DepthWrite = true;
		state.Culling = asd::CullingType::Front;
		graphics->SetRenderState(state);

		graphics->DrawPolygon(2*6);
		
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
TEST(Graphics, Simple3D_DX)
{
	Graphics_Simple3D(false);
}
#endif

TEST(Graphics, Simple3D_GL)
{
	Graphics_Simple3D(true);
}


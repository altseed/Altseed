
#include "../ace.Graphics_Test_Utls.h"

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

attribute vec3 Pos;
attribute vec2 UV;

uniform mat4 matMCP;

varying vec4 vaTexCoord;

void main()
{
	gl_Position = matMCP * vec4(Pos.x,Pos.y,Pos.z,1.0);
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

void Graphics_Simple3D(bool isOpenGLMode)
{
	StartGraphicsTest();
	SetGLEnable(isOpenGLMode);

	ace::Log* log = ace::Log_Imp::Create(L"graphics.html", L"描画");

	auto window = ace::Window_Imp::Create(640, 480, ace::ToAString(L"Simple3D").c_str());
	ASSERT_TRUE(window != nullptr);

	auto graphics = ace::Graphics_Imp::Create(window, isOpenGLMode, log, false);
	ASSERT_TRUE(graphics != nullptr);

	auto texture = graphics->CreateTexture2D(ace::ToAString(L"Data/Texture/Sample1.png").c_str());
	ASSERT_TRUE(texture != nullptr);

	auto vertexBuffer = graphics->CreateVertexBuffer_Imp(sizeof(Vertex), 8, false);
	ASSERT_TRUE(vertexBuffer != nullptr);

	auto indexBuffer = graphics->CreateIndexBuffer_Imp(36, false, false);
	ASSERT_TRUE(indexBuffer != nullptr);

	std::vector<ace::VertexLayout> vl;
	vl.push_back(ace::VertexLayout("Pos", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
	vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));

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

	std::vector<ace::ConstantBufferInformation> constantBuffers;
	constantBuffers.resize(1);
	constantBuffers[0].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
	constantBuffers[0].Name = std::string("matMCP");
	constantBuffers[0].Offset = 0;
	
	shader->CreateVertexConstantBuffer<ace::Matrix44>(constantBuffers);

	{
		vertexBuffer->Lock();
		auto vb = vertexBuffer->GetBuffer<Vertex>(8);
		vb[0] = Vertex(ace::Vector3DF(-1.0f, 1.0f, -1.0f), ace::Vector2DF(0.0f, 0.0f));
		vb[1] = Vertex(ace::Vector3DF(1.0f, 1.0f, -1.0f), ace::Vector2DF(1.0f, 0.0f));
		vb[2] = Vertex(ace::Vector3DF(1.0f, 1.0f, 1.0f), ace::Vector2DF(1.0f, 1.0f));
		vb[3] = Vertex(ace::Vector3DF(-1.0f, 1.0f, 1.0f), ace::Vector2DF(0.0f, 1.0f));
		vb[4] = Vertex(ace::Vector3DF(-1.0f, -1.0f, -1.0f), ace::Vector2DF(0.0f, 0.0f));
		vb[5] = Vertex(ace::Vector3DF(1.0f, -1.0f, -1.0f), ace::Vector2DF(1.0f, 0.0f));
		vb[6] = Vertex(ace::Vector3DF(1.0f, -1.0f, 1.0f), ace::Vector2DF(1.0f, 1.0f));
		vb[7] = Vertex(ace::Vector3DF(-1.0f, -1.0f, 1.0f), ace::Vector2DF(0.0f, 1.0f));
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

		graphics->Clear(true, true, ace::Color(64, 32, 16, 255));

		ace::Matrix44 matP, matC, matM, mat;
		matM.SetRotationY((float) time * 0.002f);
		if (isOpenGLMode)
		{
			matP.SetPerspectiveFovRH_OpenGL(3.14f / 4.0f, 4.0f / 3.0f, 0.1f, 20.0f);
		}
		else
		{
			matP.SetPerspectiveFovRH(3.14f / 4.0f, 4.0f / 3.0f, 0.1f, 20.0f);
		}
		matC.SetLookAtRH(ace::Vector3DF(2.0f, 2.0f, 5.0f), ace::Vector3DF(), ace::Vector3DF(0.0f, 1.0f, 0.0f));
		
		ace::Matrix44::Mul(mat, matC, matM);
		ace::Matrix44::Mul(mat, matP, mat);

		auto& mat_ = shader->GetVertexConstantBuffer<ace::Matrix44>();
		mat_ = mat;

		shader->SetTexture("g_texture", texture.get(), 0);
		graphics->SetVertexBuffer(vertexBuffer.get());
		graphics->SetIndexBuffer(indexBuffer.get());
		graphics->SetShader(shader.get());

		auto& state = graphics->GetRenderState()->Push();
		state.DepthTest = true;
		state.DepthWrite = true;
		state.CullingType = ace::eCullingType::CULLING_FRONT;
		state.TextureWrapTypes[0] = ace::TextureWrapType::Clamp;
		graphics->GetRenderState()->Update(false);

		graphics->DrawPolygon(2*6);

		graphics->GetRenderState()->Pop();
		
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
	texture.reset();
	vertexBuffer.reset();
	indexBuffer.reset();
	shader.reset();

	window->Release();
	delete log;
}

TEST(Graphics, Simple3D_DX)
{
	Graphics_Simple3D(false);
}

TEST(Graphics, Simple3D_GL)
{
	Graphics_Simple3D(true);
}



#include "ace.Renderer3D.h"
#include "ace.RenderedObject3D.h"
#include "ace.RenderedCameraObject3D.h"
#include "ace.RenderedDirectionalLightObject3D.h"
#include "../ace.Graphics_Imp.h"

#include "../Resource/ace.NativeShader_Imp.h"
#include "../Resource/ace.VertexBuffer_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"
#include "../Resource/ace.RenderState_Imp.h"

#if _WIN32
#include "../../DX11/ace.Graphics_Imp_DX11.h"
#endif
#include "../../GL/ace.Graphics_Imp_GL.h"

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
static const char* paste_dx_vs = R"(
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

float4 Size			: register( c0 );

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;

	Output.Pos.x = Input.Pos.x / Size.x * 2.0 - 1.0;
	Output.Pos.y = -(Input.Pos.y / Size.y * 2.0 - 1.0);
 
	Output.Pos.z = 0.5;
	Output.Pos.w = 1.0;

	Output.UV = Input.UV;
	return Output;
}

)";

static const char* paste_dx_ps = R"(

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

static const char* paste_gl_vs = R"(

attribute vec3 Pos;
attribute vec2 UV;

varying vec4 vaTexCoord;

uniform vec4 Size;

void main()
{
	gl_Position.x = Pos.x / Size.x * 2.0 - 1.0;
	gl_Position.y = -(Pos.y / Size.y * 2.0 - 1.0);

	gl_Position.z = 0.5;
	gl_Position.w = 1.0;

	vaTexCoord = vec4(UV.x,UV.y,0.0,0.0);
}

)";

static const char* paste_gl_ps = R"(

varying vec4 vaTexCoord;

uniform sampler2D g_texture;

void main() 
{
	// varying(in) は変更不可(Radeon)

	// gl only
	vec4 vaTexCoord_ = vaTexCoord;
	vaTexCoord_.y = 1.0 - vaTexCoord_.y;

	gl_FragColor = texture2D(g_texture, vaTexCoord_.xy);
}

)";

static const char* shadowBlur_dx_vs = R"(

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

static const char* shadowBlur_gl_vs = R"(

attribute vec3 Pos;
attribute vec2 UV;
attribute vec4 Color;

varying vec4 inPos;
varying vec2 inUV;
varying vec4 inColor;

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

static const char* shadowBlur_dx_ps_x = R"(

struct PS_Input
{
	float4 Pos_		: SV_POSITION;
	float4 Pos		: Pos0;
	float2 UV		: TEXCOORD0;
	float4 Color	: COLOR0;
};

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );
float4          g_weight        : register( c0 );
float4 main( const PS_Input Input ) : SV_Target
{
	uint width, height;
	g_texture.GetDimensions(width, height);
	float2 accum = float2(1.0 / width, 0.0);
	float2 half_ = float2(0.5 / width, 0.0);
	float2 adder = float2(2.0 / width, 0.0);
	
	float4 output_ = (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	                 g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	                 g_weight.x;
	if(output_.a == 0.0f) discard;
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.y;
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.z;
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.w;
	return output_;
}

)";

static const char* shadowBlur_gl_ps_x = R"(

varying vec4 inPos;
varying vec2 inUV;
varying vec4 inColor;

uniform sampler2D g_texture;
uniform vec4      g_weight;
void main()
{
	vec2 accum = vec2(1.0 / float(textureSize(g_texture, 0).x), 0.0);
	vec2 half_  = vec2(0.5 / float(textureSize(g_texture, 0).x), 0.0);
	vec2 adder = vec2(2.0 / float(textureSize(g_texture, 0).x), 0.0);
	vec4 output_ = (texture2D(g_texture, inUV.xy + half_ + accum)  +
	               texture2D(g_texture, inUV.xy + half_ - accum)) *
	               g_weight.x;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.y;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.z;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.w;
	gl_FragColor = output_; 
}

)";


static const char* shadowBlur_dx_ps_y = R"(

struct PS_Input
{
	float4 Pos_		: SV_POSITION;
	float4 Pos		: Pos0;
	float2 UV		: TEXCOORD0;
	float4 Color	: COLOR0;
};

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );
float4          g_weight        : register( c0 );
float4 main( const PS_Input Input ) : SV_Target
{
	uint width, height;
	g_texture.GetDimensions(width, height);
	float2 accum = float2(0.0, 1.0 / height);
	float2 half_ = float2(0.0, 0.5 / height);
	float2 adder = float2(0.0, 2.0 / height);
	
	float4 output_ = (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	                 g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	                 g_weight.x;
	if(output_.a == 0.0f) discard;

	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.y;
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.z;
	accum += adder;
	output_ += (g_texture.Sample(g_sampler, Input.UV + half_ + accum) +
	           g_texture.Sample(g_sampler, Input.UV + half_ - accum)) *
	           g_weight.w;
	return output_;
}

)";

static const char* shadowBlur_gl_ps_y = R"(

varying vec4 inPos;
varying vec2 inUV;
varying vec4 inColor;

uniform sampler2D g_texture;
uniform vec4      g_weight;
void main()
{
	vec2 accum = vec2(0.0, 1.0 / float(textureSize(g_texture, 0).y));
	vec2 half_  = vec2(0.0, 0.5 / float(textureSize(g_texture, 0).y));
	vec2 adder = vec2(0.0, 2.0 / float(textureSize(g_texture, 0).y));
	
	vec4 output_ = (texture2D(g_texture, inUV.xy + half_ + accum)  +
	               texture2D(g_texture, inUV.xy + half_ - accum)) *
	               g_weight.x;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.y;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.z;
	accum += adder;
	output_ += (texture2D(g_texture, inUV.xy + half_ + accum)  +
	          texture2D(g_texture, inUV.xy + half_ - accum)) *
	          g_weight.w;
	gl_FragColor = output_; 
}

)";

static const char* ssao_dx_vs = R"(
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

float4 Size			: register( c0 );

VS_Output main( const VS_Input Input )
{
	VS_Output Output = (VS_Output)0;

	Output.Pos.x = Input.Pos.x;
	Output.Pos.y = Input.Pos.y;
 
	Output.Pos.z = 0.5;
	Output.Pos.w = 1.0;

	Output.UV = Input.UV;
	Output.UV.y = 1.0 - Output.UV.y;
	return Output;
}

)";

static const char* ssao_dx_ps = R"(

struct PS_Input
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
};

// サンプル数
#define NUM_SAMPLES (13)

// サンプル時の回転数
#define NUM_TURNS (7)

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );


float radius		: register( c0 );

// 1mの位置にサイズ1のオブジェクトを出した時の縦方向ピクセル数
float projScale		: register( c1 );
float bias			: register( c2 );
float intensity		: register( c3 );

// 座標再構成情報
// perspective
// x = n・f
// y = f-n
// z = -f

// ace
// x = f-n
// y = n
float3 reconstructInfo1	: register( c4 );

// x = 1/xScale
// y = 1/yScale
float4 reconstructInfo2	: register( c5 );

float3 GetNormal(float2 uv)
{
	return g_texture.Sample(g_sampler, uv).xyz * 2.0 - 1.0;
}

float GetZ(float2 uv)
{
	return g_texture.Sample(g_sampler, uv).w;
}

float ReconstructDepth(float z)
{
	//return reconstructInfo1.x / (reconstructInfo1.y * z + reconstructInfo1.z);
	return -((z * reconstructInfo1.x) + reconstructInfo1.y);
}

float3 ReconstructPosition(float2 screenXY, float depth)
{
	return float3( reconstructInfo2.xy * screenXY * (-depth), depth );
}

float2 GetScreenPos(float2 uv)
{
	return float2(uv.x * 2.0 - 1.0, 1.0 - uv.y * 2.0 );
}

int2 GetScreenPixelPos(float2 uv)
{
	uint width, height;
	g_texture.GetDimensions(width, height);
	return int2(width*uv.x,height*uv.y);
}

// 球内のサンプルする位置のオフセットをユニット単位で取得する。
float3 GetSphereOffset(int index, float offset)
{
	float alpha = float(index + 0.5) * (1.0 / NUM_SAMPLES);
	float angle = alpha * (NUM_TURNS * 6.28) + offset;

	return float3(cos(angle), sin(angle), alpha);
}


float4 SampleAO(float3 centerPos, float2 centerUV, float3 normal, float sRadius, float random, int index)
{
	uint width, height;
	g_texture.GetDimensions(width, height);

	float3 offset = GetSphereOffset(index, random );

	float2 targetUV = centerUV;
	targetUV.x = targetUV.x + offset.x * sRadius * offset.z / (float)width;
	targetUV.y = targetUV.y + offset.y * sRadius * offset.z / (float)height;

	float3 targetPos = ReconstructPosition( GetScreenPos(targetUV), ReconstructDepth(GetZ(targetUV)));
	
	float3 v = targetPos - centerPos;

	float vv = dot(v, v);
	float vn = dot(v, normal);

	// Scalable Ambient Obscurance記載
	const float epsilon = 0.01;
    float f = max(radius * radius - vv, 0.0);

	return f * f * f * max((vn - bias) / (epsilon + vv), 0.0);
}

float2 CompressValue(float value)
{
	float temp = floor(value * 256.0);
	return float2( temp * (1.0 / 256.0), value * 256.0 - temp );
}


float4 main( const PS_Input Input ) : SV_Target
{
	int2 sPos = GetScreenPixelPos(Input.UV);
	float3 centerPos = ReconstructPosition( GetScreenPos(Input.UV), ReconstructDepth(GetZ(Input.UV)));

	// 中心の奥行きでの球の半径を計算(カメラ座標系のため、zはマイナス)
	float sRadius = -projScale * radius / centerPos.z;

	// Scalable Ambient Obscurance記載のランダム
	float random = (3 * sPos.x ^ sPos.y + sPos.x * sPos.y) * 10;

	float3 normal =  GetNormal(Input.UV);

	float sum = 0.0;
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		sum += SampleAO(centerPos, Input.UV, normal, sRadius, random, i);
	}

	// Scalable Ambient Obscurance記載のAの強さの算出方法
	float A = max( 0.0, 1.0 - intensity * sum * 5.0 / (radius * radius * radius * radius * radius * radius * NUM_SAMPLES) );

	// Bilateral box-filter
	if (abs(ddx(centerPos.z)) < 0.02)
	{
		A -= ddx(A) * ((sPos.x & 1) - 0.5);
	}
	if (abs(ddy(centerPos.z)) < 0.02)
	{
		A -= ddy(A) * ((sPos.y & 1) - 0.5);
	}

	float2 compressedDepth = CompressValue(centerPos.z);
	return float4(A,compressedDepth.x,compressedDepth.y,1.0);
}

)";

	static const char* ssao_blur_dx_ps = R"(

struct PS_Input
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD0;
};

Texture2D		g_texture		: register( t0 );
SamplerState	g_sampler		: register( s0 );

float GetValue(float2 uv)
{
	return g_texture.Sample(g_sampler, uv).x;
}

float2 GetCompressedDepth(float2 uv)
{
	return g_texture.Sample(g_sampler, uv).yz;
}

float DecompressValue(float2 compressed)
{
	return compressed.x * (256.0 / 257.0) + compressed.y * (1.0 / 257.0);
}

float GetDepth(float2 uv)
{
	float2 compressed = GetCompressedDepth(uv);
	return DecompressValue(compressed);
}

float4 main( const PS_Input Input ) : SV_Target
{
	const int radius = 4;
	const float scale = 2.0;
	const float centerOffset = 0.5;
	const float gaussian[] = { 0.144760504, 0.129537389, 0.103725441, 0.0743225217, 0.0476541445 };
	const float intensity = 200.0;

	uint width, height;
	g_texture.GetDimensions(width, height);
	float scaleX = scale / (float)width;
	float scaleY = scale / (float)height;
	float centerOffsetX = centerOffset / (float)width;
	float centerOffsetY = centerOffset / (float)height;

	float baseDepth = GetDepth( Input.UV );

	float sum = 0.0;
	float weightSum = 0.0;

	[unroll]
	for(int r = -radius; r <= radius; r++)
	{
#if BLUR_X
		float2 uv = Input.UV + float2(r * scaleX + centerOffsetX, 0.0 + centerOffsetY);
#endif

#if BLUR_Y
		float2 uv = Input.UV + float2(0.0 + centerOffsetX, r * scaleY + centerOffsetY);
#endif

		float depth = GetDepth( uv );
		
		float weight =  gaussian[abs(r)];
		weight = weight * max(0.0, 1.0 - intensity * abs(depth-baseDepth) );
		
		float value = GetValue(uv);
		
		sum += value * weight;
		weightSum += weight;
	}

	const float epsilon = 0.0001;
	float a = (sum + epsilon) / (weightSum + epsilon);

	return float4(a, GetCompressedDepth(Input.UV), 1.0 );
}

)";

	template<typename T>
	void AddRefToObjects(std::set<T*>& os)
	{
		for (auto& o : os)
		{
			SafeAddRef(o);
		}
	}

	template<typename T>
	void ReleaseObjects(std::set<T*>& os)
	{
		for (auto& o : os)
		{
			o->Release();
		}
		os.clear();
	}

	template<typename T>
	void CallRemovingObjects(std::set<T*>& os, Renderer3D* renderer)
	{
		for (auto& o : os)
		{
			o->OnRemoving(renderer);
		}
	}

	Renderer3D::RenderingEvent::RenderingEvent(Renderer3D* renderer)
		: m_renderer(renderer)
	{
	
	}

	Renderer3D::RenderingEvent::~RenderingEvent()
	{
	
	}

	void Renderer3D::RenderingEvent::Event()
	{
		m_renderer->Rendering();
	}

	void Renderer3D::Rendering()
	{
		auto g = m_graphics;
		g->MakeContextCurrent();

		for (auto& o : rendering.objects)
		{
			o->CalculateMatrix_FR();
		}

		for (auto& o : rendering.cameraObjects)
		{
			o->CalculateMatrix_FR();
		}

		for (auto& o : rendering.directionalLightObjects)
		{
			o->CalculateMatrix_FR();
		}

		// エフェクトの更新
		rendering.EffectManager->Update(1.0f);

		RenderingProperty prop;
		prop.ShadowMapPtr = nullptr;
		
		// ライトの計算
		{
			if (rendering.directionalLightObjects.size() > 0)
			{
				auto light = (RenderedDirectionalLightObject3D*) (*(rendering.directionalLightObjects.begin()));
				prop.DirectionalLightColor = light->GetColor_FR();
				prop.DirectionalLightDirection = light->GetDirection_FR();
				prop.DirectionalLightDirection.X = -prop.DirectionalLightDirection.X;
				prop.DirectionalLightDirection.Y = -prop.DirectionalLightDirection.Y;
				prop.DirectionalLightDirection.Z = -prop.DirectionalLightDirection.Z;
			}
			else
			{
				prop.DirectionalLightColor = Color(255, 255, 255, 255);
				prop.DirectionalLightDirection = Vector3DF(1.0f, 1.0f, 1.0f);
			}
			prop.DirectionalLightDirection.Normalize();
		}

		for (auto& co : rendering.cameraObjects)
		{
			auto c = (RenderedCameraObject3D*) co;

			// カメラプロジェクション行列計算
			Matrix44 cameraProjMat;
			ace::Matrix44::Mul(cameraProjMat, c->GetProjectionMatrix_FR(), c->GetCameraMatrix_FR());
			prop.CameraMatrix = c->GetCameraMatrix_FR();
			prop.ProjectionMatrix = c->GetProjectionMatrix_FR();
			prop.DepthRange = c->GetZFar_FR() - c->GetZNear_FR();
			prop.ZFar = c->GetZFar_FR();
			prop.ZNear = c->GetZNear_FR();

			// シャドウマップ作成
			RenderTexture2D_Imp* shadowMap = nullptr;
			if (rendering.directionalLightObjects.size() > 0)
			{
				auto light = (RenderedDirectionalLightObject3D*) (*(rendering.directionalLightObjects.begin()));
				shadowMap = light->GetShadowTexture_FR();

				g->SetRenderTarget(light->GetShadowTexture_FR(), light->GetShadowDepthBuffer_FR());
				g->Clear(true, true, ace::Color(0, 0, 0, 255));

				Matrix44 view, proj;

				light->CalcShadowMatrix(
					c->GetPosition_FR(),
					c->GetFocus_FR() - c->GetPosition_FR(),
					cameraProjMat,
					c->GetZNear_FR(),
					c->GetZFar_FR(),
					view,
					proj);

				RenderingShadowMapProperty shadowProp;
				shadowProp.CameraMatrix = view;
				shadowProp.ProjectionMatrix = proj;
				shadowProp.DepthRange = prop.DepthRange;
				shadowProp.ZFar = prop.ZFar;
				shadowProp.ZNear = prop.ZNear;

				prop.LightCameraMatrix = shadowProp.CameraMatrix;
				prop.LightProjectionMatrix = shadowProp.ProjectionMatrix;

				for (auto& o : m_objects)
				{
					o->RenderingShadowMap(shadowProp);
				}

				float intensity = 5.0f;
				Vector4DF weights;
				float ws[4];
				float total = 0.0f;
				float const dispersion = intensity * intensity;
				for (int32_t i = 0; i < 4; i++)
				{
					float pos = 1.0f + 2.0f * i;
					ws[i] = expf(-0.5f * pos * pos / dispersion);
					total += ws[i] * 2.0f;
				}
				weights.X = ws[0] / total;
				weights.Y = ws[1] / total;
				weights.Z = ws[2] / total;
				weights.W = ws[3] / total;

				{
					g->SetRenderTarget((RenderTexture2D_Imp*) m_shadowTempTexture.get(), nullptr);
					g->Clear(true, false, ace::Color(0, 0, 0, 255));

					m_shadowShaderX->SetTexture("g_texture", light->GetShadowTexture_FR(), 0);
					ShadowConstantBuffer& cbufX = m_shadowShaderX->GetPixelConstantBuffer<ShadowConstantBuffer>();
					cbufX.Weights = weights;

					g->SetVertexBuffer(m_shadowVertexBuffer.get());
					g->SetIndexBuffer(m_shadowIndexBuffer.get());
					g->SetShader(m_shadowShaderX.get());

					auto& state = g->GetRenderState()->Push();
					state.DepthTest = false;
					state.DepthWrite = false;
					state.CullingType = CULLING_DOUBLE;
					state.TextureFilterTypes[0] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
					g->GetRenderState()->Update(false);

					g->DrawPolygon(2);

					g->GetRenderState()->Pop();
				}

				{
					g->SetRenderTarget(light->GetShadowTexture_FR(), nullptr);
					g->Clear(true, false, ace::Color(0, 0, 0, 255));

					m_shadowShaderY->SetTexture("g_texture", m_shadowTempTexture.get(), 0);
					ShadowConstantBuffer& cbufY = m_shadowShaderY->GetPixelConstantBuffer<ShadowConstantBuffer>();
					cbufY.Weights = weights;

					g->SetVertexBuffer(m_shadowVertexBuffer.get());
					g->SetIndexBuffer(m_shadowIndexBuffer.get());
					g->SetShader(m_shadowShaderY.get());

					auto& state = g->GetRenderState()->Push();
					state.DepthTest = false;
					state.DepthWrite = false;
					state.CullingType = CULLING_DOUBLE;
					state.TextureFilterTypes[0] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
					g->GetRenderState()->Update(false);

					g->DrawPolygon(2);

					g->GetRenderState()->Pop();
				}

			}
			prop.ShadowMapPtr = shadowMap;
			
			// 影用デバッグコード
			//prop.CameraProjectionMatrix = prop.LightProjectionMatrix;

			// 奥行き描画
			{
				g->SetRenderTarget(c->GetRenderTargetDepth_FR(), c->GetDepthBuffer_FR());
				g->Clear(true, true, ace::Color(0, 0, 0, 255));

				for (auto& o : m_objects)
				{
					o->RenderingNormalDepth(prop);
				}
			}

			// SSAO
			if (m_ssaoShader != nullptr)
			{
				{
					g->SetRenderTarget(c->GetRenderTargetSSAO_FR(), nullptr);
					g->Clear(true, false, ace::Color(0, 0, 0, 255));

					m_ssaoShader->SetTexture("g_texture", c->GetRenderTargetDepth_FR(), 0);

					auto& cvbuf = m_ssaoShader->GetVertexConstantBuffer<SSAOConstantVertexBuffer>();
					cvbuf.Size[0] = m_windowSize.X;
					cvbuf.Size[1] = m_windowSize.Y;

					auto fov = c->GetFieldOfView() / 180.0f * 3.141592f;
					auto aspect = (float) c->GetWindowSize().X / (float) c->GetWindowSize().Y;

					// DirectX
					float yScale = 1 / tanf(fov / 2);
					float xScale = yScale / aspect;


					SSAOConstantPixelBuffer& cpbuf = m_ssaoShader->GetPixelConstantBuffer<SSAOConstantPixelBuffer>();
					cpbuf.Radius = 0.1f;
					cpbuf.ProjScale = c->GetWindowSize().Y * yScale / 2.0f;
					cpbuf.Bias = 0.001f;
					cpbuf.Intensity = 1.0f;

					/*
					cpbuf.ReconstructInfo1[0] = c->GetZNear_FR() * c->GetZFar_FR();
					cpbuf.ReconstructInfo1[1] = c->GetZFar_FR() - c->GetZNear_FR();
					cpbuf.ReconstructInfo1[2] = -c->GetZFar_FR();
					*/
					cpbuf.ReconstructInfo1[0] = c->GetZFar_FR() - c->GetZNear_FR();
					cpbuf.ReconstructInfo1[1] = c->GetZNear_FR();

					cpbuf.ReconstructInfo2[0] = 1.0f / xScale;
					cpbuf.ReconstructInfo2[1] = 1.0f / yScale;

					g->SetVertexBuffer(m_ssaoVertexBuffer.get());
					g->SetIndexBuffer(m_ssaoIndexBuffer.get());
					g->SetShader(m_ssaoShader.get());

					auto& state = g->GetRenderState()->Push();
					state.DepthTest = false;
					state.DepthWrite = false;
					state.CullingType = CULLING_DOUBLE;
					state.TextureFilterTypes[0] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
					g->GetRenderState()->Update(false);

					g->DrawPolygon(2);

					g->GetRenderState()->Pop();
				}

				{
					g->SetRenderTarget(c->GetRenderTargetSSAO_Temp_FR(), nullptr);
					g->Clear(true, false, ace::Color(0, 0, 0, 255));

					m_ssaoBlurXShader->SetTexture("g_texture", c->GetRenderTargetSSAO_FR(), 0);

					g->SetVertexBuffer(m_ssaoVertexBuffer.get());
					g->SetIndexBuffer(m_ssaoIndexBuffer.get());
					g->SetShader(m_ssaoBlurXShader.get());

					auto& state = g->GetRenderState()->Push();
					state.DepthTest = false;
					state.DepthWrite = false;
					state.CullingType = CULLING_DOUBLE;
					state.TextureFilterTypes[0] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
					g->GetRenderState()->Update(false);

					g->DrawPolygon(2);

					g->GetRenderState()->Pop();
				}

				{
					g->SetRenderTarget(c->GetRenderTargetSSAO_FR(), nullptr);
					g->Clear(true, false, ace::Color(0, 0, 0, 255));

					m_ssaoBlurYShader->SetTexture("g_texture", c->GetRenderTargetSSAO_Temp_FR(), 0);

					g->SetVertexBuffer(m_ssaoVertexBuffer.get());
					g->SetIndexBuffer(m_ssaoIndexBuffer.get());
					g->SetShader(m_ssaoBlurYShader.get());

					auto& state = g->GetRenderState()->Push();
					state.DepthTest = false;
					state.DepthWrite = false;
					state.CullingType = CULLING_DOUBLE;
					state.TextureFilterTypes[0] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
					g->GetRenderState()->Update(false);

					g->DrawPolygon(2);

					g->GetRenderState()->Pop();
				}

				prop.SSAOPtr = c->GetRenderTargetSSAO_FR();
			}
			else
			{
				prop.SSAOPtr = nullptr;
			}

			// 3D描画
			{
				g->SetRenderTarget(c->GetRenderTarget_FR(), c->GetDepthBuffer_FR());
				g->Clear(true, false, ace::Color(0, 0, 0, 255));

				for (auto& o : m_objects)
				{
					o->Rendering(prop);
				}
			}

			// エフェクトの描画
			{
				// 行列を転置して設定
				Effekseer::Matrix44 cameraMat, projMat;
				for (auto c_ = 0; c_ < 4; c_++)
				{
					for (auto r = 0; r < 4; r++)
					{
						cameraMat.Values[c_][r] = c->GetCameraMatrix_FR().Values[r][c_];
						projMat.Values[c_][r] = c->GetProjectionMatrix_FR().Values[r][c_];
					}
				}
				rendering.EffectRenderer->SetCameraMatrix(cameraMat);
				rendering.EffectRenderer->SetProjectionMatrix(projMat);
				rendering.EffectRenderer->BeginRendering();
				rendering.EffectManager->Draw();
				rendering.EffectRenderer->EndRendering();

				// レンダー設定リセット
				g->GetRenderState()->Update(true);
			}

			c->ApplyPostEffects_FR();
		}

		for (auto& co : rendering.cameraObjects)
		{
			auto c = (RenderedCameraObject3D*) co;
			g->SetRenderTarget(GetRenderTarget(), nullptr);

			// 定数バッファを設定
			auto& cbuf = m_pasteShader->GetVertexConstantBuffer<PasteConstantBuffer>();
			cbuf.Size[0] = m_windowSize.X;
			cbuf.Size[1] = m_windowSize.Y;

			// 頂点情報をビデオメモリに転送
			if (!m_pasteVertexBuffer->RingBufferLock(6))
			{
				assert(0);
			}

			auto buf = m_pasteVertexBuffer->GetBuffer <PasteVertex>(6);

			buf[0].Position = Vector3DF(0, 0, 0.5f);
			buf[0].UV = Vector2DF(0, 0);
			buf[1].Position = Vector3DF(m_windowSize.X, 0, 0.5f);
			buf[1].UV = Vector2DF(1, 0);
			buf[2].Position = Vector3DF(m_windowSize.X, m_windowSize.Y, 0.5f);
			buf[2].UV = Vector2DF(1, 1);


			buf[3].Position = Vector3DF(0, m_windowSize.Y, 0.5f);
			buf[3].UV = Vector2DF(0, 1);
			buf[4] = buf[0];
			buf[5] = buf[2];

			m_pasteVertexBuffer->Unlock();

			m_pasteShader->SetTexture("g_texture", c->GetAffectedRenderTarget_FR(), 0);
			
			//m_pasteShader->SetTexture("g_texture", c->GetRenderTargetSSAO_FR(), 0);
			//m_pasteShader->SetTexture("g_texture", c->GetRenderTargetSSAO_Temp_FR(), 0);

			
			m_graphics->SetVertexBuffer(m_pasteVertexBuffer.get());
			m_graphics->SetIndexBuffer(m_pasteIndexBuffer.get());
			m_graphics->SetShader(m_pasteShader.get());

			auto& state = m_graphics->GetRenderState()->Push();
			state.DepthTest = false;
			state.DepthWrite = false;
			state.CullingType = ace::eCullingType::CULLING_DOUBLE;
			state.TextureWrapTypes[0] = ace::eTextureWrapType::TEXTURE_WRAP_CLAMP;
			m_graphics->GetRenderState()->Update(false);

			m_graphics->DrawPolygon(2);

			m_graphics->GetRenderState()->Pop();
		}
	}

	Renderer3D::Renderer3D(Graphics* graphics)
		: m_graphics(nullptr)
		, m_multithreadingMode(false)
		, m_renderTarget(nullptr)
		, m_event(this)
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);

		// 別スレッドで描画を行うか指定
		m_multithreadingMode = m_graphics->IsMultithreadingMode();

		// ペースト用シェーダー
		{
			m_pasteVertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(PasteVertex), 2 * 3, true);
			m_pasteIndexBuffer = m_graphics->CreateIndexBuffer_Imp(2 * 3, false, false);

			m_pasteIndexBuffer->Lock();
			auto ib = m_pasteIndexBuffer->GetBuffer<uint16_t>(2 * 3);

			for (int32_t i = 0; i < 2; i++)
			{
				ib[i * 3 + 0] = 0 + i * 3;
				ib[i * 3 + 1] = 1 + i * 3;
				ib[i * 3 + 2] = 2 + i * 3;
			}

			m_pasteIndexBuffer->Unlock();

			std::vector<ace::VertexLayout> vl;
			vl.push_back(ace::VertexLayout("Pos", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));

			std::vector<ace::Macro> macro;
			if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
			{
				m_pasteShader = m_graphics->CreateShader_Imp(
					paste_gl_vs,
					"vs",
					paste_gl_ps,
					"ps",
					vl,
					macro);
			}
			else
			{
				m_pasteShader = m_graphics->CreateShader_Imp(
					paste_dx_vs,
					"vs",
					paste_dx_ps,
					"ps",
					vl,
					macro);
			}

			std::vector<ace::ConstantBufferInformation> constantBuffers;
			constantBuffers.resize(1);
			constantBuffers[0].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
			constantBuffers[0].Name = std::string("Size");
			constantBuffers[0].Offset = 0;

			m_pasteShader->CreateVertexConstantBuffer<PasteConstantBuffer>(constantBuffers);
		}

		// シャドー用シェーダー
		{
			m_shadowVertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(ShadowVertex), 2 * 3, true);
			m_shadowIndexBuffer = m_graphics->CreateIndexBuffer_Imp(2 * 3, false, false);

			m_shadowIndexBuffer->Lock();
			auto ib = m_shadowIndexBuffer->GetBuffer<uint16_t>(2 * 3);

			for (int32_t i = 0; i < 2; i++)
			{
				ib[i * 3 + 0] = 0 + i * 3;
				ib[i * 3 + 1] = 1 + i * 3;
				ib[i * 3 + 2] = 2 + i * 3;
			}

			m_shadowIndexBuffer->Unlock();

			std::vector<ace::VertexLayout> vl;
			vl.push_back(ace::VertexLayout("Pos", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));
			vl.push_back(ace::VertexLayout("Color", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));

			std::vector<ace::Macro> macro;
			if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
			{
				m_shadowShaderX = m_graphics->CreateShader_Imp(
					shadowBlur_gl_vs,
					"vs",
					shadowBlur_gl_ps_x,
					"ps",
					vl,
					macro);

				m_shadowShaderY = m_graphics->CreateShader_Imp(
					shadowBlur_gl_vs,
					"vs",
					shadowBlur_gl_ps_y,
					"ps",
					vl,
					macro);
			}
			else
			{
				m_shadowShaderX = m_graphics->CreateShader_Imp(
					shadowBlur_dx_vs,
					"vs",
					shadowBlur_dx_ps_x,
					"ps",
					vl,
					macro);

				m_shadowShaderY = m_graphics->CreateShader_Imp(
					shadowBlur_dx_vs,
					"vs",
					shadowBlur_dx_ps_y,
					"ps",
					vl,
					macro);
			}

			std::vector<ace::ConstantBufferInformation> constantBuffers;
			constantBuffers.resize(1);
			constantBuffers[0].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
			constantBuffers[0].Name = std::string("g_weight");
			constantBuffers[0].Offset = 0;

			m_shadowShaderX->CreatePixelConstantBuffer<ShadowConstantBuffer>(constantBuffers);
			m_shadowShaderY->CreatePixelConstantBuffer<ShadowConstantBuffer>(constantBuffers);

			m_shadowTempTexture = m_graphics->CreateRenderTexture(2048, 2048, ace::eTextureFormat::TEXTURE_FORMAT_GL_R16G16_FLOAT);

			m_shadowVertexBuffer->Lock();
			auto buf = m_shadowVertexBuffer->GetBuffer <ShadowVertex>(6);

			buf[0].Position = Vector3DF(-1.0f, -1.0f, 0.5f);
			buf[0].UV = Vector2DF(0, 0);
			buf[1].Position = Vector3DF(1.0f, -1.0f, 0.5f);
			buf[1].UV = Vector2DF(1, 0);
			buf[2].Position = Vector3DF(1.0f, 1.0f, 0.5f);
			buf[2].UV = Vector2DF(1, 1);
			buf[3].Position = Vector3DF(-1.0f, 1.0f, 0.5f);
			buf[3].UV = Vector2DF(0, 1);
			buf[4] = buf[0];
			buf[5] = buf[2];

			m_shadowVertexBuffer->Unlock();
		}

		// SSAO用シェーダー
		{
			m_ssaoVertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(SSAOVertex), 2 * 3, true);
			m_ssaoIndexBuffer = m_graphics->CreateIndexBuffer_Imp(2 * 3, false, false);

			m_ssaoIndexBuffer->Lock();
			auto ib = m_ssaoIndexBuffer->GetBuffer<uint16_t>(2 * 3);

			for (int32_t i = 0; i < 2; i++)
			{
				ib[i * 3 + 0] = 0 + i * 3;
				ib[i * 3 + 1] = 1 + i * 3;
				ib[i * 3 + 2] = 2 + i * 3;
			}

			m_ssaoIndexBuffer->Unlock();

			m_ssaoVertexBuffer->Lock();
			auto buf = m_ssaoVertexBuffer->GetBuffer <SSAOVertex>(6);

			buf[0].Position = Vector3DF(-1.0f, -1.0f, 0.5f);
			buf[0].UV = Vector2DF(0, 0);
			buf[1].Position = Vector3DF(1.0f, -1.0f, 0.5f);
			buf[1].UV = Vector2DF(1, 0);
			buf[2].Position = Vector3DF(1.0f, 1.0f, 0.5f);
			buf[2].UV = Vector2DF(1, 1.0);
			buf[3].Position = Vector3DF(-1.0f, 1.0f, 0.5f);
			buf[3].UV = Vector2DF(0, 1.0);
			buf[4] = buf[0];
			buf[5] = buf[2];

			m_ssaoVertexBuffer->Unlock();

			std::vector<ace::VertexLayout> vl;
			vl.push_back(ace::VertexLayout("Pos", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));

			std::vector<ace::Macro> macro;
			if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
			{
				/*
				m_pasteShader = m_graphics->CreateShader_Imp(
					paste_gl_vs,
					"vs",
					paste_gl_ps,
					"ps",
					vl,
					macro);
				*/
			}
			else
			{
				m_ssaoShader = m_graphics->CreateShader_Imp(
					ssao_dx_vs,
					"vs",
					ssao_dx_ps,
					"ps",
					vl,
					macro);

				const char* BLUR_X = "BLUR_X";
				const char* BLUR_Y = "BLUR_Y";
				const char* ONE = "1";

				macro.push_back(Macro(BLUR_X, ONE));
				m_ssaoBlurXShader = m_graphics->CreateShader_Imp(
					ssao_dx_vs,
					"vs",
					ssao_blur_dx_ps,
					"ps",
					vl,
					macro);

				macro.clear();
				macro.push_back(Macro(BLUR_Y, ONE));
				m_ssaoBlurYShader = m_graphics->CreateShader_Imp(
					ssao_dx_vs,
					"vs",
					ssao_blur_dx_ps,
					"ps",
					vl,
					macro);
			}

			std::vector<ace::ConstantBufferInformation> constantVBuffers;
			constantVBuffers.resize(1);
			constantVBuffers[0].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
			constantVBuffers[0].Name = std::string("Size");
			constantVBuffers[0].Offset = 0;

			std::vector<ace::ConstantBufferInformation> constantPBuffers;
			constantPBuffers.resize(6);
			constantPBuffers[0].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT1;
			constantPBuffers[0].Name = std::string("Radius");
			constantPBuffers[0].Offset = 0;

			constantPBuffers[1].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT1;
			constantPBuffers[1].Name = std::string("ProjScale");
			constantPBuffers[1].Offset = sizeof(float) * 4;

			constantPBuffers[2].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT1;
			constantPBuffers[2].Name = std::string("Bias");
			constantPBuffers[2].Offset = sizeof(float) * 8;

			constantPBuffers[3].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT1;
			constantPBuffers[3].Name = std::string("Intensity");
			constantPBuffers[3].Offset = sizeof(float) * 12;

			constantPBuffers[4].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantPBuffers[4].Name = std::string("ReconstructInfo1");
			constantPBuffers[4].Offset = sizeof(float) * 16;

			constantPBuffers[5].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
			constantPBuffers[5].Name = std::string("ReconstructInfo2");
			constantPBuffers[5].Offset = sizeof(float) * 20;

			if (m_ssaoShader != nullptr)
			{
				m_ssaoShader->CreateVertexConstantBuffer<SSAOConstantVertexBuffer>(constantVBuffers);
				m_ssaoShader->CreatePixelConstantBuffer<SSAOConstantPixelBuffer>(constantPBuffers);
			}
		}

		// エフェクト
		{
			m_effectManager = ::Effekseer::Manager::Create(2000, false);
			if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_DX11)
			{
#if _WIN32
				auto g = (Graphics_Imp_DX11*) m_graphics;
				m_effectRenderer = ::EffekseerRendererDX11::Renderer::Create(g->GetDevice(), g->GetContext(), 2000);
#endif
			}
			else if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
			{
				m_effectRenderer = ::EffekseerRendererGL::Renderer::Create(2000);
			}

			m_effectManager->SetSpriteRenderer(m_effectRenderer->CreateSpriteRenderer());
			m_effectManager->SetRibbonRenderer(m_effectRenderer->CreateRibbonRenderer());
			m_effectManager->SetRingRenderer(m_effectRenderer->CreateRingRenderer());
			m_effectManager->SetModelRenderer(m_effectRenderer->CreateModelRenderer());
			m_effectManager->SetTrackRenderer(m_effectRenderer->CreateTrackRenderer());

			m_effectManager->SetCoordinateSystem(::Effekseer::COORDINATE_SYSTEM_RH);
		}
	}

	Renderer3D::~Renderer3D()
	{
		if (m_multithreadingMode)
		{
			while (!m_event.IsExited())
			{
				Sleep(1);
			}
		}

		CallRemovingObjects(m_objects, this);
		ReleaseObjects(m_objects);
		ReleaseObjects(rendering.objects);

		CallRemovingObjects(m_cameraObjects, this);
		ReleaseObjects(m_cameraObjects);
		ReleaseObjects(rendering.cameraObjects);

		CallRemovingObjects(m_directionalLightObjects, this);
		ReleaseObjects(m_directionalLightObjects);
		ReleaseObjects(rendering.directionalLightObjects);

		SafeRelease(m_renderTarget);

		m_effectRenderer->Destory();
		m_effectManager->Destroy();
		m_effectRenderer = nullptr;
		m_effectManager = nullptr;

		SafeRelease(m_graphics);
	}

	void Renderer3D::SetWindowSize(Vector2DI windowSize)
	{
		SafeRelease(m_renderTarget);
		m_renderTarget = m_graphics->CreateRenderTexture2D_Imp(windowSize.X, windowSize.Y, eTextureFormat::TEXTURE_FORMAT_R8G8B8A8_UNORM);
		m_windowSize = windowSize;

		if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_DX11)
		{
			m_effectRenderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH(90.0f / 180.0f * 3.14f, windowSize.X / windowSize.Y, 1.0f, 50.0f));
		}
		else if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
		{
			m_effectRenderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH_OpenGL(90.0f / 180.0f * 3.14f, windowSize.X / windowSize.Y, 1.0f, 50.0f));
		}
	}


	void Renderer3D::AddObject(RenderedObject3D* o)
	{
		if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_CAMERA)
		{
			if (m_cameraObjects.count(o) == 0)
			{
				SafeAddRef(o);
				m_cameraObjects.insert(o);
				o->OnAdded(this);
			}
		}
		else if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT)
		{
			if (m_directionalLightObjects.count(o) == 0)
			{
				SafeAddRef(o);
				m_directionalLightObjects.insert(o);
				o->OnAdded(this);
			}
		}
		else
		{
			if (m_objects.count(o) == 0)
			{
				SafeAddRef(o);
				m_objects.insert(o);
				o->OnAdded(this);
			}
		}
	}

	void Renderer3D::RemoveObject(RenderedObject3D* o)
	{
		if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_CAMERA)
		{
			if (m_cameraObjects.count(o) > 0)
			{
				o->OnRemoving(this);
				m_cameraObjects.erase(o);
				SafeRelease(o);
			}
		}
		else if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT)
		{
			if (m_directionalLightObjects.count(o) > 0)
			{
				o->OnRemoving(this);
				m_directionalLightObjects.erase(o);
				SafeRelease(o);
			}
		}
		else
		{
			if (m_objects.count(o) > 0)
			{
				o->OnRemoving(this);
				m_objects.erase(o);
				SafeRelease(o);
			}
		}
	}

	void Renderer3D::Flip()
	{
		ReleaseObjects(rendering.objects);
		ReleaseObjects(rendering.cameraObjects);
		ReleaseObjects(rendering.directionalLightObjects);

		rendering.objects.insert(m_objects.begin(), m_objects.end());
		rendering.cameraObjects.insert(m_cameraObjects.begin(), m_cameraObjects.end());
		rendering.directionalLightObjects.insert(m_directionalLightObjects.begin(), m_directionalLightObjects.end());
		rendering.EffectManager = m_effectManager;
		rendering.EffectRenderer = m_effectRenderer;

		AddRefToObjects(rendering.objects);
		AddRefToObjects(rendering.cameraObjects);
		AddRefToObjects(rendering.directionalLightObjects);

		m_effectManager->Flip();

		for (auto& o : rendering.objects)
		{
			o->Flip();
		}

		for (auto& o : rendering.cameraObjects)
		{
			o->Flip();
		}

		for (auto& o : rendering.directionalLightObjects)
		{
			o->Flip();
		}
	}

	void Renderer3D::BeginRendering()
	{
		assert(m_renderTarget != nullptr);

		// ここで命令を終了させないとフリーズする環境がある
		m_graphics->FlushCommand();

		if (m_multithreadingMode)
		{
			m_graphics->GetRenderingThread()->AddEvent(&m_event);
		}
		else
		{
			Rendering();
		}
	}

	void Renderer3D::EndRendering()
	{
		if (m_multithreadingMode)
		{
			while (!m_event.IsExited())
			{
				Sleep(1);
			}
		}

		m_graphics->MakeContextCurrent();
		m_graphics->FlushCommand();
	}

	RenderTexture2D_Imp* Renderer3D::GetRenderTarget()
	{
		return m_renderTarget;
	}
}
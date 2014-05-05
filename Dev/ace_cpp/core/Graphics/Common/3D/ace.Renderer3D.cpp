
#include "ace.Renderer3D.h"
#include "ace.RenderedObject3D.h"
#include "ace.RenderedCameraObject3D.h"
#include "ace.RenderedDirectionalLightObject3D.h"
#include "../ace.Graphics_Imp.h"

#include "../Resource/ace.ShaderCache.h"
#include "../Resource/ace.NativeShader_Imp.h"
#include "../Resource/ace.VertexBuffer_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"
#include "../Resource/ace.RenderState_Imp.h"


#include "../Shader/DX/Screen/Screen_VS.h"
#include "../Shader/DX/Screen/Paste_PS.h"
#include "../Shader/GL/Screen/Screen_VS.h"
#include "../Shader/GL/Screen/Paste_PS.h"

#include "../Shader/DX/Screen/Blur_PS.h"
#include "../Shader/GL/Screen/Blur_PS.h"

#include "../Shader/DX/Screen/SSAO_PS.h"
#include "../Shader/DX/Screen/SSAO_Blur_PS.h"

#include "../Shader/DX/Screen/DirectionalLight_Shadow_PS.h"
#include "../Shader/GL/Screen/DirectionalLight_Shadow_PS.h"

#if _WIN32
#include "../../DX11/ace.Graphics_Imp_DX11.h"
#endif
#include "../../GL/ace.Graphics_Imp_GL.h"

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
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
					ShadowBlurConstantBuffer& cbufX = m_shadowShaderX->GetPixelConstantBuffer<ShadowBlurConstantBuffer>();
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
					ShadowBlurConstantBuffer& cbufY = m_shadowShaderY->GetPixelConstantBuffer<ShadowBlurConstantBuffer>();
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

			// 影マップ描画
			{
				auto light = (RenderedDirectionalLightObject3D*) (*(rendering.directionalLightObjects.begin()));
				Matrix44 view, proj;

				light->CalcShadowMatrix(
					c->GetPosition_FR(),
					c->GetFocus_FR() - c->GetPosition_FR(),
					cameraProjMat,
					c->GetZNear_FR(),
					c->GetZFar_FR(),
					view,
					proj);

				g->SetRenderTarget(c->GetRenderTargetShadow_FR(), nullptr);
				g->Clear(true, false, ace::Color(0, 0, 0, 255));
			
				m_shadowShader->SetTexture("g_gbuffer2Texture", c->GetRenderTargetDepth_FR(), 2);
				m_shadowShader->SetTexture("g_shadowmapTexture", light->GetShadowTexture_FR(), 4);

				ShadowConstantBuffer& cbuf = m_shadowShader->GetPixelConstantBuffer<ShadowConstantBuffer>();
				
				auto invCameraMat = (prop.CameraMatrix).GetInverted();

				auto fov = c->GetFieldOfView() / 180.0f * 3.141592f;
				auto aspect = (float) c->GetWindowSize().X / (float) c->GetWindowSize().Y;

				// DirectX
				float yScale = 1 / tanf(fov / 2);
				float xScale = yScale / aspect;

				cbuf.CameraPositionToShadowCameraPosition = (view) * invCameraMat;
				cbuf.ShadowProjection = proj;
				cbuf.ReconstructInfo1[0] = c->GetZFar_FR() - c->GetZNear_FR();
				cbuf.ReconstructInfo1[1] = c->GetZNear_FR();

				cbuf.ReconstructInfo2[0] = 1.0f / xScale;
				cbuf.ReconstructInfo2[1] = 1.0f / yScale;

				g->SetVertexBuffer(m_shadowVertexBuffer.get());
				g->SetIndexBuffer(m_shadowIndexBuffer.get());
				g->SetShader(m_shadowShader.get());

				auto& state = g->GetRenderState()->Push();
				state.DepthTest = false;
				state.DepthWrite = false;
				state.CullingType = CULLING_DOUBLE;
				state.AlphaBlend = eAlphaBlend::ALPHA_BLEND_OPACITY;
				state.TextureFilterTypes[2] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
				state.TextureFilterTypes[4] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
				g->GetRenderState()->Update(false);

				g->DrawPolygon(2);

				g->GetRenderState()->Pop();

				prop.ShadowMapPtr = c->GetRenderTargetShadow_FR();
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

			// 頂点情報をビデオメモリに転送
			if (!m_pasteVertexBuffer->RingBufferLock(6))
			{
				assert(0);
			}

			auto buf = m_pasteVertexBuffer->GetBuffer <PasteVertex>(6);

			buf[0].Position = Vector3DF(-1.0f, -1.0f, 0.5f);
			buf[0].UV = Vector2DF(0, 1);
			buf[1].Position = Vector3DF(1.0f, -1.0f, 0.5f);
			buf[1].UV = Vector2DF(1, 1);
			buf[2].Position = Vector3DF(1.0f, 1.0f, 0.5f);
			buf[2].UV = Vector2DF(1, 0);
			buf[3].Position = Vector3DF(-1.0f, 1.0f, 0.5f);
			buf[3].UV = Vector2DF(0, 0);
			buf[4] = buf[0];
			buf[5] = buf[2];

			m_pasteVertexBuffer->Unlock();

			m_pasteShader->SetTexture("g_texture", c->GetAffectedRenderTarget_FR(), 0);
			
			//m_pasteShader->SetTexture("g_texture", c->GetRenderTargetSSAO_FR(), 0);
			//m_pasteShader->SetTexture("g_texture", c->GetRenderTargetSSAO_Temp_FR(), 0);
			//m_pasteShader->SetTexture("g_texture", c->GetRenderTargetShadow_FR(), 0);

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

	Renderer3D::Renderer3D(Graphics* graphics, RenderSettings settings)
		: m_graphics(nullptr)
		, m_settings(settings)
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
			vl.push_back(ace::VertexLayout("Position", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));

			std::vector<ace::Macro> macro;
			if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
			{
				m_pasteShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString("Internal.Paste").c_str(),
					screen_vs_gl,
					paste_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_pasteShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString("Internal.Paste").c_str(),
					screen_vs_dx,
					paste_ps_dx,
					vl,
					macro);
			}
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
			vl.push_back(ace::VertexLayout("Position", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));
			
			std::vector<ace::Macro> macro_x;
			macro_x.push_back(Macro("BLUR_X", "1"));

			std::vector<ace::Macro> macro_y;
			macro_y.push_back(Macro("BLUR_Y", "1"));

			if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
			{
				m_shadowShaderX = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurX").c_str(),
					screen_vs_gl,
					blur_ps_gl,
					vl,
					macro_x);

				m_shadowShaderY = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurY").c_str(),
					screen_vs_gl,
					blur_ps_gl,
					vl,
					macro_y);

				m_shadowShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.Shadow").c_str(),
					screen_vs_gl,
					directonalLight_Shadow_ps_gl,
					vl,
					macro_y);
			}
			else
			{
				m_shadowShaderX = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurX").c_str(),
					screen_vs_dx,
					blur_ps_dx,
					vl,
					macro_x);

				m_shadowShaderY = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurY").c_str(),
					screen_vs_dx,
					blur_ps_dx,
					vl,
					macro_y);

				m_shadowShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.Shadow").c_str(),
					screen_vs_dx,
					directonalLight_Shadow_ps_dx,
					vl,
					macro_y);
			}

			std::vector<ace::ConstantBufferInformation> constantBuffers;
			constantBuffers.resize(1);
			constantBuffers[0].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
			constantBuffers[0].Name = std::string("g_weight");
			constantBuffers[0].Offset = 0;

			m_shadowShaderX->CreatePixelConstantBuffer<ShadowBlurConstantBuffer>(constantBuffers);
			m_shadowShaderY->CreatePixelConstantBuffer<ShadowBlurConstantBuffer>(constantBuffers);


			std::vector<ace::ConstantBufferInformation> constantBuffers2;
			constantBuffers2.resize(4);
			constantBuffers2[0].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_MATRIX44;
			constantBuffers2[0].Name = std::string("g_cameraPositionToShadowCameraPosition");
			constantBuffers2[0].Offset = 0;

			constantBuffers2[1].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_MATRIX44;
			constantBuffers2[1].Name = std::string("g_shadowProjection");
			constantBuffers2[1].Offset = sizeof(Matrix44) * 1;

			constantBuffers2[2].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers2[2].Name = std::string("reconstructInfo1");
			constantBuffers2[2].Offset = sizeof(Matrix44) * 2;

			constantBuffers2[3].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
			constantBuffers2[3].Name = std::string("reconstructInfo2");
			constantBuffers2[3].Offset = sizeof(Matrix44) * 2 + sizeof(Vector4DF) * 1;

			m_shadowShader->CreatePixelConstantBuffer<ShadowConstantBuffer>(constantBuffers2);

			m_shadowTempTexture = m_graphics->CreateRenderTexture(2048, 2048, ace::eTextureFormat::TEXTURE_FORMAT_GL_R16G16_FLOAT);

			m_shadowVertexBuffer->Lock();
			auto buf = m_shadowVertexBuffer->GetBuffer <ShadowVertex>(6);

			buf[0].Position = Vector3DF(-1.0f, 1.0f, 0.5f);
			buf[0].UV = Vector2DF(0, 0);
			buf[1].Position = Vector3DF(1.0f, 1.0f, 0.5f);
			buf[1].UV = Vector2DF(1, 0);
			buf[2].Position = Vector3DF(1.0f, -1.0f, 0.5f);
			buf[2].UV = Vector2DF(1, 1);
			buf[3].Position = Vector3DF(-1.0f, -1.0f, 0.5f);
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

			buf[0].Position = Vector3DF(-1.0f, 1.0f, 0.5f);
			buf[0].UV = Vector2DF(0, 0);
			buf[1].Position = Vector3DF(1.0f, 1.0f, 0.5f);
			buf[1].UV = Vector2DF(1, 0);
			buf[2].Position = Vector3DF(1.0f, -1.0f, 0.5f);
			buf[2].UV = Vector2DF(1, 1.0);
			buf[3].Position = Vector3DF(-1.0f, -1.0f, 0.5f);
			buf[3].UV = Vector2DF(0, 1.0);
			buf[4] = buf[0];
			buf[5] = buf[2];

			m_ssaoVertexBuffer->Unlock();

			std::vector<ace::VertexLayout> vl;
			vl.push_back(ace::VertexLayout("Position", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
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
				m_ssaoShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.SSAO").c_str(),
					screen_vs_dx,
					ssao_ps_dx,
					vl,
					macro);

				const char* BLUR_X = "BLUR_X";
				const char* BLUR_Y = "BLUR_Y";
				const char* ONE = "1";

				macro.push_back(Macro(BLUR_X, ONE));
				m_ssaoBlurXShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.SSAO.BlurX").c_str(),
					screen_vs_dx,
					ssao_blur_ps_dx,
					vl,
					macro);

				macro.clear();
				macro.push_back(Macro(BLUR_Y, ONE));
				m_ssaoBlurYShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.SSAO.BlurY").c_str(),
					screen_vs_dx,
					ssao_blur_ps_dx,
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
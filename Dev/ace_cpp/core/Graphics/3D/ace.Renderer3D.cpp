
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
#include "../Resource/ace.DepthBuffer_Imp.h"

#include "../Shader/DX/3D/Screen_VS.h"
#include "../Shader/DX/3D/Paste_PS.h"
#include "../Shader/GL/3D/Screen_VS.h"
#include "../Shader/GL/3D/Paste_PS.h"

#include "../Shader/DX/3D/Blur_PS.h"
#include "../Shader/GL/3D/Blur_PS.h"

#include "../Shader/DX/3D/PostEffect/SSAO_PS.h"
#include "../Shader/DX/3D/PostEffect/SSAO_Blur_PS.h"

#include "../Shader/DX/3D/Light_PS.h"
#include "../Shader/GL/3D/Light_PS.h"

#include "../Shader/DX/3D/DeferredBuffer_PS.h"
#include "../Shader/GL/3D/DeferredBuffer_PS.h"

#if _WIN32
#include "../Platform/DX11/ace.Graphics_Imp_DX11.h"
#endif
#include "../Platform/GL/ace.Graphics_Imp_GL.h"

#include "../Shader/ace.Vertices.h"

#include <cstddef>

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
			o->CalculateMatrix_RT();
		}

		for (auto& o : rendering.cameraObjects)
		{
			o->CalculateMatrix_RT();
		}

		for (auto& o : rendering.directionalLightObjects)
		{
			o->CalculateMatrix_RT();
		}

		// エフェクトの更新 TODO 時間を計算するように
		rendering.EffectManager->Update(1.0f);

		RenderingProperty prop;
		prop.IsLightweightMode = rendering.Settings.IsLightweightMode;
		prop.IsDepthMode = false;
		
		// ライトの計算
		{
			if (rendering.directionalLightObjects.size() > 0)
			{
				auto light = (RenderedDirectionalLightObject3D*) (*(rendering.directionalLightObjects.begin()));
				prop.DirectionalLightColor = light->GetColor_RT();
				prop.DirectionalLightDirection = light->GetDirection_RT();
				prop.DirectionalLightDirection.X = -prop.DirectionalLightDirection.X;
				prop.DirectionalLightDirection.Y = -prop.DirectionalLightDirection.Y;
				prop.DirectionalLightDirection.Z = -prop.DirectionalLightDirection.Z;
			}
			else
			{
				prop.DirectionalLightColor = Color(0, 0, 0, 0);
				prop.DirectionalLightDirection = Vector3DF(1.0f, 1.0f, 1.0f);
			}
			prop.DirectionalLightDirection.Normalize();

			prop.GroundLightColor = rendering.GroundAmbientColor;
			prop.SkyLightColor = rendering.SkyAmbientColor;
		}

		for (auto& co : rendering.cameraObjects)
		{
			auto c = (RenderedCameraObject3D*) co;

			// カメラプロジェクション行列計算
			Matrix44 cameraProjMat;
			ace::Matrix44::Mul(cameraProjMat, c->GetProjectionMatrix_RT(), c->GetCameraMatrix_RT());
			prop.CameraMatrix = c->GetCameraMatrix_RT();
			prop.ProjectionMatrix = c->GetProjectionMatrix_RT();
			prop.DepthRange = c->GetZFar_RT() - c->GetZNear_RT();
			prop.ZFar = c->GetZFar_RT();
			prop.ZNear = c->GetZNear_RT();

			// 3D描画
			{
				if (prop.IsLightweightMode)
				{
					g->SetRenderTarget(c->GetRenderTarget_RT(), c->GetDepthBuffer_RT());
					g->Clear(true, true, ace::Color(0, 0, 0, 255));

					for (auto& o : m_objects)
					{
						o->Rendering(prop);
					}
				}
				else
				{
					// 奥行き描画
					{
						g->SetRenderTarget(c->GetRenderTargetDepth_RT(), c->GetDepthBuffer_RT());
						g->Clear(true, true, ace::Color(0, 0, 0, 255));
						prop.IsDepthMode = true;
						for (auto& o : m_objects)
						{
							o->Rendering(prop);
						}
					}

					// Gバッファ描画
					{
						g->SetRenderTarget(
							c->GetRenderTargetDiffuseColor_RT(),
							c->GetRenderTargetSpecularColor_Smoothness_RT(),
							c->GetRenderTargetDepth_RT(),
							c->GetRenderTargetAO_MatID_RT(),
							c->GetDepthBuffer_RT());
						g->Clear(true, false, ace::Color(0, 0, 0, 255));
						prop.IsDepthMode = false;
						for (auto& o : m_objects)
						{
							o->Rendering(prop);
						}
					}
				}
			}


			// SSAO
			if (!m_settings.IsLightweightMode && m_ssaoShader != nullptr)
			{
				{
					g->SetRenderTarget(c->GetRenderTargetSSAO_RT(), nullptr);
					g->Clear(true, false, ace::Color(0, 0, 0, 255));

					m_ssaoShader->SetTexture("g_texture", c->GetRenderTargetDepth_RT(), 0);

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
					cpbuf.ReconstructInfo1[0] = c->GetZNear_RT() * c->GetZFar_RT();
					cpbuf.ReconstructInfo1[1] = c->GetZFar_RT() - c->GetZNear_RT();
					cpbuf.ReconstructInfo1[2] = -c->GetZFar_RT();
					*/
					cpbuf.ReconstructInfo1[0] = c->GetZFar_RT() - c->GetZNear_RT();
					cpbuf.ReconstructInfo1[1] = c->GetZNear_RT();

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
					g->SetRenderTarget(c->GetRenderTargetSSAO_Temp_RT(), nullptr);
					g->Clear(true, false, ace::Color(0, 0, 0, 255));

					m_ssaoBlurXShader->SetTexture("g_texture", c->GetRenderTargetSSAO_RT(), 0);

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
					g->SetRenderTarget(c->GetRenderTargetSSAO_RT(), nullptr);
					g->Clear(true, false, ace::Color(0, 0, 0, 255));

					m_ssaoBlurYShader->SetTexture("g_texture", c->GetRenderTargetSSAO_Temp_RT(), 0);

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
			}
			
			// 蓄積リセット
			if (!m_settings.IsLightweightMode)
			{
				g->SetRenderTarget(c->GetRenderTarget_RT(), nullptr);
				g->Clear(true, false, ace::Color(0, 0, 0, 255));
			}

			// 直接光描画
			if (!m_settings.IsLightweightMode)
			{
				auto invCameraMat = (prop.CameraMatrix).GetInverted();

				auto fov = c->GetFieldOfView() / 180.0f * 3.141592f;
				auto aspect = (float) c->GetWindowSize().X / (float) c->GetWindowSize().Y;

				float yScale = 1 / tanf(fov / 2);
				float xScale = yScale / aspect;

				Vector4DF ReconstructInfo1;
				Vector4DF ReconstructInfo2;

				ReconstructInfo1.X = c->GetZFar_RT() - c->GetZNear_RT();
				ReconstructInfo1.Y = c->GetZNear_RT();
				ReconstructInfo2.X = 1.0f / xScale;
				ReconstructInfo2.Y = 1.0f / yScale;

				Vector3DF upDir = Vector3DF(0, 1, 0);
				Vector3DF zero;
				zero = prop.CameraMatrix.Transform3D(zero);
				upDir = prop.CameraMatrix.Transform3D(upDir) - zero;

				Vector3DF groundLightColor(
					prop.GroundLightColor.R / 255.0f,
					prop.GroundLightColor.G / 255.0f,
					prop.GroundLightColor.B / 255.0f);

				Vector3DF skyLightColor(
					prop.SkyLightColor.R / 255.0f,
					prop.SkyLightColor.G / 255.0f,
					prop.SkyLightColor.B / 255.0f);

				int32_t lightIndex = 0;
				for (auto& light : rendering.directionalLightObjects)
				{
					auto light = (RenderedDirectionalLightObject3D*) (*(rendering.directionalLightObjects.begin()));
					RenderTexture2D_Imp* shadowMap = light->GetShadowTexture_RT();

					Matrix44 view, proj;

					light->CalcShadowMatrix(
						c->GetPosition_RT(),
						c->GetFocus_RT() - c->GetPosition_RT(),
						cameraProjMat,
						c->GetZNear_RT(),
						c->GetZFar_RT(),
						view,
						proj);

					// 影マップ作成
					{
						g->SetRenderTarget(light->GetShadowTexture_RT(), light->GetShadowDepthBuffer_RT());
						g->Clear(true, true, ace::Color(0, 0, 0, 255));

						RenderingProperty shadowProp = prop;
						shadowProp.IsDepthMode = true;
						shadowProp.CameraMatrix = view;
						shadowProp.ProjectionMatrix = proj;
						shadowProp.DepthRange = prop.DepthRange;
						shadowProp.ZFar = prop.ZFar;
						shadowProp.ZNear = prop.ZNear;

						prop.LightCameraMatrix = shadowProp.CameraMatrix;
						prop.LightProjectionMatrix = shadowProp.ProjectionMatrix;

						for (auto& o : m_objects)
						{
							o->Rendering(shadowProp);
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

							m_shadowShaderX->SetTexture("g_texture", light->GetShadowTexture_RT(), 0);
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
							g->SetRenderTarget(light->GetShadowTexture_RT(), nullptr);
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

					// 光源描画
					{
						g->SetRenderTarget(c->GetRenderTarget_RT(), nullptr);

						std::shared_ptr<ace::NativeShader_Imp> shader;

						if (lightIndex == 0)
						{
							shader = m_directionalWithAmbientLightShader;
							shader->SetVector3DF("skyLightColor", skyLightColor);
							shader->SetVector3DF("groundLightColor", groundLightColor);
						}
						else
						{
							shader = m_directionalLightShader;
						}

						shader->SetTexture("g_gbuffer0Texture", c->GetRenderTargetDiffuseColor_RT(), 0);
						shader->SetTexture("g_gbuffer1Texture", c->GetRenderTargetSpecularColor_Smoothness_RT(), 1);
						shader->SetTexture("g_gbuffer2Texture", c->GetRenderTargetDepth_RT(), 2);
						shader->SetTexture("g_gbuffer3Texture", c->GetRenderTargetAO_MatID_RT(), 3);
						shader->SetTexture("g_shadowmapTexture", light->GetShadowTexture_RT(), 4);

						if (m_ssaoShader != nullptr)
						{
							shader->SetTexture("g_ssaoTexture", c->GetRenderTargetSSAO_RT(), 5);
						}
						else
						{
							shader->SetTexture("g_ssaoTexture", GetDummyTextureWhite().get(), 5);
						}

						auto CameraPositionToShadowCameraPosition = (view) * invCameraMat;
						shader->SetMatrix44("g_cameraPositionToShadowCameraPosition", CameraPositionToShadowCameraPosition);

						auto ShadowProjection = proj;
						shader->SetMatrix44("g_shadowProjection", ShadowProjection);

						shader->SetVector4DF("reconstructInfo1", ReconstructInfo1);
						shader->SetVector4DF("reconstructInfo2", ReconstructInfo2);

						Vector3DF directionalLightDirection;
						Vector3DF directionalLightColor;
						
						directionalLightDirection = prop.DirectionalLightDirection;
						directionalLightDirection = prop.CameraMatrix.Transform3D(directionalLightDirection) - zero;

						directionalLightColor.X = prop.DirectionalLightColor.R / 255.0f;
						directionalLightColor.Y = prop.DirectionalLightColor.G / 255.0f;
						directionalLightColor.Z = prop.DirectionalLightColor.B / 255.0f;

						shader->SetVector3DF("directionalLightDirection", directionalLightDirection);
						shader->SetVector3DF("directionalLightColor", directionalLightColor);
						shader->SetVector3DF("upDir", upDir);

						g->SetVertexBuffer(m_shadowVertexBuffer.get());
						g->SetIndexBuffer(m_shadowIndexBuffer.get());
						g->SetShader(shader.get());

						auto& state = g->GetRenderState()->Push();
						state.DepthTest = false;
						state.DepthWrite = false;
						state.CullingType = CULLING_DOUBLE;
						state.AlphaBlend = eAlphaBlend::ALPHA_BLEND_ADD;
						state.TextureFilterTypes[2] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
						state.TextureFilterTypes[4] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
						g->GetRenderState()->Update(false);

						g->DrawPolygon(2);

						g->GetRenderState()->Pop();
					}

					lightIndex++;
				}

				// 環境光
				if (lightIndex == 0)
				{
					g->SetRenderTarget(c->GetRenderTarget_RT(), nullptr);

					std::shared_ptr<ace::NativeShader_Imp> shader = m_ambientLightShader;

					shader->SetTexture("g_gbuffer0Texture", c->GetRenderTargetDiffuseColor_RT(), 0);
					shader->SetTexture("g_gbuffer1Texture", c->GetRenderTargetSpecularColor_Smoothness_RT(), 1);
					shader->SetTexture("g_gbuffer2Texture", c->GetRenderTargetDepth_RT(), 2);
					shader->SetTexture("g_gbuffer3Texture", c->GetRenderTargetAO_MatID_RT(), 3);

					if (m_ssaoShader != nullptr)
					{
						shader->SetTexture("g_ssaoTexture", c->GetRenderTargetSSAO_RT(), 5);
					}
					else
					{
						shader->SetTexture("g_ssaoTexture", GetDummyTextureWhite().get(), 5);
					}

					shader->SetVector3DF("skyLightColor", skyLightColor);
					shader->SetVector3DF("groundLightColor", groundLightColor);

					shader->SetVector4DF("reconstructInfo1", ReconstructInfo1);
					shader->SetVector4DF("reconstructInfo2", ReconstructInfo2);
					shader->SetVector3DF("upDir", upDir);

					g->SetVertexBuffer(m_shadowVertexBuffer.get());
					g->SetIndexBuffer(m_shadowIndexBuffer.get());
					g->SetShader(shader.get());

					auto& state = g->GetRenderState()->Push();
					state.DepthTest = false;
					state.DepthWrite = false;
					state.CullingType = CULLING_DOUBLE;
					state.AlphaBlend = eAlphaBlend::ALPHA_BLEND_ADD;
					state.TextureFilterTypes[2] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
					state.TextureFilterTypes[4] = eTextureFilterType::TEXTURE_FILTER_LINEAR;
					g->GetRenderState()->Update(false);

					g->DrawPolygon(2);

					g->GetRenderState()->Pop();
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
						cameraMat.Values[c_][r] = c->GetCameraMatrix_RT().Values[r][c_];
						projMat.Values[c_][r] = c->GetProjectionMatrix_RT().Values[r][c_];
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

			if (m_settings.IsLightweightMode || rendering.Settings.VisalizedBuffer == eVisalizedBuffer::VISALIZED_BUFFER_FINALIMAGE)
			{
				// ポストエフェクト適用
				c->ApplyPostEffects_RT();
			}
			else
			{
				g->SetRenderTarget(c->GetRenderTarget_RT(), nullptr);
				g->Clear(true, false, Color(0, 0, 0, 0));

				std::shared_ptr<ace::NativeShader_Imp> shader = m_deferredBufferShader;

				if (rendering.Settings.VisalizedBuffer == eVisalizedBuffer::VISALIZED_BUFFER_DIFFUSE)
				{
					shader->SetFloat("flag", 0.0f);
				}
				else if (rendering.Settings.VisalizedBuffer == eVisalizedBuffer::VISALIZED_BUFFER_NORMAL)
				{
					shader->SetFloat("flag", 1.0f);
				}

				shader->SetTexture("g_gbuffer0Texture", c->GetRenderTargetDiffuseColor_RT(), 0);
				shader->SetTexture("g_gbuffer1Texture", c->GetRenderTargetSpecularColor_Smoothness_RT(), 1);
				shader->SetTexture("g_gbuffer2Texture", c->GetRenderTargetDepth_RT(), 2);
				shader->SetTexture("g_gbuffer3Texture", c->GetRenderTargetAO_MatID_RT(), 3);

				if (m_ssaoShader != nullptr)
				{
					shader->SetTexture("g_ssaoTexture", c->GetRenderTargetSSAO_RT(), 5);
				}
				else
				{
					shader->SetTexture("g_ssaoTexture", GetDummyTextureWhite().get(), 5);
				}

				g->SetVertexBuffer(m_shadowVertexBuffer.get());
				g->SetIndexBuffer(m_shadowIndexBuffer.get());
				g->SetShader(shader.get());

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
			}
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

			auto buf = m_pasteVertexBuffer->GetBuffer <ScreenVertexLayout>(6);

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

			if (m_settings.IsLightweightMode || rendering.Settings.VisalizedBuffer == eVisalizedBuffer::VISALIZED_BUFFER_FINALIMAGE)
			{
				m_pasteShader->SetTexture("g_texture", c->GetAffectedRenderTarget_RT(), 0);
			}
			else
			{
				m_pasteShader->SetTexture("g_texture", c->GetRenderTarget_RT(), 0);
			}
			
			
			//m_pasteShader->SetTexture("g_texture", c->GetDepthBuffer_RT(), 0);
			//m_pasteShader->SetTexture("g_texture", c->GetRenderTargetDepth_RT(), 0);
			//m_pasteShader->SetTexture("g_texture", c->GetRenderTargetSSAO_RT(), 0);
			//m_pasteShader->SetTexture("g_texture", c->GetRenderTargetShadow_RT(), 0);

			m_graphics->SetVertexBuffer(m_pasteVertexBuffer.get());
			m_graphics->SetIndexBuffer(m_pasteIndexBuffer.get());
			m_graphics->SetShader(m_pasteShader.get());

			auto& state = m_graphics->GetRenderState()->Push();
			state.DepthTest = false;
			state.DepthWrite = false;
			state.AlphaBlend = eAlphaBlend::ALPHA_BLEND_OPACITY;
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
		, m_skyAmbientColor(Color(10,10,20,255))
		, m_groundAmbientColor(Color(10,10,10,255))
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);

		// 別スレッドで描画を行うか指定
		m_multithreadingMode = m_graphics->IsMultithreadingMode();

		// テクスチャ
		{
			m_dummyTextureWhite = graphics->CreateEmptyTexture2D(1, 1, eTextureFormat::TEXTURE_FORMAT_R8G8B8A8_UNORM);
			TextureLockInfomation info;
			if (m_dummyTextureWhite->Lock(info))
			{
				auto c = (Color*) info.Pixels;
				*c = Color(255, 255, 255, 255);
				m_dummyTextureWhite->Unlock();
			}
		}

		{
			m_dummyTextureBlack = graphics->CreateEmptyTexture2D(1, 1, eTextureFormat::TEXTURE_FORMAT_R8G8B8A8_UNORM);
			TextureLockInfomation info;
			if (m_dummyTextureBlack->Lock(info))
			{
				auto c = (Color*) info.Pixels;
				*c = Color(0, 0, 0, 255);
				m_dummyTextureBlack->Unlock();
			}
		}

		{
			m_dummyTextureNormal = graphics->CreateEmptyTexture2D(1, 1, eTextureFormat::TEXTURE_FORMAT_R8G8B8A8_UNORM);
			TextureLockInfomation info;
			if (m_dummyTextureNormal->Lock(info))
			{
				auto c = (Color*) info.Pixels;
				*c = Color(255/2, 255/2, 255, 255);
				m_dummyTextureNormal->Unlock();
			}
		}

		// ペースト用シェーダー
		{
			m_pasteVertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(ScreenVertexLayout), 2 * 3, true);
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
			m_shadowVertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(ScreenVertexLayout), 2 * 3, true);
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
			}

			std::vector<ace::ConstantBufferInformation> constantBuffers;
			constantBuffers.resize(1);
			constantBuffers[0].Format = ace::eConstantBufferFormat::CONSTANT_BUFFER_FORMAT_FLOAT4;
			constantBuffers[0].Name = std::string("g_weight");
			constantBuffers[0].Offset = 0;

			m_shadowShaderX->CreatePixelConstantBuffer<ShadowBlurConstantBuffer>(constantBuffers);
			m_shadowShaderY->CreatePixelConstantBuffer<ShadowBlurConstantBuffer>(constantBuffers);

			m_shadowTempTexture = m_graphics->CreateRenderTexture(2048, 2048, ace::eTextureFormat::TEXTURE_FORMAT_GL_R16G16_FLOAT);

			m_shadowVertexBuffer->Lock();
			auto buf = m_shadowVertexBuffer->GetBuffer <ScreenVertexLayout>(6);

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

		// ライト用シェーダ
		{
			std::vector<ace::VertexLayout> vl;
			vl.push_back(ace::VertexLayout("Position", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));

			std::vector<ace::Macro> macro_d_a;
			macro_d_a.push_back(Macro("DIRECTIONAL_LIGHT", "1"));
			macro_d_a.push_back(Macro("AMBIENT_LIGHT", "1"));

			std::vector<ace::Macro> macro_d;
			macro_d.push_back(Macro("DIRECTIONAL_LIGHT", "1"));

			std::vector<ace::Macro> macro_a;
			macro_a.push_back(Macro("AMBIENT_LIGHT", "1"));

			if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
			{
				m_directionalWithAmbientLightShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_A_Light").c_str(),
					screen_vs_gl,
					light_ps_gl,
					vl,
					macro_d_a);

				m_directionalLightShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_Light").c_str(),
					screen_vs_gl,
					light_ps_gl,
					vl,
					macro_d);

				m_ambientLightShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.A_Light").c_str(),
					screen_vs_gl,
					light_ps_gl,
					vl,
					macro_a);
			}
			else
			{
				m_directionalWithAmbientLightShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_A_Light").c_str(),
					screen_vs_dx,
					light_ps_dx,
					vl,
					macro_d_a);

				m_directionalLightShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_Light").c_str(),
					screen_vs_dx,
					light_ps_dx,
					vl,
					macro_d);

				m_ambientLightShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.A_Light").c_str(),
					screen_vs_dx,
					light_ps_dx,
					vl,
					macro_a);
			}
		}

		// 遅延バッファ用シェーダ
		{
			std::vector<ace::VertexLayout> vl;
			vl.push_back(ace::VertexLayout("Position", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));

			std::vector<ace::Macro> macro;

			if (m_graphics->GetGraphicsType() == eGraphicsType::GRAPHICS_TYPE_GL)
			{
				m_deferredBufferShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.DeferredBuffer").c_str(),
					screen_vs_gl,
					deferred_buffer_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_deferredBufferShader = m_graphics->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.DeferredBuffer").c_str(),
					screen_vs_dx,
					deferred_buffer_ps_dx,
					vl,
					macro);
			}
		}

		// SSAO用シェーダー
		{
			m_ssaoVertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(ScreenVertexLayout), 2 * 3, true);
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
			auto buf = m_ssaoVertexBuffer->GetBuffer <ScreenVertexLayout>(6);

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

			m_effectManager->SetSetting(m_graphics->GetEffectSetting());
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

	RenderSettings Renderer3D::GetRenderSettings() const
	{
		return m_settings;
	}

	void Renderer3D::SetRenderSettings(RenderSettings settings)
	{
		m_settings = settings;
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

		rendering.SkyAmbientColor = m_skyAmbientColor;
		rendering.GroundAmbientColor = m_groundAmbientColor;
		rendering.Settings = m_settings;
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
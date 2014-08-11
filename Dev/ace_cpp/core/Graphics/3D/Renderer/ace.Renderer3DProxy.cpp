
#include "ace.Renderer3DProxy.h"

#include "../../Command/ace.RenderingCommandFactory.h"
#include "../../Command/ace.RenderingCommandHelper.h"
#include "../../Command/ace.RenderingCommand.h"


#include "../Object/ace.RenderedCameraObject3D.h"
#include "../Object/ace.RenderedDirectionalLightObject3D.h"

#include "../../Resource/ace.ShaderCache.h"
#include "../../Resource/ace.NativeShader_Imp.h"
#include "../../Resource/ace.VertexBuffer_Imp.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"
#include "../../Resource/ace.DepthBuffer_Imp.h"

#include "../../Shader/DX/3D/Screen_VS.h"
#include "../../Shader/DX/3D/Paste_PS.h"
#include "../../Shader/GL/3D/Screen_VS.h"
#include "../../Shader/GL/3D/Paste_PS.h"

#include "../../Shader/DX/3D/Blur_PS.h"
#include "../../Shader/GL/3D/Blur_PS.h"

#include "../../Shader/DX/3D/Light_PS.h"
#include "../../Shader/GL/3D/Light_PS.h"

#include "../../Shader/DX/3D/DeferredBuffer_PS.h"
#include "../../Shader/GL/3D/DeferredBuffer_PS.h"

#if _WIN32
#include "../../Platform/DX11/ace.Graphics_Imp_DX11.h"
#endif
#include "../../Platform/GL/ace.Graphics_Imp_GL.h"

#include "../../Shader/ace.Vertices.h"

namespace ace
{

	template<typename T>
	void ReleaseObjects(std::set<T*>& os)
	{
		for (auto& o : os)
		{
			o->Release();
		}
		os.clear();
	}

	Renderer3DProxy::Renderer3DProxy(Graphics* graphics)
	{
		auto g = (Graphics_Imp*)graphics;

		// テクスチャ
		{
			dummyTextureWhite = g->CreateEmptyTexture2D(1, 1, TextureFormat::R8G8B8A8_UNORM);
			TextureLockInfomation info;
			if (dummyTextureWhite->Lock(info))
			{
				auto c = (Color*) info.Pixels;
				*c = Color(255, 255, 255, 255);
				dummyTextureWhite->Unlock();
			}
		}

		{
			dummyTextureBlack = g->CreateEmptyTexture2D(1, 1, TextureFormat::R8G8B8A8_UNORM);
			TextureLockInfomation info;
			if (dummyTextureBlack->Lock(info))
			{
				auto c = (Color*) info.Pixels;
				*c = Color(0, 0, 0, 255);
				dummyTextureBlack->Unlock();
			}
		}

		{
			dummyTextureNormal = g->CreateEmptyTexture2D(1, 1, TextureFormat::R8G8B8A8_UNORM);
			TextureLockInfomation info;
			if (dummyTextureNormal->Lock(info))
			{
				auto c = (Color*) info.Pixels;
				*c = Color(255 / 2, 255 / 2, 255, 255);
				dummyTextureNormal->Unlock();
			}
		}

		// ペースト用シェーダー
		{
			m_pasteVertexBuffer = g->CreateVertexBuffer_Imp(sizeof(ScreenVertexLayout), 2 * 3, false);

			{
				m_pasteVertexBuffer->Lock();
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
			}
			m_pasteIndexBuffer = g->CreateIndexBuffer_Imp(2 * 3, false, false);

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
			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_pasteShader = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.Paste").c_str(),
					screen_vs_gl,
					paste_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_pasteShader = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.Paste").c_str(),
					screen_vs_dx,
					paste_ps_dx,
					vl,
					macro);
			}
		}

		// シャドー用シェーダー
		{
			m_shadowVertexBuffer = g->CreateVertexBuffer_Imp(sizeof(ScreenVertexLayout), 2 * 3, true);
			m_shadowIndexBuffer = g->CreateIndexBuffer_Imp(2 * 3, false, false);

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

			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_shadowShaderX = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurX").c_str(),
					screen_vs_gl,
					blur_ps_gl,
					vl,
					macro_x);

				m_shadowShaderY = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurY").c_str(),
					screen_vs_gl,
					blur_ps_gl,
					vl,
					macro_y);
			}
			else
			{
				m_shadowShaderX = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurX").c_str(),
					screen_vs_dx,
					blur_ps_dx,
					vl,
					macro_x);

				m_shadowShaderY = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurY").c_str(),
					screen_vs_dx,
					blur_ps_dx,
					vl,
					macro_y);
			}

			m_shadowTempTexture = g->CreateRenderTexture2D(2048, 2048, ace::TextureFormat::R16G16_FLOAT);

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

			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_directionalWithAmbientLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_A_Light").c_str(),
					screen_vs_gl,
					light_ps_gl,
					vl,
					macro_d_a);

				m_directionalLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_Light").c_str(),
					screen_vs_gl,
					light_ps_gl,
					vl,
					macro_d);

				m_ambientLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.A_Light").c_str(),
					screen_vs_gl,
					light_ps_gl,
					vl,
					macro_a);
			}
			else
			{
				m_directionalWithAmbientLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_A_Light").c_str(),
					screen_vs_dx,
					light_ps_dx,
					vl,
					macro_d_a);

				m_directionalLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_Light").c_str(),
					screen_vs_dx,
					light_ps_dx,
					vl,
					macro_d);

				m_ambientLightShader = g->GetShaderCache()->CreateFromCode(
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

			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_deferredBufferShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.DeferredBuffer").c_str(),
					screen_vs_gl,
					deferred_buffer_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_deferredBufferShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.DeferredBuffer").c_str(),
					screen_vs_dx,
					deferred_buffer_ps_dx,
					vl,
					macro);
			}
		}

		// SSAO用シェーダー
		{
			m_ssaoVertexBuffer = g->CreateVertexBuffer_Imp(sizeof(ScreenVertexLayout), 2 * 3, true);
			m_ssaoIndexBuffer = g->CreateIndexBuffer_Imp(2 * 3, false, false);

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

			ssao = std::make_shared<SSAO>(g);
		}

		factory = new RenderingCommandFactory();
	}

	Renderer3DProxy::~Renderer3DProxy()
	{
		ResetCommands();

		ReleaseObjects(objects);
		ReleaseObjects(cameraObjects);
		ReleaseObjects(directionalLightObjects);
	}

	void Renderer3DProxy::Rendering(RenderTexture2D_Imp* renderTarget)
	{
		RenderingCommandHelper helper_(commands, factory);
		auto helper = &helper_;
		using h = RenderingCommandHelper;
		SafeAddRef(renderTarget);

		for (auto& o : objects)
		{
			o->OnUpdateAsync();
		}

		for (auto& o : cameraObjects)
		{
			o->OnUpdateAsync();
		}

		for (auto& o : directionalLightObjects)
		{
			o->OnUpdateAsync();
		}

		// エフェクトの更新
		effectManager->Update(DeltaTime);

		RenderingProperty prop;
		prop.IsLightweightMode = Settings.IsLightweightMode;
		prop.IsDepthMode = false;
		prop.DummyTextureWhite = dummyTextureWhite;
		prop.DummyTextureBlack = dummyTextureBlack;
		prop.DummyTextureNormal = dummyTextureNormal;

		// ライトの計算
		{
			if (directionalLightObjects.size() > 0)
			{
				auto lightP = (RenderedDirectionalLightObject3DProxy*) (*(directionalLightObjects.begin()));
				prop.DirectionalLightColor = lightP->LightColor;
				prop.DirectionalLightDirection = lightP->GetDirection();
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

			prop.GroundLightColor = GroundAmbientColor;
			prop.SkyLightColor = SkyAmbientColor;
		}

		for (auto& co : cameraObjects)
		{
			auto cP = (RenderedCameraObject3DProxy*) co;

			if (Settings.IsLightweightMode)
			{
				RenderCameraOnLightweight(helper, cP, prop);
			}
			else
			{
				RenderCamera(helper, cP, prop);
			}
		}

		for (auto& co : cameraObjects)
		{
			auto cP = (RenderedCameraObject3DProxy*)co;

			helper->SetRenderTarget(renderTarget, nullptr);

			Texture2D* texture = nullptr;
			if (Settings.IsLightweightMode || Settings.VisalizedBuffer == eVisalizedBuffer::VISALIZED_BUFFER_FINALIMAGE)
			{
				texture = cP->GetAffectedRenderTarget();
			}
			else
			{
				texture = cP->GetRenderTarget();
			}

			RenderState state;
			state.DepthTest = false;
			state.DepthWrite = false;
			state.AlphaBlendState = AlphaBlend::Opacity;
			state.Culling = ace::CullingType::Double;

			helper->Draw(2, m_pasteVertexBuffer.get(), m_pasteIndexBuffer.get(), m_pasteShader.get(), state,
				h::GenValue("g_texture", h::Texture2DPair(texture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp))
				);
		}

		SafeRelease(renderTarget);
	}

	void Renderer3DProxy::RenderCamera(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cP, RenderingProperty prop)
	{
		using h = RenderingCommandHelper;

		// カメラプロジェクション行列計算
		Matrix44 cameraProjMat;
		ace::Matrix44::Mul(cameraProjMat, cP->ProjectionMatrix, cP->CameraMatrix);
		prop.CameraMatrix = cP->CameraMatrix;
		prop.ProjectionMatrix = cP->ProjectionMatrix;

		// 3D描画
		{
			// 奥行き描画
			{
				helper->SetRenderTarget(cP->GetRenderTargetDepth(), cP->GetDepthBuffer());
				helper->Clear(true, true, ace::Color(0, 0, 0, 255));
				prop.IsDepthMode = true;
				for (auto& o : objects)
				{
					o->Rendering(helper, prop);
				}
			}

			// Gバッファ描画
			{
				helper->SetRenderTarget(
					cP->GetRenderTargetDiffuseColor(),
					cP->GetRenderTargetSpecularColor_Smoothness(),
					cP->GetRenderTargetDepth(),
					cP->GetRenderTargetAO_MatID(),
					cP->GetDepthBuffer());
				helper->Clear(true, false, ace::Color(0, 0, 0, 255));
				prop.IsDepthMode = false;
				for (auto& o : objects)
				{
					o->Rendering(helper, prop);
				}
			}
		}


		// SSAO
		if (ssao->IsEnabled())
		{
			ssao->Draw(helper, cP, m_ssaoVertexBuffer, m_ssaoIndexBuffer);
		}

		// 蓄積リセット
		{
			helper->SetRenderTarget(cP->GetRenderTarget(), nullptr);
			helper->Clear(true, false, ace::Color(0, 0, 0, 255));
		}

		// 直接光描画
		{
			auto invCameraMat = (prop.CameraMatrix).GetInverted();

			auto fov = cP->FOV / 180.0f * 3.141592f;
			auto aspect = (float) cP->WindowSize.X / (float) cP->WindowSize.Y;

			float yScale = 1 / tanf(fov / 2);
			float xScale = yScale / aspect;

			auto reconstructInfo1 = Vector3DF(cP->ZNear * cP->ZFar, cP->ZFar - cP->ZNear, -cP->ZFar);
			//auto reconstructInfo1 = Vector3DF(cP->ZFar - cP->ZNear, cP->ZNear, 0.0f);
			auto reconstructInfo2 = Vector4DF(1.0f / xScale, 1.0f / yScale, 0.0f, 0.0f);

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
			for (auto& light_ : directionalLightObjects)
			{
				auto lightP = static_cast<RenderedDirectionalLightObject3DProxy*>(light_);

				RenderTexture2D_Imp* shadowMap = lightP->GetShadowTexture();

				Matrix44 view, proj;

				lightP->CalcShadowMatrix(
					cP->GetGlobalPosition(),
					cP->Focus - cP->GetGlobalPosition(),
					cP->Up,
					cameraProjMat,
					cP->ZNear,
					cP->ZFar,
					view,
					proj);

				// 影マップ作成
				{
					helper->SetRenderTarget(lightP->GetShadowTexture(), lightP->GetShadowDepthBuffer());
					helper->Clear(true, true, ace::Color(0, 0, 0, 255));

					RenderingProperty shadowProp = prop;
					shadowProp.IsDepthMode = true;
					shadowProp.CameraMatrix = view;
					shadowProp.ProjectionMatrix = proj;

					for (auto& o : objects)
					{
						o->Rendering(helper, shadowProp);
					}

					float intensity = 2.0f;
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
						helper->SetRenderTarget((RenderTexture2D_Imp*) m_shadowTempTexture.get(), nullptr);
						helper->Clear(true, false, ace::Color(0, 0, 0, 255));

						RenderState state;
						state.DepthTest = false;
						state.DepthWrite = false;
						state.Culling = CullingType::Double;

						helper->Draw(2, m_shadowVertexBuffer.get(), m_shadowIndexBuffer.get(), m_shadowShaderX.get(), state,
							h::GenValue("g_weight", weights),
							h::GenValue("g_texture", h::Texture2DPair(lightP->GetShadowTexture(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));
					}

						{
							helper->SetRenderTarget(lightP->GetShadowTexture(), nullptr);
							helper->Clear(true, false, ace::Color(0, 0, 0, 255));

							RenderState state;
							state.DepthTest = false;
							state.DepthWrite = false;
							state.Culling = CullingType::Double;

							helper->Draw(2, m_shadowVertexBuffer.get(), m_shadowIndexBuffer.get(), m_shadowShaderY.get(), state,
								h::GenValue("g_weight", weights),
								h::GenValue("g_texture", h::Texture2DPair(m_shadowTempTexture.get(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));
						}
				}

				// 光源描画
					{
						helper->SetRenderTarget(cP->GetRenderTarget(), nullptr);

						std::shared_ptr<ace::NativeShader_Imp> shader;

						if (lightIndex == 0)
						{
							shader = m_directionalWithAmbientLightShader;
						}
						else
						{
							shader = m_directionalLightShader;
						}

						Texture2D* ssaoTexture = dummyTextureWhite.get();
						if (ssao->IsEnabled())
						{
							ssaoTexture = cP->GetRenderTargetSSAO();
						}

						auto CameraPositionToShadowCameraPosition = (view) * invCameraMat;
						auto ShadowProjection = proj;

						Vector3DF directionalLightDirection;
						Vector3DF directionalLightColor;

						directionalLightDirection = prop.DirectionalLightDirection;
						directionalLightDirection = prop.CameraMatrix.Transform3D(directionalLightDirection) - zero;

						directionalLightColor.X = prop.DirectionalLightColor.R / 255.0f;
						directionalLightColor.Y = prop.DirectionalLightColor.G / 255.0f;
						directionalLightColor.Z = prop.DirectionalLightColor.B / 255.0f;

						RenderState state;
						state.DepthTest = false;
						state.DepthWrite = false;
						state.Culling = CullingType::Double;
						state.AlphaBlendState = AlphaBlend::Add;

						helper->Draw(2, m_shadowVertexBuffer.get(), m_shadowIndexBuffer.get(), shader.get(), state,
							h::GenValue("skyLightColor", skyLightColor),
							h::GenValue("groundLightColor", groundLightColor),
							h::GenValue("directionalLightDirection", directionalLightDirection),
							h::GenValue("directionalLightColor", directionalLightColor),
							h::GenValue("upDir", upDir),
							h::GenValue("reconstructInfo1", reconstructInfo1),
							h::GenValue("reconstructInfo2", reconstructInfo2),
							h::GenValue("g_shadowProjection", ShadowProjection),
							h::GenValue("g_cameraPositionToShadowCameraPosition", CameraPositionToShadowCameraPosition),
							h::GenValue("g_ssaoTexture", h::Texture2DPair(ssaoTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_gbuffer0Texture", h::Texture2DPair(cP->GetRenderTargetDiffuseColor(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_gbuffer1Texture", h::Texture2DPair(cP->GetRenderTargetSpecularColor_Smoothness(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_gbuffer2Texture", h::Texture2DPair(cP->GetRenderTargetDepth(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_gbuffer3Texture", h::Texture2DPair(cP->GetRenderTargetAO_MatID(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_shadowmapTexture", h::Texture2DPair(lightP->GetShadowTexture(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp))
							);
					}

				lightIndex++;
			}

			// TODO 一切光源がない場合の環境光
		}

		// エフェクトの描画
		helper->DrawEffect(cP->ProjectionMatrix, cP->CameraMatrix);

		if (Settings.VisalizedBuffer == eVisalizedBuffer::VISALIZED_BUFFER_FINALIMAGE)
		{
			// ポストエフェクト適用
			cP->ApplyPostEffects(helper);
		}
		else
		{
			helper->SetRenderTarget(cP->GetRenderTarget(), nullptr);
			helper->Clear(true, false, Color(0, 0, 0, 0));

			std::shared_ptr<ace::NativeShader_Imp> shader = m_deferredBufferShader;

			float flag = 0.0f;
			if (Settings.VisalizedBuffer == eVisalizedBuffer::VISALIZED_BUFFER_DIFFUSE)
			{
				flag = 0.0f;
			}
			else if (Settings.VisalizedBuffer == eVisalizedBuffer::VISALIZED_BUFFER_NORMAL)
			{
				flag = 1.0f;
			}

			Texture2D* ssaoTexture = dummyTextureWhite.get();
			if (ssao->IsEnabled())
			{
				ssaoTexture = cP->GetRenderTargetSSAO();
			}

			RenderState state;
			state.DepthTest = false;
			state.DepthWrite = false;
			state.Culling = CullingType::Double;
			state.AlphaBlendState = AlphaBlend::Opacity;

			helper->Draw(2, m_shadowVertexBuffer.get(), m_shadowIndexBuffer.get(), shader.get(), state,
				h::GenValue("flag", flag),
				h::GenValue("g_ssaoTexture", h::Texture2DPair(ssaoTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
				h::GenValue("g_gbuffer0Texture", h::Texture2DPair(cP->GetRenderTargetDiffuseColor(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
				h::GenValue("g_gbuffer1Texture", h::Texture2DPair(cP->GetRenderTargetSpecularColor_Smoothness(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
				h::GenValue("g_gbuffer2Texture", h::Texture2DPair(cP->GetRenderTargetDepth(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
				h::GenValue("g_gbuffer3Texture", h::Texture2DPair(cP->GetRenderTargetAO_MatID(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp))
				);
		}
	}

	void Renderer3DProxy::RenderCameraOnLightweight(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cP, RenderingProperty prop)
	{
		using h = RenderingCommandHelper;

		// カメラプロジェクション行列計算
		Matrix44 cameraProjMat;
		ace::Matrix44::Mul(cameraProjMat, cP->ProjectionMatrix, cP->CameraMatrix);
		prop.CameraMatrix = cP->CameraMatrix;
		prop.ProjectionMatrix = cP->ProjectionMatrix;

		// 3D描画
		{
			helper->SetRenderTarget(cP->GetRenderTarget(), cP->GetDepthBuffer());
			helper->Clear(true, true, ace::Color(0, 0, 0, 255));

			for (auto& o : objects)
			{
				o->Rendering(helper, prop);
			}
		}

		// エフェクトの描画
		helper->DrawEffect(cP->ProjectionMatrix, cP->CameraMatrix);

		// ポストエフェクト適用
		cP->ApplyPostEffects(helper);
	}

	void Renderer3DProxy::SetEffect(Effekseer::Manager* manager, EffekseerRenderer::Renderer* renderer)
	{
		effectManager = manager;
		effectRenderer = renderer;
	}

	void Renderer3DProxy::AddObject(RenderedObject3D* o)
	{
		auto proxy = o->GetProxy();
		if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_CAMERA)
		{
			if (cameraObjects.count(proxy) == 0)
			{
				SafeAddRef(proxy);
				cameraObjects.insert(proxy);
			}
		}
		else if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT)
		{
			if (directionalLightObjects.count(proxy) == 0)
			{
				SafeAddRef(proxy);
				directionalLightObjects.insert(proxy);
			}
		}
		else
		{
			if (objects.count(proxy) == 0)
			{
				SafeAddRef(proxy);
				objects.insert(proxy);
			}
		}
	}

	void Renderer3DProxy::RemoveObject(RenderedObject3D* o)
	{
		auto proxy = o->GetProxy();
		if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_CAMERA)
		{
			if (cameraObjects.count(proxy) > 0)
			{
				cameraObjects.erase(proxy);
				SafeRelease(proxy);
			}
		}
		else if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT)
		{
			if (directionalLightObjects.count(proxy) > 0)
			{
				directionalLightObjects.erase(proxy);
				SafeRelease(proxy);
			}
		}
		else
		{
			if (objects.count(proxy) > 0)
			{
				objects.erase(proxy);
				SafeRelease(proxy);
			}
		}
	}

	std::vector<RenderingCommand*>& Renderer3DProxy::GetCommands()
	{
		return commands;
	}

	void Renderer3DProxy::ResetCommands()
	{
		for (auto& c : commands)
		{
			c->~RenderingCommand();
		}
		commands.clear();
		factory->Reset();
	}
}
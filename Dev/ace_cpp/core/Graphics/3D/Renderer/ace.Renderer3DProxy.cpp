
#include "ace.Renderer3DProxy.h"

#include "../../Command/ace.RenderingCommandFactory.h"
#include "../../Command/ace.RenderingCommandHelper.h"
#include "../../Command/ace.RenderingCommand.h"


#include "../Object/ace.RenderedCameraObject3D.h"
#include "../Object/ace.RenderedDirectionalLightObject3D.h"
#include "../Object/ace.RenderedMassModelObject3D.h"
#include "../Object/ace.RenderedTerrainObject3D.h"

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

#define __CULLING__ 1

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

	void Renderer3DProxy::DrawMassObjects(RenderingCommandHelper* helper, RenderingProperty prop)
	{
		MassModel* currentModel = nullptr;
		MaterialPropertyBlock* currentBlock = nullptr;

		int32_t offset = 0;

		// 大量描画モデル
		auto drawMass = [&](int32_t current)-> void
		{
			auto count = current - offset;
			sortedMassModelObjects[offset]->Draw(helper, prop, sortedMassModelObjects, offset, count);
		};

		if (sortedMassModelObjects.size() > 0)
		{
			currentModel = sortedMassModelObjects[0]->ModelPtr;
			currentBlock = sortedMassModelObjects[0]->materialPropertyBlock.get();
		}

		for (auto i = 0; i < sortedMassModelObjects.size(); i++)
		{
			if (
				sortedMassModelObjects[i]->ModelPtr != currentModel ||
				sortedMassModelObjects[i]->materialPropertyBlock.get() != currentBlock)
			{
				drawMass(i);
				currentModel = sortedMassModelObjects[i]->ModelPtr;
				currentBlock = sortedMassModelObjects[i]->materialPropertyBlock.get();
				offset = i;
			}
		}

		if (sortedMassModelObjects.size() > 0 && offset != sortedMassModelObjects.size())
		{
			drawMass(sortedMassModelObjects.size());
		}
	}

	Renderer3DProxy::Renderer3DProxy(Graphics* graphics)
		: graphics(graphics)
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

		environmentRendering = std::make_shared<EnvironmentRendering>(g, m_shadowVertexBuffer, m_shadowIndexBuffer);

		factory = new RenderingCommandFactory();

		// とりあえず適当にレイヤー生成
		CullingWorld = Culling3D::World::Create(10000.0f, 500.0f, 10000.0f, 6);
	}

	Renderer3DProxy::~Renderer3DProxy()
	{
		ResetCommands();

		ReleaseObjects(objects);
		ReleaseObjects(cameraObjects);
		ReleaseObjects(directionalLightObjects);
		ReleaseObjects(massModelObjects);

		Culling3D::SafeRelease(CullingWorld);
	}

	void Renderer3DProxy::Rendering(RenderTexture2D_Imp* renderTarget)
	{
		if (ssao != nullptr)
		{
			ssao->Bias = SSAO_Bias;
			ssao->FarPlain = SSAO_FarPlain;
			ssao->Intensity = SSAO_Intensity;
			ssao->Radius = SSAO_Radius;
		}

		RenderingCommandHelper helper_(commands, factory);
		auto helper = &helper_;
		using h = RenderingCommandHelper;
		SafeAddRef(renderTarget);

		for (auto& o : objects)
		{
			o->OnUpdateAsync();
		}

		for (auto& o : massModelObjects)
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

		// ソート
#if __CULLING__
#else
		sortedMassModelObjects.clear();

		for (auto& o : massModelObjects)
		{
			sortedMassModelObjects.push_back((RenderedMassModelObject3DProxy*)o);
		}
		std::sort(
			sortedMassModelObjects.begin(),
			sortedMassModelObjects.end(),
			[](const RenderedMassModelObject3DProxy* a, const RenderedMassModelObject3DProxy* b) -> bool {
			if (a->ModelPtr != b->ModelPtr) return a->ModelPtr > b->ModelPtr;

			return a->materialPropertyBlock.get() > b->materialPropertyBlock.get();
		});
#endif	

		// エフェクトの更新
		effectManager->Update(DeltaTime / (1.0f/60.0f));

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
			if (Settings.IsLightweightMode || Settings.VisualizedBuffer == VisualizedBufferType::FinalImage)
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

#if __CULLING__
		// カリング
		Culling(cameraProjMat);
		//CullingWorld->Dump("dump.csv", *((Culling3D::Matrix44*)(&cameraProjMat)), false);

		// 大量描画ソート
		SortAndSetMassObjects(culledMassModelObjects);
#endif

		// 3D描画
		{
			// 奥行き描画
			{
				helper->SetRenderTarget(cP->GetRenderTargetDepth(), cP->GetDepthBuffer());
				helper->Clear(true, true, ace::Color(0, 0, 0, 255));
				prop.IsDepthMode = true;

				// 通常モデル
#if __CULLING__
				DrawObjects(culledObjects, helper, prop);
#else
				DrawObjects(objects, helper, prop);
#endif			

				// 大量描画モデル
				DrawMassObjects(helper, prop);

#if __CULLING__
				// 地形
				for (auto& terrain : culledTerrainObjects)
				{
					auto p = (RenderedTerrainObject3DProxy*)(terrain->ProxyPtr);
					p->Rendering(terrain->TerrainIndex, helper, prop);
				}
#endif
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

				// 通常モデル
#if __CULLING__
				DrawObjects(culledObjects, helper, prop);
#else
				DrawObjects(objects, helper, prop);
#endif			

				// 大量描画モデル
				DrawMassObjects(helper, prop);

#if __CULLING__
				// 地形
				for (auto& terrain : culledTerrainObjects)
				{
					auto p = (RenderedTerrainObject3DProxy*) (terrain->ProxyPtr);
					p->Rendering(terrain->TerrainIndex, helper, prop);
				}
#endif
			}
		}

		// 環境描画
		environmentRendering->Render(
			cP, helper,
			cP->GetRenderTargetDiffuseColor(), cP->GetRenderTargetSpecularColor_Smoothness(), cP->GetRenderTargetDepth(), cP->GetRenderTargetAO_MatID(),
			EnvironmentDiffuseColorIntensity, EnvironmentSpecularColorIntensity, EnvironmentDiffuseColor.get(), EnvironmentSpecularColor.get());

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
			auto zero = prop.CameraMatrix.Transform3D(Vector3DF());

			Vector3DF groundLightColor(
				prop.GroundLightColor.R / 255.0f * AmbientColorIntensity,
				prop.GroundLightColor.G / 255.0f * AmbientColorIntensity,
				prop.GroundLightColor.B / 255.0f * AmbientColorIntensity);

			Vector3DF skyLightColor(
				prop.SkyLightColor.R / 255.0f * AmbientColorIntensity,
				prop.SkyLightColor.G / 255.0f * AmbientColorIntensity,
				prop.SkyLightColor.B / 255.0f * AmbientColorIntensity);

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

#if __CULLING__
				// カリング
				Culling(proj * view);

				// 大量描画ソート
				SortAndSetMassObjects(culledMassModelObjects);
#endif

				// 影マップ作成
				{
					helper->SetRenderTarget(lightP->GetShadowTexture(), lightP->GetShadowDepthBuffer());
					helper->Clear(true, true, ace::Color(0, 0, 0, 255));

					RenderingProperty shadowProp = prop;
					shadowProp.IsDepthMode = true;
					shadowProp.CameraMatrix = view;
					shadowProp.ProjectionMatrix = proj;

#if __CULLING__
					DrawObjects(culledObjects, helper, shadowProp);
#else
					DrawObjects(objects, helper, shadowProp);
#endif
					DrawMassObjects(helper, shadowProp);

#if __CULLING__
					// 地形
					for (auto& terrain : culledTerrainObjects)
					{
						auto p = (RenderedTerrainObject3DProxy*) (terrain->ProxyPtr);
						p->Rendering(terrain->TerrainIndex, helper, shadowProp);
					}
#endif

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

						auto directionalLightIntensity = lightP->Intensity;
						directionalLightColor.X = prop.DirectionalLightColor.R / 255.0f * directionalLightIntensity;
						directionalLightColor.Y = prop.DirectionalLightColor.G / 255.0f * directionalLightIntensity;
						directionalLightColor.Z = prop.DirectionalLightColor.B / 255.0f * directionalLightIntensity;

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
							h::GenValue("upDir", cP->UpDir),
							h::GenValue("reconstructInfo1", cP->ReconstructInfo1),
							h::GenValue("reconstructInfo2", cP->ReconstructInfo2),
							h::GenValue("g_shadowProjection", ShadowProjection),
							h::GenValue("g_cameraPositionToShadowCameraPosition", CameraPositionToShadowCameraPosition),
							h::GenValue("g_ssaoTexture", h::Texture2DPair(ssaoTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_gbuffer0Texture", h::Texture2DPair(cP->GetRenderTargetDiffuseColor(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_gbuffer1Texture", h::Texture2DPair(cP->GetRenderTargetSpecularColor_Smoothness(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_gbuffer2Texture", h::Texture2DPair(cP->GetRenderTargetDepth(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_gbuffer3Texture", h::Texture2DPair(cP->GetRenderTargetAO_MatID(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_shadowmapTexture", h::Texture2DPair(lightP->GetShadowTexture(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
							h::GenValue("g_environmentDiffuseTexture", h::Texture2DPair(cP->GetRenderTargetEnvironment(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp))
							);

						
					}

				lightIndex++;
			}

			// 一切光源がない場合の環境光
			if (lightIndex == 0)
			{
				helper->SetRenderTarget(cP->GetRenderTarget(), nullptr);

				std::shared_ptr<ace::NativeShader_Imp> shader = m_ambientLightShader;

				Texture2D* ssaoTexture = dummyTextureWhite.get();
				if (ssao->IsEnabled())
				{
					ssaoTexture = cP->GetRenderTargetSSAO();
				}

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
					h::GenValue("upDir", cP->UpDir),
					h::GenValue("reconstructInfo1", cP->ReconstructInfo1),
					h::GenValue("reconstructInfo2", cP->ReconstructInfo2),
					h::GenValue("g_ssaoTexture", h::Texture2DPair(ssaoTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
					h::GenValue("g_gbuffer0Texture", h::Texture2DPair(cP->GetRenderTargetDiffuseColor(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
					h::GenValue("g_gbuffer1Texture", h::Texture2DPair(cP->GetRenderTargetSpecularColor_Smoothness(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
					h::GenValue("g_gbuffer2Texture", h::Texture2DPair(cP->GetRenderTargetDepth(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
					h::GenValue("g_gbuffer3Texture", h::Texture2DPair(cP->GetRenderTargetAO_MatID(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
					h::GenValue("g_environmentDiffuseTexture", h::Texture2DPair(cP->GetRenderTargetEnvironment(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp))
					);
			}
		}

		// 深度復帰
		{
			helper->SetRenderTarget(cP->GetRenderTarget(), cP->GetDepthBuffer());
		}

		// エフェクトの描画
		helper->DrawEffect(cP->ProjectionMatrix, cP->CameraMatrix);
		
		// スプライトの描画
		helper->DrawSprite(cP->ProjectionMatrix, cP->CameraMatrix);

		// 深度リセット
		{
			helper->SetRenderTarget(cP->GetRenderTarget(), nullptr);
		}

		if (Settings.VisualizedBuffer == VisualizedBufferType::FinalImage)
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
			if (Settings.VisualizedBuffer == VisualizedBufferType::DiffuseColor)
			{
				flag = 0.0f;
			}
			else if (Settings.VisualizedBuffer == VisualizedBufferType::Normal)
			{
				flag = 1.0f;
			}
			else if (Settings.VisualizedBuffer == VisualizedBufferType::SpecularColor)
			{
				flag = 2.0f;
			}
			else if (Settings.VisualizedBuffer == VisualizedBufferType::Smoothness)
			{
				flag = 3.0f;
			}
			else if (Settings.VisualizedBuffer == VisualizedBufferType::Environment)
			{
				flag = 4.0f;
			}
			else if (Settings.VisualizedBuffer == VisualizedBufferType::Occlusion)
			{
				flag = 5.0f;
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
				h::GenValue("g_gbuffer3Texture", h::Texture2DPair(cP->GetRenderTargetAO_MatID(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
				h::GenValue("g_environmentTexture", h::Texture2DPair(cP->GetRenderTargetEnvironment(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp))
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

#if __CULLING__
		// カリング
		Culling(cameraProjMat);

		// 大量描画ソート
		SortAndSetMassObjects(culledMassModelObjects);
#endif

		// 3D描画
		{
			helper->SetRenderTarget(cP->GetRenderTarget(), cP->GetDepthBuffer());
			helper->Clear(true, true, ace::Color(0, 0, 0, 255));

			// 通常モデル
#if __CULLING__
			DrawObjects(culledObjects, helper, prop);
#else
			DrawObjects(objects, helper, prop);
#endif			

			// 大量描画モデル
			DrawMassObjects(helper, prop);

#if __CULLING__
			// 地形
			for (auto& terrain : culledTerrainObjects)
			{
				auto p = (RenderedTerrainObject3DProxy*) (terrain->ProxyPtr);
				p->Rendering(terrain->TerrainIndex, helper, prop);
			}
#endif
		}

		// スプライトの描画
		helper->DrawSprite(cP->ProjectionMatrix, cP->CameraMatrix);

		// エフェクトの描画
		helper->DrawEffect(cP->ProjectionMatrix, cP->CameraMatrix);

		// ポストエフェクト適用
		cP->ApplyPostEffects(helper);
	}

	void Renderer3DProxy::Culling(const Matrix44& viewProjectionMat)
	{
		culledObjects.clear();
		culledTerrainObjects.clear();
		culledMassModelObjects.clear();

		Matrix44 viewProjectionMat_ = viewProjectionMat;
		CullingWorld->Culling(*((Culling3D::Matrix44*)(&viewProjectionMat_)), graphics->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL);

		for (auto i = 0; i < CullingWorld->GetObjectCount(); i++)
		{
			auto cp = (RenderedObject3DCullingProxy*)(CullingWorld->GetObject(i)->GetUserData());

			if (cp->ProxyPtr->GetObjectType() == RENDERED_OBJECT3D_TYPE_MESH)
			{
				culledObjects.push_back(cp->ProxyPtr);
			}
			else if (cp->ProxyPtr->GetObjectType() == RENDERED_OBJECT3D_TYPE_MASSOBJECT)
			{
				culledMassModelObjects.push_back(cp->ProxyPtr);
			}
			else if (cp->ProxyPtr->GetObjectType() == RENDERED_OBJECT3D_TYPE_TERRAIN)
			{
				culledTerrainObjects.push_back(cp);
			}
		}
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
				proxy->OnAdded(this);
			}
		}
		else if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT)
		{
			if (directionalLightObjects.count(proxy) == 0)
			{
				SafeAddRef(proxy);
				directionalLightObjects.insert(proxy);
				proxy->OnAdded(this);
			}
		}
		else if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_MASSOBJECT)
		{
			if (massModelObjects.count(proxy) == 0)
			{
				SafeAddRef(proxy);
				massModelObjects.insert(proxy);
				proxy->OnAdded(this);
			}
		}
		else
		{
			if (objects.count(proxy) == 0)
			{
				SafeAddRef(proxy);
				objects.insert(proxy);
				proxy->OnAdded(this);
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
				proxy->OnRemoving(this);
				cameraObjects.erase(proxy);
				SafeRelease(proxy);
			}
		}
		else if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT)
		{
			if (directionalLightObjects.count(proxy) > 0)
			{
				proxy->OnRemoving(this);
				directionalLightObjects.erase(proxy);
				SafeRelease(proxy);
			}
		}
		else if (o->GetObjectType() == eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_MASSOBJECT)
		{
			if (massModelObjects.count(proxy) > 0)
			{
				proxy->OnRemoving(this);
				massModelObjects.erase(proxy);
				SafeRelease(proxy);
			}
		}
		else
		{
			if (objects.count(proxy) > 0)
			{
				proxy->OnRemoving(this);
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
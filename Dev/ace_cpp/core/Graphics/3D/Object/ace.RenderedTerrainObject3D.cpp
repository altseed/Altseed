
#include "ace.RenderedTerrainObject3D.h"

#include "../Resource/ace.Terrain3D_Imp.h"

#include "../Renderer/ace.Renderer3D.h"
#include "../Renderer/ace.Renderer3DProxy.h"

#include "../../ace.Graphics_Imp.h"

#include "../../Resource/ace.ShaderCache.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"
#include "../../Resource/ace.MaterialPropertyBlock_Imp.h"
#include "../../Resource/ace.Shader3D_Imp.h"
#include "../../Resource/ace.Material3D_Imp.h"

#include "../../Shader/DX/3D/Lightweight_Terrain_Internal_VS.h"
#include "../../Shader/DX/3D/Lightweight_Terrain_Internal_PS.h"
#include "../../Shader/DX/3D/Terrain_Internal_VS.h"
#include "../../Shader/DX/3D/Terrain_Internal_PS.h"

#include "../../Shader/GL/3D/Lightweight_Terrain_Internal_VS.h"
#include "../../Shader/GL/3D/Lightweight_Terrain_Internal_PS.h"
#include "../../Shader/GL/3D/Terrain_Internal_VS.h"
#include "../../Shader/GL/3D/Terrain_Internal_PS.h"

#include "../../Command/ace.RenderingCommandHelper.h"

namespace ace
{
	RenderedTerrainObject3DProxy::RenderedTerrainObject3DProxy(Graphics* graphics)
	{
		auto g = (Graphics_Imp*) graphics;

		std::vector<ace::VertexLayout> vl;
		vl.push_back(ace::VertexLayout("Position", ace::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Normal", ace::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Binormal", ace::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("UVSub", ace::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::VertexLayoutFormat::R8G8B8A8_UNORM));

		{
			std::vector<ace::Macro> macro;
			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_shaderLightweight = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.TerrainObject3D.Lightweight").c_str(),
					lightweight_terrain_internal_vs_gl,
					lightweight_terrain_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderLightweight = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.TerrainObject3D.Lightweight").c_str(),
					lightweight_terrain_internal_vs_dx,
					lightweight_terrain_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderLightweight != nullptr);
		}

		{
			std::vector<ace::Macro> macro;
			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_shaderDF = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.TerrainObject3D.DF").c_str(),
					terrain_internal_vs_gl,
					terrain_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderDF = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.TerrainObject3D.DF").c_str(),
					terrain_internal_vs_dx,
					terrain_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderDF != nullptr);
		}

		{
			std::vector<ace::Macro> macro;
			macro.push_back(Macro("EXPORT_DEPTH", "1"));

			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_shaderDF_ND = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.TerrainObject3D.DF.ND").c_str(),
					terrain_internal_vs_gl,
					terrain_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderDF_ND = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.TerrainObject3D.DF.ND").c_str(),
					terrain_internal_vs_dx,
					terrain_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderDF_ND != nullptr);
		}

		{
			std::vector<ace::Macro> macro;
			macro.push_back(Macro("BLACK", "1"));
			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_shaderBlackLightweight = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.TerrainObject3D.Lightweight.Black").c_str(),
					lightweight_terrain_internal_vs_gl,
					lightweight_terrain_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderBlackLightweight = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.TerrainObject3D.Lightweight.Black").c_str(),
					lightweight_terrain_internal_vs_dx,
					lightweight_terrain_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderBlackLightweight != nullptr);
		}

		{
			std::vector<ace::Macro> macro;
			macro.push_back(Macro("BLACK", "1"));
			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_shaderBlack = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.TerrainObject3D.Black").c_str(),
					terrain_internal_vs_gl,
					terrain_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderBlack = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.TerrainObject3D.Black").c_str(),
					terrain_internal_vs_dx,
					terrain_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderBlack != nullptr);
		}
	}

	RenderedTerrainObject3DProxy::~RenderedTerrainObject3DProxy()
	{
		SafeRelease(TerrainPtr);
	}

	void RenderedTerrainObject3DProxy::RegisterCulling()
	{
		if (isCullingRegistered) return;
		if (TerrainPtr == nullptr) return;

		auto terrainPtr = (Terrain3D_Imp*) TerrainPtr;

		cullingProxies.resize(terrainPtr->Proxy.Clusters.size());

		for (auto i = 0; i < cullingProxies.size(); i++)
		{
			cullingProxies[i].ProxyPtr = this;
			cullingProxies[i].TerrainIndex = i;
		}

		int32_t ind = 0;
		for (auto& cluster : terrainPtr->Proxy.Clusters)
		{
			auto o = Culling3D::Object::Create();
			auto center = cluster->Center;
			auto size = cluster->Size;
			auto pos = Culling3D::Vector3DF(center.X, center.Y, center.Z);

			o->SetShapeType(Culling3D::eObjectShapeType::OBJECT_SHAPE_TYPE_CUBOID);

			o->SetPosition(pos);
			o->SetCuboidSize(Culling3D::Vector3DF(size.X, size.Y, size.Z));
			o->SetUserData(&(cullingProxies[ind]));
			
			cullingObjects.push_back(o);
			renderer->CullingWorld->AddObject(o);
			ind++;
		}

		isCullingRegistered = true;
	}

	void RenderedTerrainObject3DProxy::UnregisterCulling()
	{
		if (!isCullingRegistered) return;

		for (auto o : cullingObjects)
		{
			renderer->CullingWorld->RemoveObject(o);
			o->Release();
		}

		cullingProxies.clear();
		cullingObjects.clear();

		isCullingRegistered = false;
	}

	void RenderedTerrainObject3DProxy::UpdateCullingMatrix(const Matrix44& mat)
	{
		if (!isCullingRegistered) return;

		auto x = mat.Values[0][3];
		auto y = mat.Values[1][3];
		auto z = mat.Values[2][3];
	
		if (TerrainPtr == nullptr) return;

		auto terrainPtr = (Terrain3D_Imp*) TerrainPtr;

		int32_t ind = 0;
		for (auto& cluster : terrainPtr->Proxy.Clusters)
		{
			auto center = cluster->Center;
			auto pos = Culling3D::Vector3DF(center.X + x, center.Y + y, center.Z + z);
			cullingObjects[ind]->SetPosition(pos);
			ind++;
		}
	}

	void RenderedTerrainObject3DProxy::OnAdded(Renderer3DProxy* renderer)
	{
		this->renderer = renderer;
		RegisterCulling();
	}

	void RenderedTerrainObject3DProxy::OnRemoving(Renderer3DProxy* renderer)
	{
		UnregisterCulling();
		this->renderer = nullptr;
	}

	void RenderedTerrainObject3DProxy::Rendering(RenderingCommandHelper* helper, RenderingProperty& prop)
	{
		using h = RenderingCommandHelper;

		if (TerrainPtr == nullptr) return;
		auto terrain = (Terrain3D_Imp*)TerrainPtr;

		if (terrain->Proxy.GridWidthCount == 0) return;
		if (terrain->Proxy.GridHeightCount == 0) return;

		
		auto lightDirection = prop.DirectionalLightDirection;
		Vector3DF lightColor(prop.DirectionalLightColor.R / 255.0f, prop.DirectionalLightColor.G / 255.0f, prop.DirectionalLightColor.B / 255.0f);
		Vector3DF groudLColor(prop.GroundLightColor.R / 255.0f, prop.GroundLightColor.G / 255.0f, prop.GroundLightColor.B / 255.0f);
		Vector3DF skyLColor(prop.SkyLightColor.R / 255.0f, prop.SkyLightColor.G / 255.0f, prop.SkyLightColor.B / 255.0f);

		auto matM = GetGlobalMatrix();

		
		for (auto i = 0; i < terrain->Proxy.Clusters.size(); i++)
		{
			Rendering(i, helper, prop);
		}
	}

	void RenderedTerrainObject3DProxy::Rendering(int32_t index, RenderingCommandHelper* helper, RenderingProperty& prop)
	{
		using h = RenderingCommandHelper;

		if (TerrainPtr == nullptr) return;
		auto terrain = (Terrain3D_Imp*) TerrainPtr;

		if (terrain->Proxy.GridWidthCount == 0) return;
		if (terrain->Proxy.GridHeightCount == 0) return;

		auto lightDirection = prop.DirectionalLightDirection;
		Vector3DF lightColor(prop.DirectionalLightColor.R / 255.0f, prop.DirectionalLightColor.G / 255.0f, prop.DirectionalLightColor.B / 255.0f);
		Vector3DF groudLColor(prop.GroundLightColor.R / 255.0f, prop.GroundLightColor.G / 255.0f, prop.GroundLightColor.B / 255.0f);
		Vector3DF skyLColor(prop.SkyLightColor.R / 255.0f, prop.SkyLightColor.G / 255.0f, prop.SkyLightColor.B / 255.0f);

		auto matM = GetGlobalMatrix();

		auto& cluster = terrain->Proxy.Clusters[index];
		
		// リセット
		{
			shaderConstants.clear();

			std::shared_ptr<ace::NativeShader_Imp> shader;

			if (prop.IsLightweightMode)
			{
				shader = m_shaderBlackLightweight;
			}
			else
			{
				if (prop.IsDepthMode)
				{
					shader = m_shaderDF_ND;
				}
				else
				{
					shader = m_shaderBlack;
				}
			}

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matC", prop.CameraMatrix));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matP", prop.ProjectionMatrix));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matM", matM));

			auto vb = cluster->Black.VB;
			auto ib = cluster->Black.IB;

			ace::Texture2D* colorTexture = prop.DummyTextureWhite.get();

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_colorTexture",
				h::Texture2DPair(colorTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Repeat)));

			RenderState state;
			state.DepthTest = true;
			state.DepthWrite = true;
			state.Culling = CullingType::Front;
			state.AlphaBlendState = AlphaBlend::Opacity;

			helper->DrawWithPtr(
				ib->GetCount() / 3,
				vb.get(),
				ib.get(),
				shader.get(),
				state,
				shaderConstants.data(),
				shaderConstants.size());
		}

		if (prop.IsDepthMode) return;

		// サーフェース描画
		for (auto& polygon : cluster->Surfaces)
		{
			auto& surface = terrain->Proxy.Surfaces[polygon.SurfaceIndex];

			shaderConstants.clear();

			std::shared_ptr<ace::NativeShader_Imp> shader;

			if (terrain->Proxy.Material_ != nullptr)
			{
				auto shader_ = (Shader3D_Imp*) terrain->Proxy.Material_->GetShader3D().get();

				if (prop.IsLightweightMode)
				{
					shader = shader_->GetNativeShaderTerrainLight();
				}
				else
				{
					shader = shader_->GetNativeShaderTerrain();
				}
			}
			else
			{
				if (prop.IsLightweightMode)
				{
					shader = m_shaderLightweight;
				}
				else
				{
					shader = m_shaderDF;
				}
			}


			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matC", prop.CameraMatrix));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matP", prop.ProjectionMatrix));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matM", matM));

			if (prop.IsLightweightMode)
			{
				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "directionalLightDirection", lightDirection));
				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "directionalLightColor", lightColor));
				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "skyLightColor", skyLColor));
				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "groundLightColor", groudLColor));
			}
			else
			{
			}

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_densityTexture",
				h::Texture2DPair(surface.DensityTexture.get(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));


			ace::Texture2D* colorTexture = prop.DummyTextureWhite.get();
			ace::Texture2D* normalTexture = prop.DummyTextureNormal.get();
			ace::Texture2D* metalnessTexture = prop.DummyTextureBlack.get();
			ace::Texture2D* smoothnessTexture = prop.DummyTextureBlack.get();

			if (surface.ColorTexture != nullptr)
			{
				colorTexture = surface.ColorTexture.get();
			}

			if (!prop.IsLightweightMode)
			{
				if (surface.NormalTexture != nullptr)
				{
					normalTexture = surface.NormalTexture.get();
				}

				if (surface.MetalnessTexture != nullptr)
				{
					metalnessTexture = surface.MetalnessTexture.get();
					//smoothnessTexture = polygon->SpecularTexture.get();
				}
			}

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_colorTexture",
				h::Texture2DPair(colorTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Repeat)));

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_normalTexture",
				h::Texture2DPair(normalTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Repeat)));

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_metalnessTexture",
				h::Texture2DPair(metalnessTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Repeat)));

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_smoothnessTexture",
				h::Texture2DPair(smoothnessTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Repeat)));

			if (terrain->Proxy.Material_ != nullptr)
			{
				auto mat = (Material3D_Imp*) (terrain->Proxy.Material_.get());

				std::shared_ptr<MaterialPropertyBlock> block;
				if (materialPropertyBlock.get() != nullptr)
				{
					// ユーザー定義ブロック使用
					block = materialPropertyBlock;
				}
				else
				{
					block = mat->GetMaterialPropertyBlock();
				}

				((MaterialPropertyBlock_Imp*) block.get())->AddValuesTo(shader.get(), shaderConstants);
			}
			else
			{
			}

			auto vb = polygon.VB;
			auto ib = polygon.IB;

			RenderState state;
			state.DepthTest = true;
			state.DepthWrite = true;
			state.Culling = CullingType::Front;
			state.AlphaBlendState = AlphaBlend::AddAll;

			helper->DrawWithPtr(
				ib->GetCount() / 3,
				vb.get(),
				ib.get(),
				shader.get(),
				state,
				shaderConstants.data(),
				shaderConstants.size());
		}
	
	}

	RenderedTerrainObject3D::RenderedTerrainObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
	{
		proxy = new RenderedTerrainObject3DProxy(graphics);
	}

	RenderedTerrainObject3D::~RenderedTerrainObject3D()
	{
		SafeRelease(terrain);
		SafeRelease(proxy);
	}

	void RenderedTerrainObject3D::SetMaterialPropertyBlock(MaterialPropertyBlock* block)
	{
		SafeAddRef(block);
		materialPropertyBlock = CreateSharedPtrWithReleaseDLL(block);
	}

	void RenderedTerrainObject3D::SetTerrain(Terrain3D* terrain)
	{
		SafeSubstitute(this->terrain, terrain);
	}

	void RenderedTerrainObject3D::OnApplyingNextSRT()
	{
		auto pos = this->GetPosition();
		
		proxy->UpdateCullingMatrix(GetLocalMatrix());
	}

	void RenderedTerrainObject3D::Flip(float deltaTime)
	{
		RenderedObject3D::Flip(deltaTime);

		auto t = (Terrain3D_Imp*) terrain;
		t->Commit();

		auto oldTerrainPtr = proxy->TerrainPtr;

		SafeSubstitute(proxy->TerrainPtr, terrain);

		if (proxy->TerrainPtr != oldTerrainPtr)
		{
			if (proxy->TerrainPtr != nullptr && oldTerrainPtr != nullptr)
			{
				proxy->UnregisterCulling();
				proxy->RegisterCulling();
				proxy->UpdateCullingMatrix(GetLocalMatrix());
			}
			else if (proxy->TerrainPtr != nullptr)
			{
				proxy->RegisterCulling();
				proxy->UpdateCullingMatrix(GetLocalMatrix());
			}
			else if (proxy->TerrainPtr == nullptr)
			{
				proxy->UnregisterCulling();
			}
		}

		proxy->materialPropertyBlock = materialPropertyBlock;
	}
}
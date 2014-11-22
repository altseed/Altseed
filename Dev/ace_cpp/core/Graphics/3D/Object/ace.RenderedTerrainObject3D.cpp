
#include "ace.RenderedTerrainObject3D.h"

#include "../Resource/ace.Terrain3D_Imp.h"

#include "../Renderer/ace.Renderer3D.h"

#include "../../ace.Graphics_Imp.h"

#include "../../Resource/ace.ShaderCache.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"

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
		vl.push_back(ace::VertexLayout("Position", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Normal", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Binormal", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("UVSub", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));

		{
			std::vector<ace::Macro> macro;
			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				//m_shaderLightweight = g->GetShaderCache()->CreateFromCode(
				//	ToAString("Internal.TerrainObject3D.Lightweight").c_str(),
				//	lightweight_model_internal_vs_gl,
				//	lightweight_model_internal_ps_gl,
				//	vl,
				//	macro);
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
				//m_shaderDF = g->GetShaderCache()->CreateFromCode(
				//	ToAString("Internal.TerrainObject3D.DF").c_str(),
				//	model_internal_vs_gl,
				//	model_internal_ps_gl,
				//	vl,
				//	macro);
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
				//m_shaderDF_ND = g->GetShaderCache()->CreateFromCode(
				//	ToAString("Internal.TerrainObject3D.DF.ND").c_str(),
				//	model_internal_vs_gl,
				//	model_internal_ps_gl,
				//	vl,
				//	macro);
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
				//m_shaderBlackLightweight = g->GetShaderCache()->CreateFromCode(
				//	ToAString("Internal.TerrainObject3D.Lightweight.Black").c_str(),
				//	lightweight_model_internal_vs_gl,
				//	lightweight_model_internal_ps_gl,
				//	vl,
				//	macro);
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
				//m_shaderBlack = g->GetShaderCache()->CreateFromCode(
				//	ToAString("Internal.TerrainObject3D.Black").c_str(),
				//	lightweight_model_internal_vs_gl,
				//	lightweight_model_internal_ps_gl,
				//	vl,
				//	macro);
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

			auto vb = terrain->Proxy.VB;
			auto ib = terrain->Proxy.IB;

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
		for (auto& polygon : terrain->Polygons)
		{
			shaderConstants.clear();

			std::shared_ptr<ace::NativeShader_Imp> shader;

			if (prop.IsLightweightMode)
			{
				shader = m_shaderLightweight;
			}
			else
			{
				shader = m_shaderDF;
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

			ace::Texture2D* colorTexture = prop.DummyTextureWhite.get();
			ace::Texture2D* normalTexture = prop.DummyTextureNormal.get();
			ace::Texture2D* specularTexture = prop.DummyTextureBlack.get();
			ace::Texture2D* smoothnessTexture = prop.DummyTextureBlack.get();

			if (polygon->ColorTexture != nullptr)
			{
				colorTexture = polygon->ColorTexture.get();
			}

			if (!prop.IsLightweightMode)
			{
				if (polygon->NormalTexture != nullptr)
				{
					normalTexture = polygon->NormalTexture.get();
				}

				if (polygon->SpecularTexture != nullptr)
				{
					specularTexture = polygon->SpecularTexture.get();
					//smoothnessTexture = polygon->SpecularTexture.get();
				}
			}

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_colorTexture",
				h::Texture2DPair(colorTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Repeat)));

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_normalTexture",
				h::Texture2DPair(normalTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Repeat)));

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_specularTexture",
				h::Texture2DPair(specularTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Repeat)));

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_smoothnessTexture",
				h::Texture2DPair(smoothnessTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Repeat)));

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_densityTexture",
				h::Texture2DPair(polygon->DensityTexture.get(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

			auto vb = polygon->VB;
			auto ib = polygon->IB;

			RenderState state;
			state.DepthTest = true;
			state.DepthWrite = true;
			state.Culling = CullingType::Front;
			state.AlphaBlendState = AlphaBlend::Add;

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

	void RenderedTerrainObject3D::SetTerrain(Terrain3D* terrain)
	{
		SafeSubstitute(this->terrain, terrain);
	}

	void RenderedTerrainObject3D::Flip(float deltaTime)
	{
		RenderedObject3D::Flip(deltaTime);

		auto t = (Terrain3D_Imp*) terrain;
		t->Commit();

		SafeSubstitute(proxy->TerrainPtr, terrain);
	}
}
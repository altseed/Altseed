
#include "ace.RenderedMassModelObject3D.h"
#include "../Resource/ace.MassModel.h"

#include "../../ace.Graphics_Imp.h"

#include "../../ace.Graphics_Imp.h"
#include "../../Resource/ace.ShaderCache.h"
#include "../../Resource/ace.NativeShader_Imp.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"

#include "../../3D/Resource/ace.MassModel_Imp.h"

#include "../../Shader/DX/3D/Lightweight_MassModel_Internal_VS.h"
#include "../../Shader/DX/3D/Lightweight_Model_Internal_PS.h"

#include "../../Shader/DX/3D/MassModel_Internal_VS.h"
#include "../../Shader/DX/3D/Model_Internal_PS.h"

#include "../../Command/ace.RenderingCommandHelper.h"

namespace ace
{
	RenderedMassModelObject3DProxy::RenderedMassModelObject3DProxy(Graphics* graphics)
	{
		deviceType = graphics->GetGraphicsDeviceType();

		auto g = (Graphics_Imp*) graphics;

		std::vector<ace::VertexLayout> vl;
		vl.push_back(ace::VertexLayout("Position", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Normal", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Binormal", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("UVSub", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));
		vl.push_back(ace::VertexLayout("BoneWeights", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));
		vl.push_back(ace::VertexLayout("BoneIndexes", ace::LAYOUT_FORMAT_R8G8B8A8_UINT));
		vl.push_back(ace::VertexLayout("BoneIndexesOriginal", ace::LAYOUT_FORMAT_R8G8B8A8_UINT));

		{
			std::vector<ace::Macro> macro;
			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				/*
				m_shaderLightweight = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.MassModelObject3D.Lightweight").c_str(),
					lightweight_model_internal_vs_gl,
					lightweight_model_internal_ps_gl,
					vl,
					macro);
				*/
			}
			else
			{
				m_shaderLightweight = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.MassModelObject3D.Lightweight").c_str(),
					lightweight_mass_model_internal_vs_dx,
					lightweight_model_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderLightweight != nullptr);
		}

		{
			std::vector<ace::Macro> macro;
			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				/*
				m_shaderDF = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.MassModelObject3D.DF").c_str(),
					model_internal_vs_gl,
					model_internal_ps_gl,
					vl,
					macro);
				*/
			}
			else
			{
				m_shaderDF = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.MassModelObject3D.DF").c_str(),
					mass_model_internal_vs_dx,
					model_internal_ps_dx,
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
				/*
				m_shaderDF_ND = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.DF.ND").c_str(),
					model_internal_vs_gl,
					model_internal_ps_gl,
					vl,
					macro);
				*/
			}
			else
			{
				m_shaderDF_ND = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.DF.ND").c_str(),
					mass_model_internal_vs_dx,
					model_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderDF_ND != nullptr);
		}
	}

	RenderedMassModelObject3DProxy::~RenderedMassModelObject3DProxy()
	{
		SafeRelease(ModelPtr);
	}

	void RenderedMassModelObject3DProxy::Rendering(RenderingCommandHelper* helper, RenderingProperty& prop)
	{
		assert(0);
	}

	void RenderedMassModelObject3DProxy::Draw(RenderingCommandHelper* helper, RenderingProperty& prop, std::vector<RenderedMassModelObject3DProxy*>& proxies, int32_t offset, int32_t count)
	{
		using h = RenderingCommandHelper;

		auto lightDirection = prop.DirectionalLightDirection;
		Vector3DF lightColor(prop.DirectionalLightColor.R / 255.0f, prop.DirectionalLightColor.G / 255.0f, prop.DirectionalLightColor.B / 255.0f);
		Vector3DF groudLColor(prop.GroundLightColor.R / 255.0f, prop.GroundLightColor.G / 255.0f, prop.GroundLightColor.B / 255.0f);
		Vector3DF skyLColor(prop.SkyLightColor.R / 255.0f, prop.SkyLightColor.G / 255.0f, prop.SkyLightColor.B / 255.0f);
		
		Matrix44 matM[MaxObject];

		float animationIndex0[MaxObject];
		float animationIndex1[MaxObject];
		float animationTime0[MaxObject];
		float animationTime1[MaxObject];
		float animationWeight[MaxObject];

		int32_t prop_count = 0;

		auto draw = [&]()-> void
		{
			shaderConstants.clear();

			std::shared_ptr<ace::NativeShader_Imp> shader;

			if (prop.IsLightweightMode)
			{
				shader = m_shaderLightweight;
			}
			else
			{
				if (prop.IsDepthMode)
				{
					shader = m_shaderDF_ND;
				}
				else
				{
					shader = m_shaderDF;
				}
			}

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matC", prop.CameraMatrix));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matP", prop.ProjectionMatrix));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matM", h::Array<Matrix44>(matM, 32)));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "animationIndex0", h::Array<float>(animationIndex0, 32)));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "animationIndex1", h::Array<float>(animationIndex1, 32)));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "animationTime0", h::Array<float>(animationTime0, 32)));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "animationTime1", h::Array<float>(animationTime1, 32)));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "animationWeight", h::Array<float>(animationWeight, 32)));

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

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_colorTexture",
				h::Texture2DPair(colorTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_normalTexture",
				h::Texture2DPair(normalTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_specularTexture",
				h::Texture2DPair(specularTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_smoothnessTexture",
				h::Texture2DPair(smoothnessTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

			auto modelPtr = (MassModel_Imp*)ModelPtr;
			auto vb = modelPtr->GetVertexBuffer();
			auto ib = modelPtr->GetIndexBuffer();

			RenderState state;
			state.DepthTest = true;
			state.DepthWrite = true;
			state.Culling = CullingType::Front;
			state.AlphaBlendState = AlphaBlend::Opacity;

			helper->DrawInstancedWithPtr(
				ib->GetCount() / 3,
				prop_count,
				vb.get(),
				ib.get(),
				shader.get(),
				state,
				shaderConstants.data(),
				shaderConstants.size());
		};

		for (auto i = offset; i < offset + count; i++)
		{
			matM[prop_count] = proxies[i]->GetGlobalMatrix();
			animationIndex0[prop_count] = 0;
			animationIndex1[prop_count] = 0;
			animationTime0[prop_count] = 0;
			animationTime1[prop_count] = 0;
			animationWeight[prop_count] = 0;

			prop_count++;

			if (prop_count == MaxObject)
			{
				draw();
				prop_count = 0;
			}
		}

		if (prop_count  > 0)
		{
			draw();
		}
	}

	RenderedMassModelObject3D::RenderedMassModelObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
	{
		proxy = new RenderedMassModelObject3DProxy(graphics);
	}

	RenderedMassModelObject3D::~RenderedMassModelObject3D()
	{
		SafeRelease(model);
		SafeRelease(proxy);
	}

	MassModel* RenderedMassModelObject3D::GetModel()
	{
		return model;
	}

	void RenderedMassModelObject3D::SetModel(MassModel* model)
	{
		SafeSubstitute(this->model, model);
	}

	void RenderedMassModelObject3D::Flip(float deltaTime)
	{
		RenderedObject3D::Flip(deltaTime);

		SafeSubstitute(proxy->ModelPtr, model);
	}
}

#include "asd.RenderedMassModelObject3D.h"
#include "../Resource/asd.MassModel.h"

#include "../Renderer/asd.Renderer3D.h"
#include "../Renderer/asd.Renderer3DProxy.h"

#include "../../asd.Graphics_Imp.h"

#include "../../asd.Graphics_Imp.h"
#include "../../Resource/asd.ShaderCache.h"
#include "../../Resource/asd.NativeShader_Imp.h"
#include "../../Resource/asd.IndexBuffer_Imp.h"
#include "../../Resource/asd.MaterialPropertyBlock_Imp.h"
#include "../../Resource/asd.Shader3D_Imp.h"
#include "../../Resource/asd.Material3D_Imp.h"

#include "../../3D/Resource/asd.MassModel_Imp.h"

#include "../../Shader/DX/3D/Lightweight_MassModel_Internal_VS.h"
#include "../../Shader/DX/3D/Lightweight_Model_Internal_PS.h"

#include "../../Shader/DX/3D/MassModel_Internal_VS.h"
#include "../../Shader/DX/3D/Model_Internal_PS.h"

#include "../../Shader/GL/3D/Lightweight_MassModel_Internal_VS.h"
#include "../../Shader/GL/3D/Lightweight_Model_Internal_PS.h"

#include "../../Shader/GL/3D/MassModel_Internal_VS.h"
#include "../../Shader/GL/3D/Model_Internal_PS.h"

#include "../../Command/asd.RenderingCommandHelper.h"

namespace asd
{
	RenderedMassModelObject3DProxy::RenderedMassModelObject3DProxy(Graphics* graphics)
	{
		deviceType = graphics->GetGraphicsDeviceType();

		auto g = (Graphics_Imp*) graphics;

		for (int32_t i = 0; i < 2; i++)
		{
			std::vector<asd::VertexLayout> vl;
			vl.push_back(asd::VertexLayout("Position", asd::VertexLayoutFormat::R32G32B32_FLOAT));
			vl.push_back(asd::VertexLayout("Normal", asd::VertexLayoutFormat::R32G32B32_FLOAT));
			vl.push_back(asd::VertexLayout("Binormal", asd::VertexLayoutFormat::R32G32B32_FLOAT));
			vl.push_back(asd::VertexLayout("UV", asd::VertexLayoutFormat::R32G32_FLOAT));
			vl.push_back(asd::VertexLayout("UVSub", asd::VertexLayoutFormat::R32G32_FLOAT));
			vl.push_back(asd::VertexLayout("Color", asd::VertexLayoutFormat::R8G8B8A8_UNORM));
			vl.push_back(asd::VertexLayout("BoneWeights", asd::VertexLayoutFormat::R8G8B8A8_UNORM));
			vl.push_back(asd::VertexLayout("BoneIndexes", asd::VertexLayoutFormat::R8G8B8A8_UINT));

			astring numKey;
			if (i == 0)
			{
				numKey = ToAString(".0");
			}
			else if (i == 1)
			{
				numKey = ToAString(".1");
			}

			{
				std::vector<asd::Macro> macro;
				if (i == 1) macro.push_back(Macro("ANIMATION_IS_ENABLED", "1"));

				if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
				{
					m_shadersLightweight[i] = g->GetShaderCache()->CreateFromCode(
						(ToAString("Internal.MassModelObject3D.Lightweight") + numKey).c_str(),
						lightweight_mass_model_internal_vs_gl,
						lightweight_model_internal_ps_gl,
						vl,
						macro);
				}
				else
				{
					m_shadersLightweight[i] = g->GetShaderCache()->CreateFromCode(
						(ToAString("Internal.MassModelObject3D.Lightweight") + numKey).c_str(),
						lightweight_mass_model_internal_vs_dx,
						lightweight_model_internal_ps_dx,
						vl,
						macro);
				}

				assert(m_shadersLightweight[i] != nullptr);
			}

		{
			std::vector<asd::Macro> macro;
			if (i == 1) macro.push_back(Macro("ANIMATION_IS_ENABLED", "1"));

			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_shadersDF[i] = g->GetShaderCache()->CreateFromCode(
					(ToAString("Internal.MassModelObject3D.DF") + numKey).c_str(),
					mass_model_internal_vs_gl,
					model_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shadersDF[i] = g->GetShaderCache()->CreateFromCode(
					(ToAString("Internal.MassModelObject3D.DF") + numKey).c_str(),
					mass_model_internal_vs_dx,
					model_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shadersDF[i] != nullptr);
		}

		{
			std::vector<asd::Macro> macro;
			macro.push_back(Macro("EXPORT_DEPTH", "1"));
			if (i == 1) macro.push_back(Macro("ANIMATION_IS_ENABLED", "1"));

			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				m_shadersDF_ND[i] = g->GetShaderCache()->CreateFromCode(
					(ToAString("Internal.ModelObject3D.DF.ND") + numKey).c_str(),
					mass_model_internal_vs_gl,
					model_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shadersDF_ND[i] = g->GetShaderCache()->CreateFromCode(
					(ToAString("Internal.ModelObject3D.DF.ND") + numKey).c_str(),
					mass_model_internal_vs_dx,
					model_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shadersDF_ND[i] != nullptr);
		}
		}

		cullingProxy.ProxyPtr = this;

		CullingObject = Culling3D::Object::Create();
		CullingObject->SetUserData(&cullingProxy);
	}

	RenderedMassModelObject3DProxy::~RenderedMassModelObject3DProxy()
	{
		SafeRelease(ModelPtr);
		Culling3D::SafeRelease(CullingObject);
	}

	void RenderedMassModelObject3DProxy::OnAdded(Renderer3DProxy* renderer)
	{
		renderer->CullingWorld->AddObject(CullingObject);
	}

	void RenderedMassModelObject3DProxy::OnRemoving(Renderer3DProxy* renderer)
	{
		renderer->CullingWorld->RemoveObject(CullingObject);
	}

	void RenderedMassModelObject3DProxy::Rendering(RenderingCommandHelper* helper, RenderingProperty& prop)
	{
		assert(0);
	}

	void RenderedMassModelObject3DProxy::Draw(RenderingCommandHelper* helper, RenderingProperty& prop, std::vector<RenderedMassModelObject3DProxy*>& proxies, int32_t offset, int32_t count)
	{
		using h = RenderingCommandHelper;

		auto lightDirection = prop.DirectionalLightDirection;
		Vector3DF lightColor = prop.DirectionalLightColor;
		Vector3DF groudLColor(prop.GroundLightColor.R / 255.0f, prop.GroundLightColor.G / 255.0f, prop.GroundLightColor.B / 255.0f);
		Vector3DF skyLColor(prop.SkyLightColor.R / 255.0f, prop.SkyLightColor.G / 255.0f, prop.SkyLightColor.B / 255.0f);
		
		Matrix44 matM[MaxObject];
		Vector4DF animationParam0[MaxObject];
		Vector4DF animationParam1[MaxObject];

		int32_t prop_count = 0;

		auto draw = [&]()-> void
		{
			shaderConstants.clear();

			auto modelPtr = (MassModel_Imp*) ModelPtr;
			auto& material = modelPtr->GetMaterial();

			bool hasAnimation = modelPtr->GetAnimationTexture() != nullptr;
			int32_t shaderIndex = hasAnimation ? 1 : 0;

			std::shared_ptr<asd::NativeShader_Imp> shader;

			if (material.Material_.get() != nullptr)
			{
				auto mat = (Material3D_Imp*) (material.Material_.get());

				auto shader_ = (Shader3D_Imp*) (mat->GetShader3D().get());

				if (prop.IsLightweightMode)
				{
					if (prop.IsDepthMode)
					{
						shader = shader_->GetNativeShaderMassLightDepth(shaderIndex);
					}
					else
					{
						shader = shader_->GetNativeShaderMassLight(shaderIndex);
					}
				}
				else
				{
					if (prop.IsDepthMode)
					{
						shader = shader_->GetNativeShaderMassDepth(shaderIndex);
					}
					else
					{
						shader = shader_->GetNativeShaderMass(shaderIndex);
					}
				}
			}
			else
			{
				if (prop.IsLightweightMode)
				{
					shader = m_shadersLightweight[shaderIndex];
				}
				else
				{
					if (prop.IsDepthMode)
					{
						shader = m_shadersDF_ND[shaderIndex];
					}
					else
					{
						shader = m_shadersDF[shaderIndex];
					}
				}
			}
			
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matC", prop.CameraMatrix));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matP", prop.ProjectionMatrix));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matM", h::Array<Matrix44>(matM, 32)));

			if (hasAnimation)
			{
				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "animationParam0", h::Array<Vector4DF>(animationParam0, 32)));
				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "animationParam1", h::Array<Vector4DF>(animationParam1, 32)));

				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_animationTexture",
					h::Texture2DPair(modelPtr->GetAnimationTexture().get(), asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)));
			}
			
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

			if (material.Material_.get() != nullptr)
			{
				auto mat = (Material3D_Imp*) (material.Material_.get());

				// 定数設定

				auto currentP = proxies[offset];

				std::shared_ptr<MaterialPropertyBlock> block;
				if (currentP->materialPropertyBlock.get() != nullptr)
				{
					// ユーザー定義ブロック使用
					block = currentP->materialPropertyBlock;
				}
				else
				{
					block = mat->GetMaterialPropertyBlock();
				}

				((MaterialPropertyBlock_Imp*) block.get())->AddValuesTo(shader.get(), shaderConstants);

				auto vb = modelPtr->GetVertexBuffer();
				auto ib = modelPtr->GetIndexBuffer();

				RenderState state;
				state.DepthTest = true;
				state.DepthWrite = true;
				state.Culling = CullingType::Front;
				state.AlphaBlendState = AlphaBlendMode::Opacity;

				helper->DrawInstancedWithPtr(
					ib->GetCount() / 3,
					prop_count,
					vb.get(),
					ib.get(),
					shader.get(),
					state,
					shaderConstants.data(),
					shaderConstants.size());
			}
			else
			{
				asd::Texture2D* colorTexture = prop.DummyTextureWhite.get();
				asd::Texture2D* normalTexture = prop.DummyTextureNormal.get();
				asd::Texture2D* metalnessTexture = prop.DummyTextureBlack.get();
				asd::Texture2D* smoothnessTexture = prop.DummyTextureBlack.get();

				if (material.ColorTexture != nullptr)
				{
					colorTexture = material.ColorTexture.get();
				}

				if (!prop.IsLightweightMode)
				{
					if (material.NormalTexture != nullptr)
					{
						normalTexture = material.NormalTexture.get();
					}

					if (material.MetalnessTexture != nullptr)
					{
						metalnessTexture = material.MetalnessTexture.get();
					}

					if (material.SmoothnessTexture != nullptr)
					{
						smoothnessTexture = material.SmoothnessTexture.get();
					}
				}

				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_colorTexture",
					h::Texture2DPair(colorTexture, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)));

				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_normalTexture",
					h::Texture2DPair(normalTexture, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)));

				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_metalnessTexture",
					h::Texture2DPair(metalnessTexture, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)));

				shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_smoothnessTexture",
					h::Texture2DPair(smoothnessTexture, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)));

				auto vb = modelPtr->GetVertexBuffer();
				auto ib = modelPtr->GetIndexBuffer();

				RenderState state;
				state.DepthTest = true;
				state.DepthWrite = true;
				state.Culling = CullingType::Front;
				state.AlphaBlendState = AlphaBlendMode::Opacity;

				helper->DrawInstancedWithPtr(
					ib->GetCount() / 3,
					prop_count,
					vb.get(),
					ib.get(),
					shader.get(),
					state,
					shaderConstants.data(),
					shaderConstants.size());
			}
		};

		for (auto i = offset; i < offset + count; i++)
		{
			matM[prop_count] = proxies[i]->GetGlobalMatrix();
			
			animationParam0[prop_count].X = proxies[i]->AnimationIndex0;
			animationParam0[prop_count].Y = proxies[i]->AnimationIndex1;
			animationParam0[prop_count].Z = proxies[i]->AnimationTime0;
			animationParam0[prop_count].W = proxies[i]->AnimationTime1;
			animationParam1[prop_count].X = proxies[i]->AnimationWeight;

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

	void RenderedMassModelObject3D::SetMaterialPropertyBlock(MaterialPropertyBlock* block)
	{
		SafeAddRef(block);
		materialPropertyBlock = CreateSharedPtrWithReleaseDLL(block);
	}

	MassModel* RenderedMassModelObject3D::GetModel()
	{
		return model;
	}

	void RenderedMassModelObject3D::SetModel(MassModel* model)
	{
		SafeSubstitute(this->model, model);
	}

	void RenderedMassModelObject3D::PlayAnimation(const achar* name)
	{
		if (model == nullptr) return;

		auto modelPtr = (MassModel_Imp*)model;
		auto index = modelPtr->GetClipIndex(name);
		if (index < 0) return;

		animationIndex0 = index;
		animationIndex1 = 0;
		animationTime0 = 0;
		animationTime1 = 0;
		animationWeight = 0.0f;
		isAnimationPlaying0 = true;
		isAnimationPlaying1 = false;

		isFading = false;
		variation = 0.0f;
	}

	void RenderedMassModelObject3D::StopAnimation()
	{
		animationIndex0 = 0;
		animationIndex1 = 0;
		animationTime0 = 0;
		animationTime1 = 0;
		animationWeight = 0.0f;
		isAnimationPlaying0 = false;
		isAnimationPlaying1 = false;

		isFading = false;
		variation = 0.0f;
	}

	void RenderedMassModelObject3D::CrossFadeAnimation(const achar* name, float time)
	{
		if (model == nullptr) return;

		auto modelPtr = (MassModel_Imp*) model;
		auto index = modelPtr->GetClipIndex(name);
		if (index < 0) return;

		animationIndex1 = index;
		animationTime1 = 0;
		animationWeight = 0.0f;
		isAnimationPlaying1 = true;

		isFading = true;
		variation = time;
	}

	bool RenderedMassModelObject3D::GetIsAnimationPlaying()
	{
		return isAnimationPlaying0 || isAnimationPlaying1;
	}

	void RenderedMassModelObject3D::OnApplyingNextSRT()
	{
		auto pos = this->GetPosition();
		proxy->CullingObject->SetPosition(Culling3D::Vector3DF(pos.X, pos.Y, pos.Z));
		// 仮の数値
		proxy->CullingObject->ChangeIntoSphere(200.0f);
	}

	void RenderedMassModelObject3D::Flip(float deltaTime)
	{
		RenderedObject3D::Flip(deltaTime);

		auto modelPtr = (MassModel_Imp*) model;

		SafeSubstitute(proxy->ModelPtr, model);

		proxy->materialPropertyBlock = materialPropertyBlock;

		proxy->AnimationIndex0 = animationIndex0;
		proxy->AnimationIndex1 = animationIndex1;
		proxy->AnimationTime0 = animationTime0;
		proxy->AnimationTime1 = animationTime1;
		proxy->AnimationWeight = animationWeight;

		if (isFading)
		{
			if (variation == 0)
			{
				animationWeight = 1.0f;
			}
			else
			{
				animationWeight += (deltaTime / variation);
			}

			if (animationWeight >= 1.0f)
			{
				animationWeight = 0.0f;
				animationIndex0 = animationIndex1;
				animationTime0 = animationTime1;
				isAnimationPlaying0 = isAnimationPlaying1;
				animationWeight = 0.0f;
				isFading = false;
			}
		}

		if (isAnimationPlaying0)
		{
			animationTime0 += (deltaTime / (1.0 / 60.0));

			if (modelPtr->GetIsLoopingMode(animationIndex0))
			{
				while (animationTime0 >= modelPtr->GetFrameCount(animationIndex0))
				{
					animationTime0 -= modelPtr->GetFrameCount(animationIndex0);
				}
			}
			else
			{
				if (animationTime0 >= modelPtr->GetFrameCount(animationIndex0))
				{
					animationTime0 = modelPtr->GetFrameCount(animationIndex0) - 1;
				}
			}
		}

		if (isAnimationPlaying1)
		{
			animationTime1 += (deltaTime / (1.0 / 60.0));

			if (modelPtr->GetIsLoopingMode(animationIndex1))
			{
				while (animationTime1 >= modelPtr->GetFrameCount(animationIndex1))
				{
					animationTime1 -= modelPtr->GetFrameCount(animationIndex1);
				}
			}
			else
			{
				if (animationTime1 >= modelPtr->GetFrameCount(animationIndex1))
				{
					animationTime1 = modelPtr->GetFrameCount(animationIndex1) - 1;
				}
			}
		}
	}
}
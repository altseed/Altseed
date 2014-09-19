
#include "ace.RenderedModelObject3D.h"
#include "../Resource/ace.Mesh_Imp.h"
#include "../Resource/ace.Deformer_Imp.h"
#include "../Resource/ace.Model_Imp.h"
#include "../Renderer/ace.Renderer3D.h"

#include "../Resource/Animation/ace.AnimationClip_Imp.h"
#include "../Resource/Animation/ace.AnimationSource_Imp.h"
#include "../Resource/Animation/ace.KeyframeAnimation_Imp.h"

#include "../../ace.Graphics_Imp.h"
#include "../../Resource/ace.ShaderCache.h"
#include "../../Resource/ace.NativeShader_Imp.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"
#include "../../Resource/ace.MaterialPropertyBlock_Imp.h"
#include "../../Resource/ace.Shader3D_Imp.h"
#include "../../Resource/ace.Material3D_Imp.h"

#include "../../Shader/DX/3D/Lightweight_Model_Internal_VS.h"
#include "../../Shader/DX/3D/Lightweight_Model_Internal_PS.h"
#include "../../Shader/DX/3D/Model_Internal_VS.h"
#include "../../Shader/DX/3D/Model_Internal_PS.h"

#include "../../Shader/GL/3D/Lightweight_Model_Internal_VS.h"
#include "../../Shader/GL/3D/Lightweight_Model_Internal_PS.h"
#include "../../Shader/GL/3D/Model_Internal_VS.h"
#include "../../Shader/GL/3D/Model_Internal_PS.h"

#include "../../Command/ace.RenderingCommandHelper.h"

namespace ace
{
	static void ResetAnimation(std::vector <BoneProperty>& boneProps)
	{
		for (auto i = 0; i < boneProps.size(); i++)
		{
			boneProps[i].IsAnimationPlaying = false;
			boneProps[i].Position[0] = 0.0f;
			boneProps[i].Position[1] = 0.0f;
			boneProps[i].Position[2] = 0.0f;
			boneProps[i].Rotation[0] = 0.0f;
			boneProps[i].Rotation[1] = 0.0f;
			boneProps[i].Rotation[2] = 0.0f;
			boneProps[i].Rotation[3] = 0.0f;
			boneProps[i].Scale[0] = 1.0f;
			boneProps[i].Scale[1] = 1.0f;
			boneProps[i].Scale[2] = 1.0f;
		}
	}

	static void CalculateAnimation(std::vector <BoneProperty>& boneProps, Deformer* deformer, AnimationClip* animationClip, float time, float weight, float lerp)
	{
		if (animationClip == nullptr) return;

		auto source = (AnimationSource_Imp*) animationClip->GetSource().get();
		auto& animations = source->GetAnimations();
		auto d = (Deformer_Imp*) deformer;

		for (auto& a : animations)
		{
			auto a_ = (KeyframeAnimation_Imp*) a;

			auto type = a_->GetTargetType();
			auto axis = a_->GetTargetAxis();
			auto bi = d->GetBoneIndex(a_->GetTargetName());

			if (bi < 0) continue;
			auto value = a_->GetValue(time);

			value *= weight;

			ModelUtils::SetBoneValue(
				boneProps[bi].Position,
				boneProps[bi].Rotation,
				boneProps[bi].Scale,
				type,
				axis,
				value,
				lerp);

			boneProps[bi].IsAnimationPlaying = true;
		}
	}

	static void CalclateBoneMatrices(std::vector<Matrix44>& matrixes, std::vector <BoneProperty>& boneProps, Deformer* deformer)
	{
		if (deformer == nullptr) return;
		auto d = (Deformer_Imp*) deformer;

		for (auto i = 0; i < d->GetBones().size(); i++)
		{
			auto& b = d->GetBones()[i];

			if (boneProps[i].IsAnimationPlaying)
			{
				matrixes[i] = boneProps[i].CalcMatrix(b.RotationType);
			}
			else
			{
				matrixes[i] = b.LocalMat;
			}
		}
		
		ModelUtils::CalculateBoneMatrixes(
			matrixes,
			d->GetBones(),
			matrixes);
	}

	BoneProperty::BoneProperty()
	{
		Position[0] = 0.0f;
		Position[1] = 0.0f;
		Position[2] = 0.0f;

		Rotation[0] = 0.0f;
		Rotation[1] = 0.0f;
		Rotation[2] = 0.0f;
		Rotation[3] = 0.0f;

		Scale[0] = 1.0f;
		Scale[1] = 1.0f;
		Scale[2] = 1.0f;
	}


	Matrix44 BoneProperty::CalcMatrix(eRotationOrder rotationType)
	{
		return ModelUtils::CalcMatrix(
			Position,
			Rotation,
			Scale,
			rotationType);
	}

	RenderedModelObject3DProxy::RenderedModelObject3DProxy(Graphics* graphics)
	{
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
				m_shaderLightweight = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.Lightweight").c_str(),
					lightweight_model_internal_vs_gl,
					lightweight_model_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderLightweight = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.Lightweight").c_str(),
					lightweight_model_internal_vs_dx,
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
				m_shaderDF = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.DF").c_str(),
					model_internal_vs_gl,
					model_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderDF = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.DF").c_str(),
					model_internal_vs_dx,
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
				m_shaderDF_ND = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.DF.ND").c_str(),
					model_internal_vs_gl,
					model_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderDF_ND = g->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.DF.ND").c_str(),
					model_internal_vs_dx,
					model_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderDF_ND != nullptr);
		}

	}

	RenderedModelObject3DProxy::~RenderedModelObject3DProxy()
	{

	}

	void RenderedModelObject3DProxy::OnUpdateAsync()
	{
		if (calcAnimationOnProxy)
		{
			ResetAnimation(m_boneProps);

			for (int32_t i = 0; i < AnimationCount; i++)
			{
				if (m_animationPlaying[i].size() == 0) continue;

				for (auto& anim_ : m_animationPlaying[i])
				{
					auto anim = anim_.Animation.get();
					auto loop = anim->GetIsLoopingMode();
					auto src = (AnimationSource_Imp*) anim->GetSource().get();
					auto length = src->GetLength();
					auto time = anim_.Time;
					if (loop)
					{
						time = fmodf(time, length);
					}
					CalculateAnimation(m_boneProps, m_deformer.get(), anim, time, anim_.CurrentWeight, m_animationWeight[i]);
				}
			}

			CalclateBoneMatrices(m_matrixes, m_boneProps, m_deformer.get());
		}
	}

	void RenderedModelObject3DProxy::Rendering(RenderingCommandHelper* helper, RenderingProperty& prop)
	{
		using h = RenderingCommandHelper;
		shaderConstants.clear();

		auto lightDirection = prop.DirectionalLightDirection;
		Vector3DF lightColor(prop.DirectionalLightColor.R / 255.0f, prop.DirectionalLightColor.G / 255.0f, prop.DirectionalLightColor.B / 255.0f);
		Vector3DF groudLColor(prop.GroundLightColor.R / 255.0f, prop.GroundLightColor.G / 255.0f, prop.GroundLightColor.B / 255.0f);
		Vector3DF skyLColor(prop.SkyLightColor.R / 255.0f, prop.SkyLightColor.G / 255.0f, prop.SkyLightColor.B / 255.0f);
		Matrix44 matM[32];

		
		auto& matrices = m_matrixes;
		int32_t currentMeshIndex = 0;

		for (auto& mesh_ : m_meshes)
		{
			auto mesh_root = (Mesh_Imp*) mesh_.get();

			for (auto& mesh : mesh_root->GetDvidedMeshes())
			{
				// 有効チェック
				if (mesh.IndexBufferPtr == nullptr) continue;

				auto& boneConnectors = mesh.BoneConnectors;

				// 行列計算
				if (boneConnectors.size() > 0)
				{
					// ボーンあり
					for (int32_t i = 0; i < Min(32, boneConnectors.size()); i++)
					{
						matM[i].SetIndentity();
						Matrix44::Mul(matM[i], matrices[boneConnectors[i].TargetIndex], boneConnectors[i].BoneToMesh);
						Matrix44::Mul(matM[i], GetGlobalMatrix(), matM[i]);
					}
				}
				else
				{
					// ボーンなし
					matM[0] = GetGlobalMatrix();
					for (int32_t i = 1; i < 32; i++)
					{
						matM[i] = matM[0];
					}
				}

				auto& materialOffsets = mesh.MaterialOffsets;

				{
					// 設定がある場合
					auto mIndex = 0;
					auto fOffset = 0;
					auto fCount = 0;
					auto mFCount = 0;

					Mesh_Imp::Material* material = nullptr;
					int32_t currentMaterialIndex = -1;

					while (fCount < mesh.IndexBufferPtr->GetCount() / 3)
					{
						if (materialOffsets.size() > 0)
						{
							if (fOffset == mFCount && materialOffsets.size() > mIndex)
							{
								mFCount += materialOffsets[mIndex].FaceOffset;
								material = mesh_root->GetMaterial(materialOffsets[mIndex].MaterialIndex);
								currentMaterialIndex = materialOffsets[mIndex].MaterialIndex;
								mIndex++;
							}
						}
						else
						{
							mFCount = mesh.IndexBufferPtr->GetCount() / 3;
						}

						fCount = mFCount - fOffset;
						if (fCount == 0) break;

						std::shared_ptr<ace::NativeShader_Imp> shader;

						if (material != nullptr && material->Material_.get() != nullptr)
						{
							auto mat = (Material3D_Imp*) (material->Material_.get());
							auto shader_ = (Shader3D_Imp*) (mat->GetShader3D().get());

							if (prop.IsLightweightMode)
							{
								if (prop.IsDepthMode)
								{
									shader = shader_->GetNativeShaderLightDepth();
								}
								else
								{
									shader = shader_->GetNativeShaderLight();
								}
							}
							else
							{
								if (prop.IsDepthMode)
								{
									shader = shader_->GetNativeShaderDepth();
								}
								else
								{
									shader = shader_->GetNativeShader();
								}
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
								if (prop.IsDepthMode)
								{
									shader = m_shaderDF_ND;
								}
								else
								{
									shader = m_shaderDF;
								}
							}
						}

						shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matC", prop.CameraMatrix));
						shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matP", prop.ProjectionMatrix));
						shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matM", h::Array<Matrix44>(matM, 32)));

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

						if (material != nullptr && material->Material_.get() != nullptr)
						{
							auto mat = (Material3D_Imp*) (material->Material_.get());
				
							// 定数設定
							std::shared_ptr<MaterialPropertyBlock> block;
							if (
								currentMaterialIndex >= 0 &&
								materialPropertyBlocks.size() > currentMeshIndex &&
								materialPropertyBlocks[currentMeshIndex].size() > currentMaterialIndex &&
								materialPropertyBlocks[currentMeshIndex][currentMaterialIndex].get() != nullptr)
							{
								// ユーザー定義ブロック使用
								block = materialPropertyBlocks[currentMeshIndex][currentMaterialIndex];
							}
							else
							{
								block = mat->GetMaterialPropertyBlock();
							}

							((MaterialPropertyBlock_Imp*) block.get())->AddValuesTo(shader.get(), shaderConstants);

							RenderState state;
							state.DepthTest = true;
							state.DepthWrite = true;
							state.Culling = CullingType::Front;
							state.AlphaBlendState = AlphaBlend::Opacity;

							helper->DrawWithPtr(mesh.IndexBufferPtr->GetCount() / 3, mesh.VertexBufferPtr.get(), mesh.IndexBufferPtr.get(), shader.get(), state,
								shaderConstants.data(), shaderConstants.size());
						}
						else
						{
							ace::Texture2D* colorTexture = prop.DummyTextureWhite.get();
							ace::Texture2D* normalTexture = prop.DummyTextureNormal.get();
							ace::Texture2D* specularTexture = prop.DummyTextureBlack.get();
							ace::Texture2D* smoothnessTexture = prop.DummyTextureBlack.get();

							if (material != nullptr)
							{
								if (material->ColorTexture != nullptr)
								{
									colorTexture = material->ColorTexture.get();
								}

								if (!prop.IsLightweightMode)
								{
									if (material->NormalTexture != nullptr)
									{
										normalTexture = material->NormalTexture.get();
									}

									if (material->SpecularTexture != nullptr)
									{
										specularTexture = material->SpecularTexture.get();
									}

									if (material->SmoothnessTexture != nullptr)
									{
										smoothnessTexture = material->SmoothnessTexture.get();
									}
								}
							}

							shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_colorTexture",
								h::Texture2DPair(colorTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

							shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_normalTexture",
								h::Texture2DPair(normalTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

							shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_specularTexture",
								h::Texture2DPair(specularTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

							shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_smoothnessTexture",
								h::Texture2DPair(smoothnessTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

							RenderState state;
							state.DepthTest = true;
							state.DepthWrite = true;
							state.Culling = CullingType::Front;
							state.AlphaBlendState = AlphaBlend::Opacity;

							helper->DrawWithPtr(mesh.IndexBufferPtr->GetCount() / 3, mesh.VertexBufferPtr.get(), mesh.IndexBufferPtr.get(), shader.get(), state,
								shaderConstants.data(), shaderConstants.size());
						}

						shaderConstants.clear();
						fOffset += fCount;
					}
				}
			}

			currentMeshIndex++;
		}
	}

	RenderedModelObject3D::RenderedModelObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
	{
		proxy = new RenderedModelObject3DProxy(graphics);

		for (int32_t i = 0; i < AnimationCount; i++)
		{
			m_animationPlaying[i].clear();
			m_animationWeight[i] = 1.0f;
		}
	}

	RenderedModelObject3D::~RenderedModelObject3D()
	{
		if (m_model != nullptr)
		{
			m_model->Detach(this);
		}

		SafeRelease(m_model);

		for (auto& a : m_animationClips)
		{
			a.second->Release();
		}
		m_animationClips.clear();

		SafeRelease(proxy);
	}

	void RenderedModelObject3D::SetModel(Model* model)
	{
		UnloadModel();

		if (m_model != nullptr)
		{
			m_model->Detach(this);
		}

		SafeRelease(m_model);

		if (model == nullptr) return;
		auto model_ = (Model_Imp*) model;
		m_model = model_;
		SafeAddRef(m_model);

		m_model->Attach(this);

		LoadModel();
	}

	void RenderedModelObject3D::AddMesh(Mesh* mesh)
	{
		if (mesh == nullptr) return;
		SafeAddRef(mesh);
		auto mesh_ = CreateSharedPtrWithReleaseDLL(mesh);
		m_meshes.push_back(mesh_);
	}

	void  RenderedModelObject3D::SetDeformer(Deformer* deformer)
	{
		auto d = (Deformer_Imp*) deformer;

		SafeAddRef(deformer);
		auto deformer_ = CreateSharedPtrWithReleaseDLL(deformer);
		m_deformer = deformer_;

		if (m_deformer != nullptr)
		{
			m_matrixes.resize(d->GetBones().size());
			m_boneProps.resize(d->GetBones().size());
		}
		else
		{
			m_matrixes.resize(0);
			m_boneProps.resize(0);
		}
	}

	void RenderedModelObject3D::SetMaterialPropertyBlock(int32_t meshIndex, int32_t materialIndex, MaterialPropertyBlock* block)
	{
		if (materialPropertyBlocks.size() <= meshIndex)
		{
			materialPropertyBlocks.resize(meshIndex+1);
		}

		if (materialPropertyBlocks[meshIndex].size() <= materialIndex)
		{
			materialPropertyBlocks[meshIndex].resize(materialIndex + 1);
		}

		SafeAddRef(block);
		materialPropertyBlocks[meshIndex][materialIndex] = CreateSharedPtrWithReleaseDLL(block);
	}

	void RenderedModelObject3D::UnloadModel()
	{
		// 描画中以外のオブジェクトをリセット
		m_meshes.clear();
		m_deformer.reset();

		for (auto& a : m_animationClips)
		{
			a.second->Release();
		}
		m_animationClips.clear();
	}

	void RenderedModelObject3D::LoadModel()
	{
		if (m_model == nullptr) return;

		int32_t index = 0;
		for (auto& mg : m_model->GetMeshGroup()->Meshes)
		{
			AddMesh(mg);
		}
		SetDeformer(m_model->GetMeshGroup()->Deformer_);

		for (int32_t i = 0; i < m_model->GetAnimationClips().size(); i++)
		{
			AddAnimationClip(m_model->GetAnimationClipNames()[i].c_str(), m_model->GetAnimationClips()[i]);
		}
	}

	void RenderedModelObject3D::ReloadModel()
	{
		UnloadModel();
		LoadModel();
	}

	AnimationClip* RenderedModelObject3D::GetAnimationClip(const achar* name)
	{
		if (m_animationClips.find(name) != m_animationClips.end())
		{
			return m_animationClips[name];
		}

		return nullptr;
	}

	void RenderedModelObject3D::AddAnimationClip(const achar* name, AnimationClip* animationClip)
	{
		if (animationClip == nullptr) return;

		if (m_animationClips.find(name) == m_animationClips.end())
		{
			SafeAddRef(animationClip);
			m_animationClips[name] = animationClip;
		}
	}

	void RenderedModelObject3D::PlayAnimation(int32_t index, const achar* name)
	{
		if (index >= AnimationCount) return;
		if (index < 0) return;

		auto it = m_animationClips.find(name);
		if (it == m_animationClips.end()) return;

		auto anim = (*it).second;
		SafeAddRef(anim);

		PlayedAnimation panim;
		panim.Animation = CreateSharedPtrWithReleaseDLL(anim);
		panim.CurrentWeight = 1.0f;
		panim.Variation = 0.0f;
		panim.Time = 0;

		m_animationPlaying[index].resize(1);
		m_animationPlaying[index][0] = panim;
	}

	void RenderedModelObject3D::StopAnimation(int32_t index)
	{
		if (index >= AnimationCount) return;
		if (index < 0) return;

		m_animationPlaying[index].clear();
	}

	void RenderedModelObject3D::SetAnimationWeight(int32_t index, float weight)
	{
		if (index >= AnimationCount) return;
		if (index < 0) return;

		m_animationWeight[index] = weight;
	}

	void RenderedModelObject3D::CrossFadeAnimation(int32_t index, const achar* name, float time)
	{
		if (index >= AnimationCount) return;
		if (index < 0) return;

		auto it = m_animationClips.find(name);
		if (it == m_animationClips.end()) return;

		auto anim = (*it).second;
		SafeAddRef(anim);

		PlayedAnimation panim;
		panim.Animation = CreateSharedPtrWithReleaseDLL(anim);
		panim.CurrentWeight = 0.0f;
		panim.Variation = 1.0f / time;
		panim.Time = 0;

		for (auto& a : m_animationPlaying[index])
		{
			a.Variation = -(1.0f / time) * a.CurrentWeight;
		}

		m_animationPlaying[index].push_back(panim);
	}

	bool RenderedModelObject3D::IsAnimationPlaying(int32_t index)
	{
		if (index >= AnimationCount) return false;
		if (index < 0) return false;
		if (m_animationPlaying[index].size() == 0) return false;

		auto& anims = m_animationPlaying[index];
		
		for (auto& anim : anims)
		{
			if (anim.CurrentWeight == 0.0f) continue;

			if (anim.Animation->GetIsLoopingMode())
			{
				return true;
			}
			else
			{
				auto src = (AnimationSource_Imp*) anim.Animation->GetSource().get();
				return src->GetLength() > anim.Time;
			}
		}

		return true;
	}

	void RenderedModelObject3D::OnAdded(Renderer3D* renderer)
	{
		assert(m_renderer == nullptr);
		m_renderer = renderer;
	}

	void RenderedModelObject3D::OnRemoving(Renderer3D* renderer)
	{
		assert(m_renderer == renderer);
		m_renderer = nullptr;
	}

	void RenderedModelObject3D::Flip(float deltaTime)
	{
		RenderedObject3D::Flip(deltaTime);

		bool calcAnimationOnProxy = true;

		if (calcAnimationOnProxy)
		{
			proxy->m_boneProps.resize(m_boneProps.size());
			proxy->m_matrixes.resize(m_matrixes.size());

			for (auto i = 0; i < AnimationCount; i++)
			{
				proxy->m_animationPlaying[i] = m_animationPlaying[i];
				proxy->m_animationWeight[i] = m_animationWeight[i];
			}
			
			proxy->m_deformer = m_deformer;
		}
		else
		{
			ResetAnimation(m_boneProps);

			for (int32_t i = 0; i < AnimationCount; i++)
			{
				if (m_animationPlaying[i].size() == 0) continue;

				for (auto& anim_ : m_animationPlaying[i])
				{
					auto anim = anim_.Animation.get();
					auto loop = anim->GetIsLoopingMode();
					auto src = (AnimationSource_Imp*) anim->GetSource().get();
					auto length = src->GetLength();
					auto time = anim_.Time;
					if (loop)
					{
						time = fmodf(time, length);
					}
					CalculateAnimation(m_boneProps, m_deformer.get(), anim, time, anim_.CurrentWeight, m_animationWeight[i]);
				}
			}

			CalclateBoneMatrices(m_matrixes, m_boneProps, m_deformer.get());

			proxy->m_matrixes = m_matrixes;
		}

		proxy->m_meshes = m_meshes;
		proxy->calcAnimationOnProxy = calcAnimationOnProxy;

		if (materialPropertyBlocks.size() != proxy->materialPropertyBlocks.size() || materialPropertyBlocks.size() > 0)
		{
			proxy->materialPropertyBlocks = materialPropertyBlocks;
		}
		

		// アニメーションの時間を進める
		for (auto i = 0; i < AnimationCount; i++)
		{
			bool rm = false;

			for (auto& anim : m_animationPlaying[i])
			{
				anim.Time += (deltaTime / (1.0 / 60.0));
				anim.CurrentWeight += anim.Variation * deltaTime;

				if (anim.CurrentWeight > 1.0f) anim.CurrentWeight = 1.0f;
				if (anim.CurrentWeight <= 0.0f)
				{
					anim.CurrentWeight = 0.0f;
					rm = true;
				}
			}

			if (rm)
			{
				auto it_ = std::remove_if(m_animationPlaying[i].begin(), m_animationPlaying[i].end(), 
					[](const PlayedAnimation& v)->bool { return v.CurrentWeight == 0.0f; });

				m_animationPlaying[i].erase(it_, m_animationPlaying[i].end());
			}
		}
	}
}
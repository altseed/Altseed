
#include "asd.RenderedModelObject3D.h"
#include "../Resource/asd.Mesh_Imp.h"
#include "../Resource/asd.Deformer_Imp.h"
#include "../Resource/asd.Model_Imp.h"

#include "../Renderer/asd.Renderer3D.h"
#include "../Renderer/asd.Renderer3DProxy.h"

#include "../Resource/Animation/asd.AnimationClip_Imp.h"
#include "../Resource/Animation/asd.AnimationSource_Imp.h"
#include "../Resource/Animation/asd.KeyframeAnimation_Imp.h"

#include "../../asd.Graphics_Imp.h"
#include "../../Resource/asd.ShaderCache.h"
#include "../../Resource/asd.NativeShader_Imp.h"
#include "../../Resource/asd.IndexBuffer_Imp.h"
#include "../../Resource/asd.MaterialPropertyBlock_Imp.h"
#include "../../Resource/asd.Shader3D_Imp.h"
#include "../../Resource/asd.Material3D_Imp.h"

#include "../../Shader/DX/3D/Lightweight_Model_Internal_VS.h"
#include "../../Shader/DX/3D/Lightweight_Model_Internal_PS.h"
#include "../../Shader/DX/3D/Model_Internal_VS.h"
#include "../../Shader/DX/3D/Model_Internal_PS.h"

#include "../../Shader/GL/3D/Lightweight_Model_Internal_VS.h"
#include "../../Shader/GL/3D/Lightweight_Model_Internal_PS.h"
#include "../../Shader/GL/3D/Model_Internal_VS.h"
#include "../../Shader/GL/3D/Model_Internal_PS.h"

#include "../../Command/asd.RenderingCommandHelper.h"

namespace asd
{
	static void ResetAnimationPlaying(std::vector <BoneProperty>& boneProps)
	{
		for (auto i = 0; i < boneProps.size(); i++)
		{
			boneProps[i].IsAnimationPlaying = false;
		}
	}

	static void SetAnimationPlaying(std::vector <BoneProperty>& boneProps, ModelObject3DAnimationCache& cache, float time)
	{
		for (size_t i = 0; i < cache.Pairs.size(); i++)
		{
			auto& pair = cache.Pairs[i];

			boneProps[pair.BoneIndex].IsAnimationPlaying = true;
		}
	}

	static void CalculateAnimation(std::vector <BoneProperty>& boneProps, ModelObject3DAnimationCache& cache, float time)
	{
		for (size_t i = 0; i < cache.Pairs.size(); i++)
		{
			auto& pair = cache.Pairs[i];

			auto a_ = (KeyframeAnimation_Imp*) pair.Anim;

			auto type = a_->GetTargetType();
			auto axis = a_->GetTargetAxis();
			auto bi = pair.BoneIndex;

			if (bi < 0) continue;
			auto value = a_->GetValue(time);

			ModelUtils::SetBoneValue(
				boneProps[bi].TempPosition[0],
				boneProps[bi].TempRotation,
				boneProps[bi].TempScale[0],
				type,
				axis,
				value
				);
		}
	}

	static void SetDefaultSRT(std::vector <BoneProperty>& boneProps, Deformer* deformer)
	{
		if (deformer == nullptr) return;
		auto d = (Deformer_Imp*) deformer;

		for (auto i = 0; i < d->GetBones().size(); i++)
		{
			auto& b = d->GetBones()[i];
			for (int32_t j = 0; j < 3; j++) boneProps[i].Position[j] = b.Translation[j];
			for (int32_t j = 0; j < 3; j++) boneProps[i].Scale[j] = b.Scaling[j];

			boneProps[i].RotY = Vector3DF(0, 1, 0);
			boneProps[i].RotZ = Vector3DF(0, 0, 1);
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
				matrixes[i] = boneProps[i].CalcMatrix();
			}
			else
			{
				matrixes[i] = b.LocalMat;
			}
		}
	}

	/**
		@brief	球面補間用関数
		@note
		http://marupeke296.com/DXG_No57_SheareLinearInterWithoutQu.html
		を参照。
	*/
	static Vector3DF SphereLinear(Vector3DF start, Vector3DF end, float t) 
	{
		const float epsilon = 0.001f;

		// 単位ベクトルが入力されること前提
		// start.Normalize();
		// end.Normalize();

		// 2ベクトル間の角度（鋭角側）
		float CosTh = Vector3DF::Dot(start, end);
		CosTh = Clamp(CosTh, 1.0f, -1.0f);

		float angle = acos(CosTh);

		// ほぼ変化なし
		if (abs(angle) < epsilon) return end;

		// sinθ
		float SinTh = sin(angle);

		// 補間係数
		float Ps = sin(angle * (1 - t));
		float Pe = sin(angle * t);

		auto ret = (start * Ps + end * Pe) / SinTh;

		// 一応正規化して球面線形補間に
		ret.Normalize();

		return ret;
	}

	void ModelObject3DAnimationCache::SetObjects(AnimationSource* source_, Deformer* deformer)
	{
		Pairs.clear();
		CurrentAnimationSource = source_;

		auto source = (AnimationSource_Imp*)source_;
		auto& animations = source->GetAnimations();
		auto d = (Deformer_Imp*) deformer;

		for (auto& a : animations)
		{
			auto a_ = (KeyframeAnimation_Imp*) a;

			auto type = a_->GetTargetType();
			auto axis = a_->GetTargetAxis();
			auto bi = d->GetBoneIndex(a_->GetTargetName());
		
			if (bi < 0) continue;

			Pair p;
			p.Anim = a;
			p.BoneIndex = bi;
			Pairs.push_back(p);
		}
	}

	BoneProperty::BoneProperty()
	{
		Position[0] = 0.0f;
		Position[1] = 0.0f;
		Position[2] = 0.0f;

		Scale[0] = 1.0f;
		Scale[1] = 1.0f;
		Scale[2] = 1.0f;

		IsAnimationPlaying = false;
	}


	Matrix44 BoneProperty::CalcMatrix()
	{
		Vector3DF vecX;
		vecX = Vector3DF::Cross(this->RotY, this->RotZ);
		vecX.Normalize();

		Matrix44 matR;

		RotY.Normalize();
		RotZ.Normalize();

		matR.Values[0][0] = vecX.X;
		matR.Values[1][0] = vecX.Y;
		matR.Values[2][0] = vecX.Z;

		matR.Values[0][1] = RotY.X;
		matR.Values[1][1] = RotY.Y;
		matR.Values[2][1] = RotY.Z;

		matR.Values[0][2] = RotZ.X;
		matR.Values[1][2] = RotZ.Y;
		matR.Values[2][2] = RotZ.Z;

		return ModelUtils::CalcMatrix(
			Position,
			matR,
			Scale);
	}

	Matrix44 BoneProperty::CalcRotationMatrix(RotationOrder rotationType)
	{
		return ModelUtils::CalcRotationMatrix(
			TempRotation,
			rotationType);
	}

	RenderedModelObject3DProxy::RenderedModelObject3DProxy(Graphics* graphics)
	{
		auto g = (Graphics_Imp*) graphics;

		std::vector<asd::VertexLayout> vl;
		vl.push_back(asd::VertexLayout("Position", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("Normal", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("Binormal", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("UV", asd::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(asd::VertexLayout("UVSub", asd::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(asd::VertexLayout("Color", asd::VertexLayoutFormat::R8G8B8A8_UNORM));
		vl.push_back(asd::VertexLayout("BoneWeights", asd::VertexLayoutFormat::R8G8B8A8_UNORM));
		vl.push_back(asd::VertexLayout("BoneIndexes", asd::VertexLayoutFormat::R8G8B8A8_UINT));
		vl.push_back(asd::VertexLayout("BoneIndexesOriginal", asd::VertexLayoutFormat::R8G8B8A8_UINT));

		{
			std::vector<asd::Macro> macro;
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
			std::vector<asd::Macro> macro;
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
			std::vector<asd::Macro> macro;
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

		cullingProxy.ProxyPtr = this;

		CullingObject = Culling3D::Object::Create();
		CullingObject->SetUserData(&cullingProxy);
	}

	RenderedModelObject3DProxy::~RenderedModelObject3DProxy()
	{
		Culling3D::SafeRelease(CullingObject);
	}

	void RenderedModelObject3DProxy::OnUpdateAsync()
	{
		if (calcAnimationOnProxy)
		{
			auto d = (Deformer_Imp*) m_deformer.get();

			// 初期値を設定する
			SetDefaultSRT(m_boneProps, m_deformer.get());
			//SetLocalMatrixes(m_matrixes, m_boneProps, m_deformer.get());

			// アニメーションを再生する。
			for (int32_t i = 0; i < AnimationCount; i++)
			{
				if (m_animationPlaying[i].size() == 0) continue;

				// ボーンの設定をリセットする
				ResetAnimationPlaying(m_boneProps);

				// アニメーションが再生されているか取得する
				for (auto& anim_ : m_animationPlaying[i])
				{
					auto anim = anim_.Animation.get();

					if (cachedAnimationDeformer != m_deformer.get())
					{
						animationCaches.clear();
					}
					cachedAnimationDeformer = m_deformer.get();

					auto src = anim->GetSource().get();
					if (animationCaches.count(src) == 0)
					{
						ModelObject3DAnimationCache cache;
						cache.SetObjects(src, m_deformer.get());
						animationCaches[src] = cache;
					}

					SetAnimationPlaying(m_boneProps, animationCaches[src], 0);
				}

				for (auto& b : m_boneProps)
				{
					for (int32_t j = 0; j < 3; j++) b.TempPosition[1][j] = 0.0f;
					for (int32_t j = 0; j < 3; j++) b.TempScale[1][j] = 0.0f;
					b.TempRotY = Vector3DF(0, 1, 0);
					b.TempRotZ = Vector3DF(0, 0, 1);
					b.TempRotWeight = 0.0f;
				}

				for (int32_t bi = 0; bi < m_animationPlaying[i].size(); bi++)
				{
					auto& anim_ = m_animationPlaying[i][bi];

					for (auto& b : m_boneProps)
					{
						for (int32_t j = 0; j < 3; j++) b.TempPosition[0][j] = 0.0f;
						for (int32_t j = 0; j < 4; j++) b.TempRotation[j] = 0.0f;
						for (int32_t j = 0; j < 3; j++) b.TempScale[0][j] = 0.0f;
					}

					auto anim = anim_.Animation.get();
					auto loop = anim->GetIsLoopingMode();
					auto src = (AnimationSource_Imp*) anim->GetSource().get();
					auto length = src->GetLength();
					auto time = anim_.Time;
					if (loop)
					{
						time = fmodf(time, length);
					}

					SetAnimationPlaying(m_boneProps, animationCaches[src], time);

					CalculateAnimation(m_boneProps, animationCaches[src], time);
					
					for (auto b_ = 0; b_ < m_boneProps.size(); b_++)
					{
						auto& b = d->GetBones()[b_];
						auto& bp = m_boneProps[b_];
						auto matR = bp.CalcRotationMatrix(b.RotationType);
						
						for (int32_t j = 0; j < 3; j++) bp.TempPosition[1][j] += bp.TempPosition[0][j] * anim_.CurrentWeight;
						for (int32_t j = 0; j < 3; j++) bp.TempScale[1][j] += bp.TempScale[0][j] * anim_.CurrentWeight;

						auto vecRY = Vector3DF(matR.Values[0][1], matR.Values[1][1], matR.Values[2][1]);
						auto vecRZ = Vector3DF(matR.Values[0][2], matR.Values[1][2], matR.Values[2][2]);

						if (anim_.CurrentWeight == 0.0f)
						{
						}
						else
						{
							auto ratio = (anim_.CurrentWeight) / (anim_.CurrentWeight + bp.TempRotWeight);

							if (ratio == 1.0f)
							{
								bp.TempRotY = vecRY;
								bp.TempRotZ = vecRZ;
							}
							else
							{
								bp.TempRotY = SphereLinear(bp.TempRotY, vecRY, ratio);
								bp.TempRotZ = SphereLinear(bp.TempRotZ, vecRZ, ratio);
							}
							
							bp.TempRotWeight += anim_.CurrentWeight;
						}
					}
				}

				for (auto& b : m_boneProps)
				{
					if (!b.IsAnimationPlaying) continue;

					if (m_animationWeight[i] == 1.0f)
					{
						for (int32_t j = 0; j < 3; j++) b.Position[j] = b.Position[j] * (1.0f - m_animationWeight[i]) + b.TempPosition[1][j] * m_animationWeight[i];
						for (int32_t j = 0; j < 3; j++) b.Scale[j] = b.Scale[j] * (1.0f - m_animationWeight[i]) + b.TempScale[1][j] * m_animationWeight[i];

						b.RotY = SphereLinear(b.RotY, b.TempRotY, m_animationWeight[i]);
						b.RotZ = SphereLinear(b.RotZ, b.TempRotZ, m_animationWeight[i]);
					}
					else
					{
						for (int32_t j = 0; j < 3; j++) b.Position[j] = b.TempPosition[1][j];
						for (int32_t j = 0; j < 3; j++) b.Scale[j] = b.TempScale[1][j];
						b.RotY = b.TempRotY;
						b.RotZ = b.TempRotZ;
					}
				}
			}

			if (m_deformer.get() != nullptr)
			{
				CalclateBoneMatrices(m_matrixes, m_boneProps, m_deformer.get());

				ModelUtils::CalculateBoneMatrixes(
					m_matrixes,
					((Deformer_Imp*) m_deformer.get())->GetBones(),
					m_matrixes);
			}	
		}
	}

	void RenderedModelObject3DProxy::OnAdded(Renderer3DProxy* renderer)
	{
		renderer->CullingWorld->AddObject(CullingObject);
	}

	void RenderedModelObject3DProxy::OnRemoving(Renderer3DProxy* renderer)
	{
		renderer->CullingWorld->RemoveObject(CullingObject);
	}

	void RenderedModelObject3DProxy::Rendering(RenderingCommandHelper* helper, RenderingProperty& prop)
	{
		using h = RenderingCommandHelper;
		shaderConstants.clear();

		auto lightDirection = prop.DirectionalLightDirection;
		Vector3DF lightColor = prop.DirectionalLightColor;
		Vector3DF groudLColor(prop.GroundLightColor.R / 255.0f, prop.GroundLightColor.G / 255.0f, prop.GroundLightColor.B / 255.0f);
		Vector3DF skyLColor(prop.SkyLightColor.R / 255.0f, prop.SkyLightColor.G / 255.0f, prop.SkyLightColor.B / 255.0f);
		Matrix44 matM[32];

		
		auto& matrices = m_matrixes;
		 
		for (int32_t currentMeshIndex = 0; currentMeshIndex < m_meshes.size(); currentMeshIndex++)
		{
			auto mesh_root = (Mesh_Imp*) m_meshes[currentMeshIndex].get();

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
						matM[i].SetIdentity();
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

						std::shared_ptr<asd::NativeShader_Imp> shader;

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
							state.AlphaBlendState = AlphaBlendMode::Opacity;

							helper->DrawWithPtr(
								fOffset,
								fCount,
								mesh.VertexBufferPtr.get(),
								mesh.IndexBufferPtr.get(),
								shader.get(),
								state,
								shaderConstants.data(),
								shaderConstants.size());

							//helper->DrawWithPtr(
							//	mesh.IndexBufferPtr->GetCount() / 3, 
							//	mesh.VertexBufferPtr.get(), 
							//	mesh.IndexBufferPtr.get(), 
							//	shader.get(), 
							//	state,
							//	shaderConstants.data(), 
							//	shaderConstants.size());
						}
						else
						{
							asd::Texture2D* colorTexture = prop.DummyTextureWhite.get();
							asd::Texture2D* normalTexture = prop.DummyTextureNormal.get();
							asd::Texture2D* metalnessTexture = prop.DummyTextureBlack.get();
							asd::Texture2D* smoothnessTexture = prop.DummyTextureBlack.get();

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

									if (material->MetalnessTexture != nullptr)
									{
										metalnessTexture = material->MetalnessTexture.get();
									}

									if (material->SmoothnessTexture != nullptr)
									{
										smoothnessTexture = material->SmoothnessTexture.get();
									}
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

							RenderState state;
							state.DepthTest = true;
							state.DepthWrite = true;
							state.Culling = CullingType::Front;
							state.AlphaBlendState = AlphaBlendMode::Opacity;

							helper->DrawWithPtr(
								fOffset,
								fCount,
								mesh.VertexBufferPtr.get(),
								mesh.IndexBufferPtr.get(),
								shader.get(),
								state,
								shaderConstants.data(),
								shaderConstants.size());

							//helper->DrawWithPtr(
							//	mesh.IndexBufferPtr->GetCount() / 3, 
							//	mesh.VertexBufferPtr.get(), 
							//	mesh.IndexBufferPtr.get(), 
							//	shader.get(), 
							//	state,
							//	shaderConstants.data(), 
							//	shaderConstants.size());
						}

						shaderConstants.clear();
						fOffset += fCount;
					}
				}
			}
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

	bool RenderedModelObject3D::GetIsAnimationPlaying(int32_t index)
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

	void RenderedModelObject3D::OnApplyingNextSRT()
	{
		auto pos = this->GetPosition();
		proxy->CullingObject->SetPosition(Culling3D::Vector3DF(pos.X, pos.Y, pos.Z));

		// 仮の数値
		proxy->CullingObject->ChangeIntoSphere(200.0f);
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
			auto d = (Deformer_Imp*) m_deformer.get();

			// 初期値を設定する
			SetDefaultSRT(m_boneProps, m_deformer.get());
			//SetLocalMatrixes(m_matrixes, m_boneProps, m_deformer.get());

			// アニメーションを再生する。
			for (int32_t i = 0; i < AnimationCount; i++)
			{
				if (m_animationPlaying[i].size() == 0) continue;

				// ボーンの設定をリセットする
				ResetAnimationPlaying(m_boneProps);

				// アニメーションが再生されているか取得する
				for (auto& anim_ : m_animationPlaying[i])
				{
					auto anim = anim_.Animation.get();

					if (cachedAnimationDeformer != m_deformer.get())
					{
						animationCaches.clear();
					}
					cachedAnimationDeformer = m_deformer.get();

					auto src = anim->GetSource().get();
					if (animationCaches.count(src) == 0)
					{
						ModelObject3DAnimationCache cache;
						cache.SetObjects(src, m_deformer.get());
						animationCaches[src] = cache;
					}

					SetAnimationPlaying(m_boneProps, animationCaches[src], 0);
				}

				for (auto& b : m_boneProps)
				{
					for (int32_t j = 0; j < 3; j++) b.TempPosition[1][j] = 0.0f;
					for (int32_t j = 0; j < 3; j++) b.TempScale[1][j] = 0.0f;
					b.TempRotY = Vector3DF(0, 1, 0);
					b.TempRotZ = Vector3DF(0, 0, 1);
					b.TempRotWeight = 0.0f;
				}

				for (int32_t bi = 0; bi < m_animationPlaying[i].size(); bi++)
				{
					auto& anim_ = m_animationPlaying[i][bi];

					for (auto& b : m_boneProps)
					{
						for (int32_t j = 0; j < 3; j++) b.TempPosition[0][j] = 0.0f;
						for (int32_t j = 0; j < 4; j++) b.TempRotation[j] = 0.0f;
						for (int32_t j = 0; j < 3; j++) b.TempScale[0][j] = 0.0f;
					}

					auto anim = anim_.Animation.get();
					auto loop = anim->GetIsLoopingMode();
					auto src = (AnimationSource_Imp*) anim->GetSource().get();
					auto length = src->GetLength();
					auto time = anim_.Time;
					if (loop)
					{
						time = fmodf(time, length);
					}

					SetAnimationPlaying(m_boneProps, animationCaches[src], time);

					CalculateAnimation(m_boneProps, animationCaches[src], time);

					for (auto b_ = 0; b_ < m_boneProps.size(); b_++)
					{
						auto& b = d->GetBones()[b_];
						auto& bp = m_boneProps[b_];
						auto matR = bp.CalcRotationMatrix(b.RotationType);

						for (int32_t j = 0; j < 3; j++) bp.TempPosition[1][j] += bp.TempPosition[0][j] * anim_.CurrentWeight;
						for (int32_t j = 0; j < 3; j++) bp.TempScale[1][j] += bp.TempScale[0][j] * anim_.CurrentWeight;

						auto vecRY = Vector3DF(matR.Values[0][1], matR.Values[1][1], matR.Values[2][1]);
						auto vecRZ = Vector3DF(matR.Values[0][2], matR.Values[1][2], matR.Values[2][2]);

						if (anim_.CurrentWeight == 0.0f)
						{
						}
						else
						{
							auto ratio = (anim_.CurrentWeight) / (anim_.CurrentWeight + bp.TempRotWeight);

							if (ratio == 1.0f)
							{
								bp.TempRotY = vecRY;
								bp.TempRotZ = vecRZ;
							}
							else
							{
								bp.TempRotY = SphereLinear(bp.TempRotY, vecRY, ratio);
								bp.TempRotZ = SphereLinear(bp.TempRotZ, vecRZ, ratio);
							}

							bp.TempRotWeight += anim_.CurrentWeight;
						}
					}
				}

				for (auto& b : m_boneProps)
				{
					if (!b.IsAnimationPlaying) continue;

					if (m_animationWeight[i] == 1.0f)
					{
						for (int32_t j = 0; j < 3; j++) b.Position[j] = b.Position[j] * (1.0f - m_animationWeight[i]) + b.TempPosition[1][j] * m_animationWeight[i];
						for (int32_t j = 0; j < 3; j++) b.Scale[j] = b.Scale[j] * (1.0f - m_animationWeight[i]) + b.TempScale[1][j] * m_animationWeight[i];

						b.RotY = SphereLinear(b.RotY, b.TempRotY, m_animationWeight[i]);
						b.RotZ = SphereLinear(b.RotZ, b.TempRotZ, m_animationWeight[i]);
					}
					else
					{
						for (int32_t j = 0; j < 3; j++) b.Position[j] = b.TempPosition[1][j];
						for (int32_t j = 0; j < 3; j++) b.Scale[j] = b.TempScale[1][j];
						b.RotY = b.TempRotY;
						b.RotZ = b.TempRotZ;
					}
				}
			}

			if (m_deformer.get() != nullptr)
			{
				CalclateBoneMatrices(m_matrixes, m_boneProps, m_deformer.get());

				ModelUtils::CalculateBoneMatrixes(
					m_matrixes,
					((Deformer_Imp*) m_deformer.get())->GetBones(),
					m_matrixes);
			}

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
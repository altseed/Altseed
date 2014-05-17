
#include "ace.RenderedModelObject3D.h"
#include "ace.Mesh_Imp.h"
#include "ace.Deformer_Imp.h"
#include "ace.Model_Imp.h"
#include "ace.Renderer3D.h"

#include "../Animation/ace.AnimationClip_Imp.h"
#include "../Animation/ace.AnimationSource_Imp.h"
#include "../Animation/ace.KeyframeAnimation_Imp.h"

#include "../ace.Graphics_Imp.h"
#include "../Resource/ace.ShaderCache.h"
#include "../Resource/ace.NativeShader_Imp.h"
#include "../Resource/ace.RenderState_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"

#include "../Shader/DX/3D/Lightweight_Model_Internal_VS.h"
#include "../Shader/DX/3D/Lightweight_Model_Internal_PS.h"
#include "../Shader/DX/3D/Model_Internal_VS.h"
#include "../Shader/DX/3D/Model_Internal_PS.h"

#include "../Shader/GL/3D/Lightweight_Model_Internal_VS.h"
#include "../Shader/GL/3D/Lightweight_Model_Internal_PS.h"
#include "../Shader/GL/3D/Model_Internal_VS.h"
#include "../Shader/GL/3D/Model_Internal_PS.h"

#include <cstddef>

namespace ace
{
	RenderedModelObject3D::BoneProperty::BoneProperty()
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


	Matrix44 RenderedModelObject3D::BoneProperty::CalcMatrix(eRotationOrder rotationType)
	{
		return ModelUtils::CalcMatrix(
			Position,
			Rotation,
			Scale,
			rotationType);
	}

	RenderedModelObject3D::MeshGroup::MeshGroup()
		: m_deformer(nullptr)
	{
	}

	RenderedModelObject3D::MeshGroup::~MeshGroup()
	{
		for (auto& mesh : m_meshes)
		{
			mesh->Release();
		}
		m_meshes.clear();

		SafeRelease(m_deformer);
	}

	void RenderedModelObject3D::MeshGroup::Flip(AnimationClip* animationClip, int32_t time)
	{
		CalculateAnimation(animationClip, time);

		CalclateBoneMatrices();

		// コピー
		if (m_matrixes_fr.size() != m_matrixes.size())
		{
			m_matrixes_fr.resize(m_matrixes.size());
		}

		std::copy(m_matrixes.begin(), m_matrixes.end(), m_matrixes_fr.begin());
	}


	void RenderedModelObject3D::MeshGroup::CalculateAnimation(AnimationClip* animationClip, int32_t time)
	{
		if (animationClip == nullptr) return;

		auto source = (AnimationSource_Imp*) animationClip->GetSource().get();
		auto& animations = source->GetAnimations();

		for (auto& a : animations)
		{
			auto a_ = (KeyframeAnimation_Imp*) a;

			auto type = a_->GetTargetType();
			auto axis = a_->GetTargetAxis();
			auto bi = m_deformer->GetBoneIndex(a_->GetTargetName());

			if (bi < 0) continue;
			auto value = a_->GetValue(time);

			ModelUtils::SetBoneValue(
				m_boneProps[bi].Position,
				m_boneProps[bi].Rotation,
				m_boneProps[bi].Scale,
				type,
				axis,
				value);
		}
	}

	void RenderedModelObject3D::MeshGroup::CalclateBoneMatrices()
	{
		if (m_deformer == nullptr) return;

		for (auto i = 0; i < m_deformer->GetBones().size(); i++)
		{
			auto& b = m_deformer->GetBones()[i];
			m_matrixes[i] = m_boneProps[i].CalcMatrix(b.RotationType);
		}

		ModelUtils::CalculateBoneMatrixes(
			m_matrixes,
			m_deformer->GetBones(),
			m_matrixes);
	}

	void RenderedModelObject3D::MeshGroup::AddMesh(Mesh_Imp* mesh)
	{
		SafeAddRef(mesh);
		m_meshes.push_back(mesh);
	}

	void RenderedModelObject3D::MeshGroup::SetDeformer(Deformer_Imp* deformer)
	{
		SafeSubstitute(m_deformer, deformer);

		if (deformer != nullptr)
		{
			m_matrixes.resize(m_deformer->GetBones().size());
			m_boneProps.resize(m_deformer->GetBones().size());

			for (int32_t i = 0; i < m_boneProps.size(); i++)
			{
				m_boneProps[i] = BoneProperty();
			}
		}
		else
		{
			m_matrixes.resize(0);
			m_boneProps.resize(0);
		}
	}

	RenderedModelObject3D::RenderedModelObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
		, m_animationPlaying(nullptr)
		, m_animationTime(0)
	{
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
			std::vector<ace::ConstantBufferInformation> constantBuffers_vs;
			constantBuffers_vs.resize(7);
			constantBuffers_vs[0].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY;
			constantBuffers_vs[0].Name = std::string("matM");
			constantBuffers_vs[0].Offset = 0;
			constantBuffers_vs[0].Count = 32;

			constantBuffers_vs[1].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
			constantBuffers_vs[1].Name = std::string("matC");
			constantBuffers_vs[1].Offset = offsetof(VertexConstantBufferLightweight, matC);

			constantBuffers_vs[2].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
			constantBuffers_vs[2].Name = std::string("matP");
			constantBuffers_vs[2].Offset = offsetof(VertexConstantBufferLightweight, matP);

			constantBuffers_vs[3].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers_vs[3].Name = std::string("directionalLightDirection");
			constantBuffers_vs[3].Offset = offsetof(VertexConstantBufferLightweight, directionalLightDirection);

			constantBuffers_vs[4].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers_vs[4].Name = std::string("directionalLightColor");
			constantBuffers_vs[4].Offset = offsetof(VertexConstantBufferLightweight, directionalLightColor);

			constantBuffers_vs[5].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers_vs[5].Name = std::string("skyLightColor");
			constantBuffers_vs[5].Offset = offsetof(VertexConstantBufferLightweight, skyLightColor);

			constantBuffers_vs[6].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers_vs[6].Name = std::string("groundLightColor");
			constantBuffers_vs[6].Offset = offsetof(VertexConstantBufferLightweight, groundLightColor);

			std::vector<ace::Macro> macro;
			if (GetGraphics()->GetGraphicsType() == GRAPHICS_TYPE_GL)
			{
				m_shaderLightweight = GetGraphics()->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.Lightweight").c_str(),
					lightweight_model_internal_vs_gl,
					lightweight_model_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderLightweight = GetGraphics()->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.Lightweight").c_str(),
					lightweight_model_internal_vs_dx,
					lightweight_model_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderLightweight != nullptr);
			m_shaderLightweight->CreateVertexConstantBuffer<VertexConstantBufferLightweight>(constantBuffers_vs);
		}

		{
			std::vector<ace::ConstantBufferInformation> constantBuffers_vs;
			constantBuffers_vs.resize(4);
			constantBuffers_vs[0].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY;
			constantBuffers_vs[0].Name = std::string("matM");
			constantBuffers_vs[0].Offset = 0;
			constantBuffers_vs[0].Count = 32;

			constantBuffers_vs[1].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
			constantBuffers_vs[1].Name = std::string("matC");
			constantBuffers_vs[1].Offset = offsetof(VertexConstantBufferDeferredRendering, matC);

			constantBuffers_vs[2].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
			constantBuffers_vs[2].Name = std::string("matP");
			constantBuffers_vs[2].Offset = offsetof(VertexConstantBufferDeferredRendering, matP);

			constantBuffers_vs[3].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers_vs[3].Name = std::string("depthParams");
			constantBuffers_vs[3].Offset = offsetof(VertexConstantBufferDeferredRendering, depthParams);

			std::vector<ace::Macro> macro;
			if (GetGraphics()->GetGraphicsType() == GRAPHICS_TYPE_GL)
			{
				m_shaderDF = GetGraphics()->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.DF").c_str(),
					model_internal_vs_gl,
					model_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderDF = GetGraphics()->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.DF").c_str(),
					model_internal_vs_dx,
					model_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderDF != nullptr);
			m_shaderDF->CreateVertexConstantBuffer<VertexConstantBufferDeferredRendering>(constantBuffers_vs);
		}

		{
			std::vector<ace::ConstantBufferInformation> constantBuffers_vs;
			constantBuffers_vs.resize(4);
			constantBuffers_vs[0].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY;
			constantBuffers_vs[0].Name = std::string("matM");
			constantBuffers_vs[0].Offset = 0;
			constantBuffers_vs[0].Count = 32;

			constantBuffers_vs[1].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
			constantBuffers_vs[1].Name = std::string("matC");
			constantBuffers_vs[1].Offset = offsetof(VertexConstantBufferDeferredRendering, matC);

			constantBuffers_vs[2].Format = ace::CONSTANT_BUFFER_FORMAT_MATRIX44;
			constantBuffers_vs[2].Name = std::string("matP");
			constantBuffers_vs[2].Offset = offsetof(VertexConstantBufferDeferredRendering, matP);

			constantBuffers_vs[3].Format = ace::CONSTANT_BUFFER_FORMAT_FLOAT3;
			constantBuffers_vs[3].Name = std::string("depthParams");
			constantBuffers_vs[3].Offset = offsetof(VertexConstantBufferDeferredRendering, depthParams);

			std::vector<ace::Macro> macro;
			macro.push_back(Macro("EXPORT_DEPTH", "1"));

			if (GetGraphics()->GetGraphicsType() == GRAPHICS_TYPE_GL)
			{
				m_shaderDF_ND = GetGraphics()->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.DF.ND").c_str(),
					model_internal_vs_gl,
					model_internal_ps_gl,
					vl,
					macro);
			}
			else
			{
				m_shaderDF_ND = GetGraphics()->GetShaderCache()->CreateFromCode(
					ToAString("Internal.ModelObject3D.DF.ND").c_str(),
					model_internal_vs_dx,
					model_internal_ps_dx,
					vl,
					macro);
			}

			assert(m_shaderDF_ND != nullptr);
			m_shaderDF_ND->CreateVertexConstantBuffer<VertexConstantBufferDeferredRendering>(constantBuffers_vs);
		}
	}

	RenderedModelObject3D::~RenderedModelObject3D()
	{
		m_meshGroups_fr.clear();

		m_meshGroups.clear();

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

	void RenderedModelObject3D::AddMeshGroup()
	{
		m_meshGroups.push_back(std::make_shared<MeshGroup>());
	}

	int32_t RenderedModelObject3D::GetMeshGroupCount()
	{
		return m_meshGroups.size();
	}

	void RenderedModelObject3D::AddMesh(int32_t meshGroupIndex, Mesh* mesh)
	{
		if (meshGroupIndex >= GetMeshGroupCount()) return;
		m_meshGroups[meshGroupIndex]->AddMesh((Mesh_Imp*)mesh);
	}

	void  RenderedModelObject3D::SetDeformer(int32_t meshGroupIndex, Deformer* deformer)
	{
		if (meshGroupIndex >= GetMeshGroupCount()) return;
		m_meshGroups[meshGroupIndex]->SetDeformer((Deformer_Imp*) deformer);
	}

	void RenderedModelObject3D::UnloadModel()
	{
		// 描画中以外のオブジェクトをリセット

		m_meshGroups.clear();

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
		for (auto& mg : m_model->GetMeshGroups())
		{
			AddMeshGroup();

			for (auto& m : mg->Meshes)
			{
				AddMesh(index, m);
			}
			SetDeformer(index, mg->Deformer_);
			index++;
		}

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

	void RenderedModelObject3D::AddAnimationClip(const achar* name, AnimationClip* animationClip)
	{
		if (animationClip == nullptr) return;

		if (m_animationClips.find(name) == m_animationClips.end())
		{
			SafeAddRef(animationClip);
			m_animationClips[name] = animationClip;
		}
	}

	void RenderedModelObject3D::PlayAnimation(const achar* name)
	{
		auto it = m_animationClips.find(name);
		if (it == m_animationClips.end()) return;

		m_animationPlaying = (*it).second;
		m_animationTime = 0;
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

	void RenderedModelObject3D::Flip()
	{
		RenderedObject3D::Flip();

		for (auto& g : m_meshGroups)
		{
			g->Flip(m_animationPlaying, m_animationTime);
		}

		// アニメーションの適用
		if (m_animationPlaying != nullptr)
		{
			m_animationTime++;
		}

		m_meshGroups_fr.clear();
		m_meshGroups_fr.resize(m_meshGroups.size());
		std::copy(m_meshGroups.begin(), m_meshGroups.end(), m_meshGroups_fr.begin());
	}

	void RenderedModelObject3D::Rendering(RenderingProperty& prop)
	{
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

		Matrix44* matM = nullptr;
		Matrix44* matC = nullptr;
		Matrix44* matP = nullptr;
		{
			if (prop.IsLightweightMode)
			{
				auto& vbuf = shader->GetVertexConstantBuffer<VertexConstantBufferLightweight>();
				matM = vbuf.matM;
				matC = &vbuf.matC;
				matP = &vbuf.matP;
			}
			else
			{
				auto& vbuf = shader->GetVertexConstantBuffer<VertexConstantBufferDeferredRendering>();
				matM = vbuf.matM;
				matC = &vbuf.matC;
				matP = &vbuf.matP;
			}

			*matC = prop.CameraMatrix;
			*matP = prop.ProjectionMatrix;
		}

		if (prop.IsLightweightMode)
		{
			auto& vbuf = shader->GetVertexConstantBuffer<VertexConstantBufferLightweight>();

			vbuf.directionalLightDirection = prop.DirectionalLightDirection;
			vbuf.directionalLightColor.X = prop.DirectionalLightColor.R / 255.0f;
			vbuf.directionalLightColor.Y = prop.DirectionalLightColor.G / 255.0f;
			vbuf.directionalLightColor.Z = prop.DirectionalLightColor.B / 255.0f;
			vbuf.groundLightColor.X = prop.GroundLightColor.R / 255.0f;
			vbuf.groundLightColor.Y = prop.GroundLightColor.G / 255.0f;
			vbuf.groundLightColor.Z = prop.GroundLightColor.B / 255.0f;
			vbuf.skyLightColor.X = prop.SkyLightColor.R / 255.0f;
			vbuf.skyLightColor.Y = prop.SkyLightColor.G / 255.0f;
			vbuf.skyLightColor.Z = prop.SkyLightColor.B / 255.0f;
		}
		else
		{
			auto& vbuf = shader->GetVertexConstantBuffer<VertexConstantBufferDeferredRendering>();
			vbuf.depthParams.X = prop.DepthRange;
			vbuf.depthParams.Y = prop.ZFar;
			vbuf.depthParams.Z = prop.ZNear;
		}

		for (auto& g : m_meshGroups_fr)
		{
			auto& matrices = g->m_matrixes_fr;

			for (auto& mesh : g->GetMeshes())
			{
				// 有効チェック
				if (mesh->GetIndexBuffer() == nullptr) continue;

				// 行列計算
				if (matrices.size() > 0)
				{
					// ボーンあり
					for (int32_t i = 0; i < Min(32, matrices.size()); i++)
					{
						matM[i].SetIndentity();
						Matrix44::Mul(matM[i], GetLocalMatrix_RT(), matrices[i]);
					}
				}
				else
				{
					// ボーンなし
					matM[0] = GetLocalMatrix_RT();
					for (int32_t i = 1; i < 32; i++)
					{
						matM[i] = matM[0];
					}
				}

				auto& boneOffsets = mesh->GetBoneOffsets();
				auto& materialOffsets = mesh->GetMaterialOffsets();

				{
					// 設定がある場合
					auto mIndex = 0;
					auto bIndex = 0;

					auto fOffset = 0;
					auto fCount = 0;

					auto bFCount = 0;
					if (boneOffsets.size() > 0)
					{
						bFCount = boneOffsets[bIndex].FaceOffset;
					}
					else
					{
						bFCount = mesh->GetIndexBuffer()->GetCount() / 3;
					}

					auto mFCount = 0;
					if (materialOffsets.size() > 0)
					{
						mFCount = materialOffsets[mIndex].FaceOffset;
					}
					else
					{
						mFCount = mesh->GetIndexBuffer()->GetCount() / 3;
					}
					
					while (fCount < mesh->GetIndexBuffer()->GetCount() / 3)
					{
						fCount = Min(bFCount, mFCount) - fOffset;
						if (fCount == 0) break;

						Mesh_Imp::Material* material = nullptr;
						if (materialOffsets.size() > 0)
						{
							material = mesh->GetMaterial(materialOffsets[mIndex].MaterialIndex);
						}

						if (material != nullptr)
						{
							if (material->ColorTexture != nullptr)
							{
								shader->SetTexture("g_colorTexture", material->ColorTexture, 0);
							}
							else
							{
								shader->SetTexture("g_colorTexture", m_renderer->GetDummyTextureWhite().get(), 0);
							}

							if (prop.IsLightweightMode)
							{
								if (material->NormalTexture != nullptr)
								{
									shader->SetTexture("g_normalTexture", material->NormalTexture, 1);
								}
								else
								{
									shader->SetTexture("g_normalTexture", m_renderer->GetDummyTextureNormal().get(), 1);
								}

								if (material->SpecularTexture != nullptr)
								{
									shader->SetTexture("g_specularTexture", material->SpecularTexture, 2);
								}
								else
								{
									shader->SetTexture("g_specularTexture", m_renderer->GetDummyTextureBlack().get(), 2);
								}
							}
						}
						else
						{		
							shader->SetTexture("g_colorTexture", m_renderer->GetDummyTextureWhite().get(), 0);
							shader->SetTexture("g_normalTexture", m_renderer->GetDummyTextureNormal().get(), 1);
							shader->SetTexture("g_specularTexture", m_renderer->GetDummyTextureBlack().get(), 2);
						}

						GetGraphics()->SetVertexBuffer(mesh->GetVertexBuffer().get());
						GetGraphics()->SetIndexBuffer(mesh->GetIndexBuffer().get());
						GetGraphics()->SetShader(shader.get());

						auto& state = GetGraphics()->GetRenderState()->Push();
						state.DepthTest = true;
						state.DepthWrite = true;
						state.CullingType = eCullingType::CULLING_FRONT;
						state.AlphaBlend = eAlphaBlend::ALPHA_BLEND_OPACITY;

						GetGraphics()->GetRenderState()->Update(false);

						GetGraphics()->DrawPolygon(mesh->GetIndexBuffer()->GetCount() / 3);

						GetGraphics()->GetRenderState()->Pop();

						if (fCount + fOffset == bFCount && boneOffsets.size() > bIndex)
						{
							bFCount += boneOffsets[bIndex].FaceOffset;
							bIndex++;
						}

						if (fCount + fOffset == mFCount && materialOffsets.size() > mIndex)
						{
							mFCount += materialOffsets[mIndex].FaceOffset;
							mIndex++;
						}

						fOffset += fCount;
					}
				}
			}
		}
	}
}
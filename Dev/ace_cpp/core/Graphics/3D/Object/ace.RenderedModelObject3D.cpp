
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

#include "../../Shader/DX/3D/Lightweight_Model_Internal_VS.h"
#include "../../Shader/DX/3D/Lightweight_Model_Internal_PS.h"
#include "../../Shader/DX/3D/Model_Internal_VS.h"
#include "../../Shader/DX/3D/Model_Internal_PS.h"

#include "../../Shader/GL/3D/Lightweight_Model_Internal_VS.h"
#include "../../Shader/GL/3D/Lightweight_Model_Internal_PS.h"
#include "../../Shader/GL/3D/Model_Internal_VS.h"
#include "../../Shader/GL/3D/Model_Internal_PS.h"

#include "../../Command/ace.RenderingCommandHelper.h"

#include <cstddef>

namespace ace
{
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
			//m_shaderLightweight->CreateVertexConstantBuffer<VertexConstantBufferLightweight>(constantBuffers_vs);
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
			//m_shaderDF->CreateVertexConstantBuffer<VertexConstantBufferDeferredRendering>(constantBuffers_vs);
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
			//m_shaderDF_ND->CreateVertexConstantBuffer<VertexConstantBufferDeferredRendering>(constantBuffers_vs);
		}

	}

	RenderedModelObject3DProxy::~RenderedModelObject3DProxy()
	{

	}

	void RenderedModelObject3DProxy::Rendering(RenderingCommandHelper* helper,RenderingProperty& prop)
	{
		using h = RenderingCommandHelper;
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

		//Matrix44* matM = nullptr;
		//Matrix44* matC = nullptr;
		//Matrix44* matP = nullptr;
		{
			/*
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
			*/
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matC", prop.CameraMatrix));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matP", prop.ProjectionMatrix));

		}

		if (prop.IsLightweightMode)
		{
			/*
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
			*/

			auto direction = prop.DirectionalLightDirection;
			Vector3DF lightColor(prop.DirectionalLightColor.R / 255.0f, prop.DirectionalLightColor.G / 255.0f, prop.DirectionalLightColor.B / 255.0f);
			Vector3DF groudLColor(prop.GroundLightColor.R / 255.0f, prop.GroundLightColor.G / 255.0f, prop.GroundLightColor.B / 255.0f);
			Vector3DF skyLColor(prop.SkyLightColor.R / 255.0f, prop.SkyLightColor.G / 255.0f, prop.SkyLightColor.B / 255.0f);

			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "directionalLightDirection", direction));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "directionalLightColor", lightColor));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "skyLightColor", skyLColor));
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "groundLightColor", groudLColor));

		}
		else
		{
			/*
			auto& vbuf = shader->GetVertexConstantBuffer<VertexConstantBufferDeferredRendering>();
			vbuf.depthParams.X = prop.DepthRange;
			vbuf.depthParams.Y = prop.ZFar;
			vbuf.depthParams.Z = prop.ZNear;
			*/
			Vector3DF depthParams(prop.DepthRange, prop.ZFar, prop.ZNear);
			shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "depthParams", depthParams));
		}

		{
			auto& matrices = m_matrixes_rt;

			for (auto& mesh_ : m_meshes_rt)
			{
				auto mesh_root = (Mesh_Imp*) mesh_.get();

				for (auto& mesh : mesh_root->GetDvidedMeshes())
				{
					// 有効チェック
					if (mesh.IndexBufferPtr == nullptr) continue;

					auto& boneConnectors = mesh.BoneConnectors;

					Matrix44 matM[32];
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

					shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "matM", h::Array<Matrix44>(matM, 32)));

					auto& materialOffsets = mesh.MaterialOffsets;

					{
						// 設定がある場合
						auto mIndex = 0;
						auto fOffset = 0;
						auto fCount = 0;
						auto mFCount = 0;

						Mesh_Imp::Material* material = nullptr;

						while (fCount < mesh.IndexBufferPtr->GetCount() / 3)
						{
							if (materialOffsets.size() > 0)
							{
								if (fOffset == mFCount && materialOffsets.size() > mIndex)
								{
									mFCount += materialOffsets[mIndex].FaceOffset;
									material = mesh_root->GetMaterial(materialOffsets[mIndex].MaterialIndex);
									mIndex++;
								}
							}
							else
							{
								mFCount = mesh.IndexBufferPtr->GetCount() / 3;
							}

							fCount = mFCount - fOffset;
							if (fCount == 0) break;

							ace::Texture2D* colorTexture = prop.DummyTextureWhite.get();
							ace::Texture2D* normalTexture = prop.DummyTextureNormal.get();
							ace::Texture2D* specularTexture = prop.DummyTextureBlack.get();

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
								}
							}
							
							shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_colorTexture",
								h::Texture2DPair(colorTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

							shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_normalTexture",
								h::Texture2DPair(normalTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));

							shaderConstants.push_back(helper->CreateConstantValue(shader.get(), "g_specularTexture",
								h::Texture2DPair(specularTexture, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)));


							RenderState state;
							state.DepthTest = true;
							state.DepthWrite = true;
							state.CullingType = eCullingType::CULLING_FRONT;
							state.AlphaBlendState = AlphaBlend::Opacity;

							helper->DrawWithPtr(mesh.IndexBufferPtr->GetCount() / 3, mesh.VertexBufferPtr.get(), mesh.IndexBufferPtr.get(), shader.get(), state,
								shaderConstants.data(), shaderConstants.size());

							fOffset += fCount;
						}
					}
				}
			}
		}
	}

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

	void RenderedModelObject3D::CalculateAnimation(std::vector <BoneProperty>& boneProps, Deformer* deformer, AnimationClip* animationClip, int32_t time)
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

			ModelUtils::SetBoneValue(
				boneProps[bi].Position,
				boneProps[bi].Rotation,
				boneProps[bi].Scale,
				type,
				axis,
				value);
		}
	}

	void RenderedModelObject3D::CalclateBoneMatrices(std::vector<Matrix44>& matrixes, std::vector <BoneProperty>& boneProps, Deformer* deformer, bool isPlayingAnimation)
	{
		if (deformer == nullptr) return;
		auto d = (Deformer_Imp*) deformer;

		if (isPlayingAnimation)
		{
			for (auto i = 0; i < d->GetBones().size(); i++)
			{
				auto& b = d->GetBones()[i];
				matrixes[i] = boneProps[i].CalcMatrix(b.RotationType);
			}
		}
		else
		{
		}

		ModelUtils::CalculateBoneMatrixes(
			matrixes,
			d->GetBones(),
			matrixes,
			isPlayingAnimation);
	}

	RenderedModelObject3D::RenderedModelObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
		, m_animationPlaying(nullptr)
		, m_animationTime(0)
	{
		proxy = new RenderedModelObject3DProxy(graphics);
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

		CalculateAnimation(m_boneProps, m_deformer.get(), m_animationPlaying, m_animationTime);
		CalclateBoneMatrices(m_matrixes, m_boneProps, m_deformer.get(), m_animationPlaying != nullptr);
		
		// アニメーションの適用
		if (m_animationPlaying != nullptr)
		{
			m_animationTime++;
		}

		proxy->m_meshes_rt = m_meshes;
		proxy->m_matrixes_rt = m_matrixes;
	}
}
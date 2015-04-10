
#include "ace.MassModel_Imp.h"

#include "../../ace.Graphics_Imp.h"

#include "../../Resource/ace.VertexBuffer_Imp.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"
#include "../../Resource/ace.RenderTexture2D_Imp.h"

#include "../../Resource/ace.Material3D.h"
#include "../../Resource/ace.Shader3D.h"
#include "../../Resource/ace.Shader3D_Imp.h"

namespace ace
{
	MassModel_Imp::Material::Material()
	{
	}

	MassModel_Imp::Material::~Material()
	{
	}

	MassModel_Imp::MassModel_Imp()
	{
	}

	MassModel_Imp::~MassModel_Imp()
	{
	}

	const achar* MassModel_Imp::GetAnimationName(int32_t index) const
	{
		if (AnimationClipNames.size() <= index) return nullptr;
		if (index < 0) return nullptr;
		return AnimationClipNames[index].c_str();
	}

	float MassModel_Imp::GetAnimationLength(const achar* name) const
	{
		auto ind = GetClipIndex(name);
		if (ind == -1) return false;
		return frameCount[ind];
	}

	bool MassModel_Imp::GetIsAnimationLoopingMode(const achar* name) const
	{
		auto ind = GetClipIndex(name);
		if (ind == -1) return false;
		return loopingMode[ind];
	}

	void MassModel_Imp::SetIsAnimationLoopingMode(const achar* name, bool isLoopingMode)
	{
		auto ind = GetClipIndex(name);
		if (ind == -1) return;
		loopingMode[ind] = isLoopingMode;
	}

	void MassModel_Imp::SetMaterial(Material3D* material)
	{
		SafeAddRef(material);
		auto t = CreateSharedPtrWithReleaseDLL(material);
		this->material.Material_ = t;

		if (this->material.Material_ != nullptr)
		{
			auto shader = (Shader3D_Imp*)(this->material.Material_->GetShader3D().get());
			shader->CompileMass();
		}
	}

	bool MassModel_Imp::Load(Graphics_Imp* g, MassModel_IO& io)
	{
		m_vertexBuffer.reset();
		m_indexBuffer.reset();
		m_animationTexture.reset();

		// 頂点バッファ
		m_vertexBuffer = g->CreateVertexBuffer_Imp(sizeof(MassModel_IO::Vertex), io.Vertices.size(), false);
		m_vertexBuffer->Lock();

		auto vbuf = m_vertexBuffer->GetBuffer<MassModel_IO::Vertex>(io.Vertices.size());
		for (size_t i = 0; i < io.Vertices.size(); i++)
		{
			vbuf[i] = io.Vertices[i];
		}

		m_vertexBuffer->Unlock();

		// インデックスバッファ
		m_indexBuffer = g->CreateIndexBuffer_Imp(io.Faces.size() * 3, false, true);
		m_indexBuffer->Lock();

		auto ibuf = m_indexBuffer->GetBuffer<int32_t>(io.Faces.size() * 3);
		for (size_t i = 0; i < io.Faces.size(); i++)
		{
			ibuf[i * 3 + 0] = io.Faces[i].Index1;
			ibuf[i * 3 + 1] = io.Faces[i].Index2;
			ibuf[i * 3 + 2] = io.Faces[i].Index3;
		}

		m_indexBuffer->Unlock();

		// アニメーションテクスチャ
		if (io.AnimationTexture_.TextureWidth > 0)
		{
			auto texture = g->CreateEmptyTexture2D(io.AnimationTexture_.TextureWidth, io.AnimationTexture_.TextureHeight, TextureFormat::R32G32B32A32_FLOAT);
			TextureLockInfomation info;

			if (texture->Lock(&info))
			{
				memcpy(info.GetPixels(), io.AnimationTexture_.Buffer.data(), io.AnimationTexture_.Buffer.size() * sizeof(float) * 4);
				texture->Unlock();
			}

			m_animationTexture = texture;
		}
		

		frameCount = io.AnimationTexture_.FrameCount;

		loopingMode.resize(frameCount.size());
		for (auto i = 0; i < loopingMode.size(); i++)
		{
			loopingMode[i] = false;
		}

		for (auto& clip : io.AnimationClips)
		{
			animationClips[clip.Name] = clip.Index;

			if (AnimationClipNames.size() <= clip.Index)
			{
				AnimationClipNames.resize(clip.Index + 1);
				AnimationClipNames[clip.Index] = clip.Name;
			}
		}

		// マテリアル
		if (io.Material_.ColorTexture != astring()) material.ColorTexture = g->CreateTexture2D(io.Material_.ColorTexture.c_str());
		if (io.Material_.NormalTexture != astring()) material.NormalTexture = g->CreateTexture2DAsRawData(io.Material_.NormalTexture.c_str());
		if (io.Material_.MetalnessTexture != astring()) material.MetalnessTexture = g->CreateTexture2DAsRawData(io.Material_.MetalnessTexture.c_str());

		return true;
	}
}
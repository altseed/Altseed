
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
		auto texture = g->CreateEmptyTexture2D(io.AnimationTexture.TextureWidth, io.AnimationTexture.TextureHeight, TextureFormat::R32G32B32A32_FLOAT);
		TextureLockInfomation info;

		if (texture->Lock(info))
		{
			memcpy(info.Pixels, io.AnimationTexture.Buffer.data(), io.AnimationTexture.Buffer.size() * sizeof(float) * 4);
			texture->Unlock();
		}

		m_animationTexture = texture;

		frameCount = io.AnimationTexture.FrameCount;
		frameSkip = io.AnimationTexture.FrameSkip;

		for (auto& clip : io.AnimationClips)
		{
			animationClips[clip.Name] = clip.Index;
		}

		// マテリアル
		if (io.Material_.ColorTexture != astring()) material.ColorTexture = g->CreateTexture2D(io.Material_.ColorTexture.c_str());
		if (io.Material_.NormalTexture != astring()) material.NormalTexture = g->CreateTexture2D(io.Material_.NormalTexture.c_str());
		if (io.Material_.SpecularTexture != astring()) material.SpecularTexture = g->CreateTexture2D(io.Material_.SpecularTexture.c_str());

		return true;
	}
}
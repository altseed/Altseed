
#include "ace.MassModel_Imp.h"

#include "../../ace.Graphics_Imp.h"

#include "../../Resource/ace.VertexBuffer_Imp.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"
#include "../../Resource/ace.RenderTexture2D_Imp.h"

namespace ace
{
	MassModel_Imp::MassModel_Imp()
	{
	}

	MassModel_Imp::~MassModel_Imp()
	{
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

		return true;
	}
}

#include "ace.Mesh.h"
#include "../ace.Graphics_Imp.h"

#include "../Resource/ace.VertexBuffer_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"

namespace ace
{

	Mesh::Mesh(Graphics* graphics)
		: m_graphics(nullptr)
		, m_vertexBuffer(nullptr)
		, m_indexBuffer(nullptr)
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);
	}

	Mesh::~Mesh()
	{
		SafeRelease(m_graphics);
	}


	void Mesh::AddVertex(
		const Vector3DF& position,
		const Vector3DF& normal,
		const Vector3DF& binormal,
		const Vector3DF& tangent,
		const Vector2DF& uv1,
		const Vector2DF& uv2,
		const Color& color,
		int32_t boneIndexes,
		int32_t boneWeights)
	{
		Vertex v;
		v.Position = position;
		v.Normal = normal;
		v.Binormal = binormal;
		v.Tangent = tangent;
		v.UV1 = uv1;
		v.UV2 = uv2;
		v.VColor = color;
		v.BoneIndexes = boneIndexes;
		v.BoneWeights = boneWeights;

		m_vertexBufferOnMM.push_back(v);
	}

	void Mesh::AddFace(int32_t index1, int32_t index2, int32_t index3)
	{
		Face f;
		f.Index1 = index1;
		f.Index2 = index2;
		f.Index3 = index3;

		m_faceBufferOnMM.push_back(f);
	}

	void Mesh::AddMaterialOffset(int32_t materialIndex, int32_t faceOffset)
	{
		MaterialOffset mo;
		mo.MaterialIndex = materialIndex;
		mo.FaceOffset = faceOffset;

		m_materialOffsets.push_back(mo);
	}

	void Mesh::SendToGPUMemory()
	{
		m_vertexBuffer.reset();
		m_indexBuffer.reset();

		// 頂点バッファ送信
		m_vertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(Vertex), m_vertexBufferOnMM.size(), false);
		m_vertexBuffer->Lock();

		auto vbuf = m_vertexBuffer->GetBuffer<Vertex>(m_vertexBufferOnMM.size());
		for (size_t i = 0; i < m_vertexBufferOnMM.size(); i++)
		{
			vbuf[i] = m_vertexBufferOnMM[i];
		}

		m_vertexBuffer->Unlock();

		// インデックスバッファ送信
		m_indexBuffer = m_graphics->CreateIndexBuffer_Imp(m_faceBufferOnMM.size() * 3, false, true);
		m_indexBuffer->Lock();

		auto ibuf = m_indexBuffer->GetBuffer<int32_t>(m_faceBufferOnMM.size() * 3);
		for (size_t i = 0; i < m_faceBufferOnMM.size(); i++)
		{
			ibuf[i * 3 + 0] = m_faceBufferOnMM[i].Index1;
			ibuf[i * 3 + 1] = m_faceBufferOnMM[i].Index2;
			ibuf[i * 3 + 2] = m_faceBufferOnMM[i].Index3;
		}

		m_indexBuffer->Unlock();
	}
};

#include "ace.Mesh_Imp.h"
#include "ace.Deformer_Imp.h"

#include "../ace.Graphics_Imp.h"

#include "../Resource/ace.VertexBuffer_Imp.h"
#include "../Resource/ace.IndexBuffer_Imp.h"

namespace ace
{
	Mesh_Imp::Material::Material()
		: ColorTexture(nullptr)
		, NormalTexture(nullptr)
		, SpecularTexture(nullptr)
	{
	}

	Mesh_Imp::Material::~Material()
	{
		SafeRelease(ColorTexture);
		SafeRelease(NormalTexture);
		SafeRelease(SpecularTexture);
	}

	Mesh_Imp::Mesh_Imp(Graphics* graphics)
		: m_graphics(nullptr)
		, m_vertexBuffer(nullptr)
		, m_indexBuffer(nullptr)
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);
	}

	Mesh_Imp::~Mesh_Imp()
	{
		SafeRelease(m_graphics);
	}

	Mesh_Imp* Mesh_Imp::Create(Graphics* graphics)
	{
		return new Mesh_Imp(graphics);
	}

	void Mesh_Imp::AddVertex(
		const Vector3DF& position,
		const Vector3DF& normal,
		const Vector3DF& binormal,
		const Vector2DF& uv1,
		const Vector2DF& uv2,
		const Color& color,
		int32_t boneWeights,
		int32_t boneIndexes)
	{
		Vertex v;
		v.Position = position;
		v.Normal = normal;
		v.Binormal = binormal;
		v.UV1 = uv1;
		v.UV2 = uv2;
		v.VColor = color;
		v.BoneWeights = boneWeights;
		v.BoneIndexes = boneIndexes;
		v.BoneIndexesOriginal = boneIndexes;
		m_vertexBufferOnMM.push_back(v);
	}

	void Mesh_Imp::AddInternalVertex(
		const Vector3DF& position,
		const Vector3DF& normal,
		const Vector3DF& binormal,
		const Vector2DF& uv1,
		const Vector2DF& uv2,
		const Color& color,
		int32_t boneWeights,
		int32_t boneIndexes,
		int32_t boneIndexesOriginal)
	{
		Vertex v;
		v.Position = position;
		v.Normal = normal;
		v.Binormal = binormal;
		v.UV1 = uv1;
		v.UV2 = uv2;
		v.VColor = color;
		v.BoneWeights = boneWeights;
		v.BoneIndexes = boneIndexes;
		v.BoneIndexesOriginal = boneIndexesOriginal;
		m_vertexBufferOnMM.push_back(v);
	}

	void Mesh_Imp::AddInternalBoneOffset(uint8_t boneIndex[32], int32_t faceOffset)
	{
		BoneOffset offset;

		for (int32_t i = 0; i < 32; i++)offset.BoneIndex[i] = boneIndex[i];
		offset.FaceOffset = faceOffset;
		m_boneOffsets.push_back(offset);
	}

	void Mesh_Imp::AddFace(int32_t index1, int32_t index2, int32_t index3)
	{
		Face f;
		f.Index1 = index1;
		f.Index2 = index2;
		f.Index3 = index3;

		m_faceBufferOnMM.push_back(f);
	}

	void Mesh_Imp::AddMaterialCount(int32_t materialIndex, int32_t faceCount)
	{
		MaterialOffset mo;
		mo.MaterialIndex = materialIndex;
		mo.FaceOffset = faceCount;

		m_materialOffsets.push_back(mo);

		m_materials.push_back(Material());
	}

	void Mesh_Imp::SendToGPUMemory()
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

	Deformer* Mesh_Imp::GetDeformer_()
	{
		return m_deformer;
	}

	void Mesh_Imp::SetDeformer(Deformer* deformer)
	{
		SafeSubstitute(m_deformer, deformer);
	}

	void Mesh_Imp::SetColorTexture(int32_t materialIndex, Texture2D* texture)
	{
		if (m_materials.size() <= materialIndex) return;
		SafeSubstitute(m_materials[materialIndex].ColorTexture, texture);
	}

	void Mesh_Imp::SetNormalTexture(int32_t materialIndex, Texture2D* texture)
	{
		if (m_materials.size() <= materialIndex) return;
		SafeSubstitute(m_materials[materialIndex].NormalTexture, texture);
	}

	void Mesh_Imp::SetSpecularTexture(int32_t materialIndex, Texture2D* texture)
	{
		if (m_materials.size() <= materialIndex) return;
		SafeSubstitute(m_materials[materialIndex].SpecularTexture, texture);
	}
};
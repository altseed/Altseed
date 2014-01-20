
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
		, m_requireToCalcInternalParameters(nullptr)
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

		m_requireToCalcInternalParameters = true;
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
		if (m_requireToCalcInternalParameters)
		{
			// 内部パラメーターを計算する
			m_boneOffsets.clear();

			// ボーンのインデックスに応じて分割していく。
			int32_t offset = 0;
			int32_t count = 0;
			std::set<int32_t> boneIndexes;
			std::map<int32_t, int32_t> boneIndexTo;

			// ボーンの集合体を登録する。
			auto registBoneOffset = [this, &boneIndexTo, &boneIndexes](int32_t& offset_, int32_t& count_) -> void
			{
				boneIndexTo.clear();
				int32_t bCount = 0;
				uint8_t boneIndexTable[32];
				for (auto& b : boneIndexes)
				{
					boneIndexTo[b] = bCount;
					boneIndexTable[b] = bCount;
					bCount++;
				}

				for (auto j = offset_; j < offset_ + count_; j++)
				{
					auto& v = m_vertexBufferOnMM[j];
					uint8_t* indexesSrc = (uint8_t*) (&v.BoneIndexesOriginal);
					uint8_t* indexesDst = (uint8_t*) (&v.BoneIndexes);
					for (int32_t loop = 0; loop < 4; loop++)
					{
						indexesDst[loop] = (uint8_t) boneIndexTo[indexesSrc[loop]];
					}
				}

				AddInternalBoneOffset(boneIndexTable, count_);

				offset_ += count_;
				count_ = 0;
			};

			for (auto i = 0; i < m_vertexBufferOnMM.size(); i++)
			{
				auto& v = m_vertexBufferOnMM[i];

				uint8_t* indexes = (uint8_t*)(&v.BoneIndexesOriginal);

				// 未登録のインデックス数探索
				int32_t unregistedCount = 0;
				for (int32_t loop = 0; loop < 4; loop++)
				{
					if (boneIndexes.count(indexes[loop]) == 0)
					{
						unregistedCount++;
					}
				}

				if (boneIndexes.size() + unregistedCount <= 32)
				{
					for (int32_t loop = 0; loop < 4; loop++)
					{
						boneIndexes.insert(indexes[loop]);
					}
				}
				else
				{
					// 未変換のボーンの種類が32に達しそうだったら変換
					registBoneOffset(offset, count);
				}

				count++;
			}

			// 末尾を変換
			registBoneOffset(offset, count);

			m_requireToCalcInternalParameters = false;
		}

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

	Mesh_Imp::Material* Mesh_Imp::GetMaterial(int32_t materialIndex)
	{
		if (m_materials.size() <= materialIndex) return nullptr;
		return &m_materials[materialIndex];
	}
};
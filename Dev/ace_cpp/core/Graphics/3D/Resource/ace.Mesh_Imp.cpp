
#include "ace.Mesh_Imp.h"

#include "../../ace.Graphics_Imp.h"

#include "../../Resource/ace.VertexBuffer_Imp.h"
#include "../../Resource/ace.IndexBuffer_Imp.h"

namespace ace
{
	Mesh_Imp::Material::Material()
	{
	}

	Mesh_Imp::Material::~Material()
	{
	}

	Mesh_Imp::Mesh_Imp(Graphics* graphics)
		: m_graphics(nullptr)
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

	void Mesh_Imp::AddFace(int32_t index1, int32_t index2, int32_t index3, int32_t materialIndex)
	{
		Face f;
		f.Index1 = index1;
		f.Index2 = index2;
		f.Index3 = index3;
		f.MaterialIndex = materialIndex;
		m_faceBufferOnMM.push_back(f);
	}

	void Mesh_Imp::AddBoneConnector(int32_t targetIndex, const Matrix44& boneToMesh)
	{
		BoneConnector bc;
		bc.TargetIndex = targetIndex;
		bc.BoneToMesh = boneToMesh;
		m_boneConnectors.push_back(bc);
	}

	void Mesh_Imp::AddDividedMesh(const std::vector<Vertex>& vertexes, const std::vector<Face>& faces, const std::vector<BoneConnector>& boneConnectors, const std::vector<MaterialOffset>& materialOffsets)
	{
		// 頂点バッファ
		auto vertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(Vertex), vertexes.size(), false);
		vertexBuffer->Lock();

		auto vbuf = vertexBuffer->GetBuffer<Vertex>(vertexes.size());
		for (size_t i = 0; i < vertexes.size(); i++)
		{
			vbuf[i] = vertexes[i];
		}

		vertexBuffer->Unlock();

		// インデックスバッファ
		auto indexBuffer = m_graphics->CreateIndexBuffer_Imp(faces.size() * 3, false, true);
		
		indexBuffer->Lock();

		auto ibuf = indexBuffer->GetBuffer<int32_t>(faces.size() * 3);
		for (size_t i = 0; i < faces.size(); i++)
		{
			ibuf[i * 3 + 0] = faces[i].Index1;
			ibuf[i * 3 + 1] = faces[i].Index2;
			ibuf[i * 3 + 2] = faces[i].Index3;
		}

		indexBuffer->Unlock();
		
		m_dividedMeshes.resize(m_dividedMeshes.size() + 1);

		auto dividedMesh = (m_dividedMeshes.end() - 1);
		dividedMesh->VertexBufferPtr = vertexBuffer;
		dividedMesh->IndexBufferPtr = indexBuffer;
		dividedMesh->VertexBufferOnMM = vertexes;
		dividedMesh->FaceBufferOnMM = faces;
		dividedMesh->BoneConnectors = boneConnectors;
		dividedMesh->MaterialOffsets = materialOffsets;
	}

	int32_t Mesh_Imp::AddMaterial()
	{
		m_materials.push_back(Material());
		return m_materials.size() - 1;
	}

	void Mesh_Imp::SendToGPUMemory()
	{
		m_dividedMeshes.clear();

		// 内部パラメーターを計算する

		// 材質で面をソート
		std::sort(m_faceBufferOnMM.begin(), m_faceBufferOnMM.end(), [](const Face& f1, const Face& f2) { return f1.MaterialIndex < f2.MaterialIndex; });

		// todo 分割
		m_dividedMeshes.resize(m_dividedMeshes.size() + 1);
		auto dividedMesh = (m_dividedMeshes.end() - 1);
			
		// 頂点バッファ
		auto vertexBuffer = m_graphics->CreateVertexBuffer_Imp(sizeof(Vertex), m_vertexBufferOnMM.size(), false);
		vertexBuffer->Lock();

		auto vbuf = vertexBuffer->GetBuffer<Vertex>(m_vertexBufferOnMM.size());
		for (size_t i = 0; i < m_vertexBufferOnMM.size(); i++)
		{
			m_vertexBufferOnMM[i].BoneIndexes = m_vertexBufferOnMM[i].BoneIndexesOriginal;
			vbuf[i] = m_vertexBufferOnMM[i];
		}

		vertexBuffer->Unlock();

		// インデックスバッファ
		auto indexBuffer = m_graphics->CreateIndexBuffer_Imp(m_faceBufferOnMM.size() * 3, false, true);

		indexBuffer->Lock();

		auto ibuf = indexBuffer->GetBuffer<int32_t>(m_faceBufferOnMM.size() * 3);
		for (size_t i = 0; i < m_faceBufferOnMM.size(); i++)
		{
			ibuf[i * 3 + 0] = m_faceBufferOnMM[i].Index1;
			ibuf[i * 3 + 1] = m_faceBufferOnMM[i].Index2;
			ibuf[i * 3 + 2] = m_faceBufferOnMM[i].Index3;
		}

		indexBuffer->Unlock();

		// 材質面出力
		std::vector<MaterialOffset> materialOffsets;
		{
			int32_t materialIndex = -1;
			int32_t materialCount = 0;
			for (const auto& face : m_faceBufferOnMM)
			{
				if (materialIndex == face.MaterialIndex)
				{
					materialCount++;
				}
				else
				{
					if (materialCount > 0)
					{
						MaterialOffset mo;
						mo.MaterialIndex = materialIndex;
						mo.FaceOffset = materialCount;
						materialOffsets.push_back(mo);
						materialCount = 0;
					}

					materialCount++;
					materialIndex = face.MaterialIndex;
				}
			}

			if (materialCount > 0)
			{
				MaterialOffset mo;
				mo.MaterialIndex = materialIndex;
				mo.FaceOffset = materialCount;
				materialOffsets.push_back(mo);
			}
		}

		dividedMesh->VertexBufferPtr = vertexBuffer;
		dividedMesh->IndexBufferPtr = indexBuffer;
		dividedMesh->VertexBufferOnMM = m_vertexBufferOnMM;
		dividedMesh->FaceBufferOnMM = m_faceBufferOnMM;
		dividedMesh->BoneConnectors = m_boneConnectors;

		dividedMesh->MaterialOffsets = materialOffsets;
	}

	void Mesh_Imp::SetColorTexture(int32_t materialIndex, Texture2D* texture)
	{
		if (m_materials.size() <= materialIndex) return;

		SafeAddRef(texture);
		auto t = CreateSharedPtrWithReleaseDLL(texture);
		m_materials[materialIndex].ColorTexture = t;
	}

	void Mesh_Imp::SetNormalTexture(int32_t materialIndex, Texture2D* texture)
	{
		if (m_materials.size() <= materialIndex) return;
		
		SafeAddRef(texture);
		auto t = CreateSharedPtrWithReleaseDLL(texture);
		m_materials[materialIndex].NormalTexture = t;
	}

	void Mesh_Imp::SetSpecularTexture(int32_t materialIndex, Texture2D* texture)
	{
		if (m_materials.size() <= materialIndex) return;
		
		SafeAddRef(texture);
		auto t = CreateSharedPtrWithReleaseDLL(texture);
		m_materials[materialIndex].SpecularTexture = t;
	}

	Mesh_Imp::Material* Mesh_Imp::GetMaterial(int32_t materialIndex)
	{
		if (m_materials.size() <= materialIndex) return nullptr;
		return &m_materials[materialIndex];
	}
};
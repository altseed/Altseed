
#include "ace.Model_Imp.h"

#include "ace.Mesh_Imp.h"
#include "ace.Deformer_Imp.h"
#include "../Animation/ace.AnimationClip_Imp.h"
#include "../Animation/ace.AnimationSource_Imp.h"

#include "../ace.Graphics_Imp.h"

namespace ace
{
	void Model_Imp::Reset()
	{
		for (auto& m : m_meshes)
		{
			m->Release();
		}

		for (auto& a : m_animationClips)
		{
			a->Release();
		}
	}

	Model_Imp::Model_Imp()
	{
	}

	Model_Imp::~Model_Imp()
	{
		Reset();
	}

	bool Model_Imp::Load(Graphics* g, std::vector<uint8_t>&	data, const achar* path)
	{
		Reset();

		BinaryReader reader;
		reader.ReadIn(data.begin(), data.end());

		// ヘッダーチェック
		uint8_t header_true[] = "MDL";
		for (int32_t i = 0; i < 4; i++)
		{
			auto h = reader.Get<uint8_t>();
			if (header_true[i] != h) return false;
		}
		
		// バージョン
		int32_t version = reader.Get<int32_t>();

		// メッシュグループ
		int32_t meshGroupCount = reader.Get<int32_t>();
		for (int32_t i = 0; i < meshGroupCount; i++)
		{
			auto mesh = LoadMeshGroup(g, reader, path);
			m_meshes.push_back(mesh);
		}

		// アニメーション
	}

	Mesh_Imp* Model_Imp::LoadMeshGroup(Graphics* g, BinaryReader& reader, const achar* path)
	{
		auto mesh = LoadMesh(g, reader, path);
		auto deformer = LoadDeformer(g, reader, path);
		mesh->SetDeformer(deformer);

		return mesh;
	}

	Mesh_Imp* Model_Imp::LoadMesh(Graphics* g, BinaryReader& reader, const achar* path)
	{
		auto g_ = (Graphics_Imp*) g;
		auto mesh = (Mesh_Imp*)g_->CreateMesh_();

		int32_t vcount = reader.Get<int32_t>();
		for (int32_t i = 0; i < vcount; i++)
		{
			auto pos = reader.Get<Vector3DF>();
			auto normal = reader.Get<Vector3DF>();
			auto binormal = reader.Get<Vector3DF>();
			auto uv = reader.Get<Vector2DF>();
			auto subuv = reader.Get<Vector2DF>();
			auto color = reader.Get<Color>();
			auto weights = reader.Get<int32_t>();
			auto indexes = reader.Get<int32_t>();
			auto indexesOriginal = reader.Get<int32_t>();

			mesh->AddInternalVertex(pos, normal, binormal, uv, subuv, color, weights, indexes, indexesOriginal);
		}

		int32_t fcount = reader.Get<int32_t>();
		for (int32_t i = 0; i < fcount; i++)
		{
			auto f1 = reader.Get<int32_t>();
			auto f2 = reader.Get<int32_t>();
			auto f3 = reader.Get<int32_t>();

			mesh->AddFace(f1, f2, f3);
		}

		int32_t mcount = reader.Get<int32_t>();
		for (int32_t i = 0; i < mcount; i++)
		{
			auto index = reader.Get<int32_t>();
			auto facecount = reader.Get<int32_t>();
		
			mesh->AddMaterialOffset(index, facecount);
		}

		int32_t bcount = reader.Get<int32_t>();
		for (int32_t i = 0; i < bcount; i++)
		{
			uint8_t offset[32];
			for (int32_t i = 0; i < 32; i++) offset[i] = reader.Get<uint8_t>();
			auto facecount = reader.Get<int32_t>();

			mesh->AddInternalBoneOffset(offset, facecount);
		}

		mesh->SendToGPUMemory();

		return mesh;
	}

	Deformer_Imp* Model_Imp::LoadDeformer(Graphics* g, BinaryReader& reader, const achar* path)
	{
		auto g_ = (Graphics_Imp*) g;
		auto deformer = (Deformer_Imp*) g_->CreateDeformer_();

		int32_t bcount = reader.Get<int32_t>();
		for (int32_t i = 0; i < bcount; i++)
		{
			auto name = reader.Get<ace::astring>();
			auto parent = reader.Get<int32_t>();
			eRotationOrder rotationOrder = reader.Get<eRotationOrder>();
			auto localMat = reader.Get<Matrix44>();
			auto matInv = reader.Get<Matrix44>();

			deformer->AddBone(name.c_str(), parent, rotationOrder, localMat, matInv);
		}

		return deformer;
	}
}
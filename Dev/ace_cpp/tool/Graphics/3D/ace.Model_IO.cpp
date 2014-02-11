
#include "ace.Model_IO.h"

namespace ace
{
	bool Model_IO::Load(std::vector<uint8_t>&	data, const achar* path)
	{
		MeshGroups.clear();

		BinaryReader reader;
		reader.ReadIn(data.begin(), data.end());

		// ヘッダーチェック
		uint8_t header_true [] = "MDL";
		for (int32_t i = 0; i < 4; i++)
		{
			auto h = reader.Get<uint8_t>();
			if (header_true[i] != h) return false;
		}

		// バージョン
		int32_t version = reader.Get<int32_t>();

		// メッシュグループ
		int32_t meshGroupCount = reader.Get<int32_t>();
		MeshGroups.resize(meshGroupCount);
		for (int32_t i = 0; i < meshGroupCount; i++)
		{
			LoadMeshGroup(&(MeshGroups[i]), reader, path);
		}

		// アニメーション
		int32_t sourceCount = reader.Get<int32_t>();
		AnimationSources.resize(sourceCount);
		for (int32_t i = 0; i < sourceCount; i++)
		{
			LoadAnimationSource(&(AnimationSources[i]), reader, path);
		}

		int32_t clipCount = reader.Get<int32_t>();
		AnimationClips.resize(clipCount);
		for (int32_t i = 0; i < clipCount; i++)
		{
			LoadAnimationClip(&(AnimationClips[i]), reader, path);
		}

		return true;
	}

	void Model_IO::LoadMeshGroup(MeshGroup* meshGroup, BinaryReader& reader, const achar* path)
	{
		LoadMesh(&(meshGroup->Mesh), reader, path);
		LoadDeformer(&(meshGroup->Deformer), reader, path);
		LoadMaterials(&(meshGroup->Materials), reader, path);
	}

	void Model_IO::LoadMesh(Mesh* mesh, BinaryReader& reader, const achar* path)
	{
		int32_t vcount = reader.Get<int32_t>();
		mesh->Vertices.resize(vcount);

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

			mesh->Vertices[i].Position = pos;
			mesh->Vertices[i].Normal = normal;
			mesh->Vertices[i].Binormal = binormal;
			mesh->Vertices[i].UV1 = uv;
			mesh->Vertices[i].UV2 = subuv;
			mesh->Vertices[i].VColor = color;
			mesh->Vertices[i].BoneWeights = weights;
			mesh->Vertices[i].BoneIndexes = indexes;
			mesh->Vertices[i].BoneIndexesOriginal = indexesOriginal;
		}

		int32_t fcount = reader.Get<int32_t>();
		mesh->Faces.resize(fcount);

		for (int32_t i = 0; i < fcount; i++)
		{
			auto f1 = reader.Get<int32_t>();
			auto f2 = reader.Get<int32_t>();
			auto f3 = reader.Get<int32_t>();

			mesh->Faces[i].Index1 = f1;
			mesh->Faces[i].Index2 = f2;
			mesh->Faces[i].Index3 = f3;
		}

		int32_t mcount = reader.Get<int32_t>();
		mesh->MaterialOffsets.resize(mcount);

		for (int32_t i = 0; i < mcount; i++)
		{
			auto index = reader.Get<int32_t>();
			auto facecount = reader.Get<int32_t>();

			mesh->MaterialOffsets[i].MaterialIndex = index;
			mesh->MaterialOffsets[i].FaceOffset = facecount;
		}

		int32_t bcount = reader.Get<int32_t>();
		mesh->BoneOffsets.resize(bcount);

		for (int32_t i = 0; i < bcount; i++)
		{
			uint8_t offset[32];
			for (int32_t j = 0; j < 32; j++) offset[j] = reader.Get<uint8_t>();
			auto facecount = reader.Get<int32_t>();

			mesh->BoneOffsets[i].FaceOffset = facecount;
			for (int32_t j = 0; j < 32; j++) mesh->BoneOffsets[i].BoneIndex[j] = offset[i];
		}
	}

	void Model_IO::LoadDeformer(Deformer* deformer, BinaryReader& reader, const achar* path)
	{
		int32_t bcount = reader.Get<int32_t>();
		deformer->Bones.resize(bcount);

		for (int32_t i = 0; i < bcount; i++)
		{
			auto name = reader.Get<ace::astring>();
			auto parent = reader.Get<int32_t>();
			eRotationOrder rotationOrder = reader.Get<eRotationOrder>();
			auto localMat = reader.Get<Matrix44>();
			auto matInv = reader.Get<Matrix44>();

			deformer->Bones[i].Name = name;
			deformer->Bones[i].ParentBoneIndex = parent;
			deformer->Bones[i].RotationType = rotationOrder;
			deformer->Bones[i].LocalMat = localMat;
			deformer->Bones[i].GlobalMatInv = matInv;
		}
	}

	void Model_IO::LoadMaterials(std::vector<Material>* materials, BinaryReader& reader, const achar* path)
	{
		int32_t materialCount = reader.Get<int32_t>();
		materials->resize(materialCount);

		for (int32_t i = 0; i < materialCount; i++)
		{
			LoadMaterial(&(materials->at(i)), reader, path);
		}
	}

	void Model_IO::LoadMaterial(Material* material, BinaryReader& reader, const achar* path)
	{
		auto type = reader.Get<int32_t>();
		if (type == 1)
		{
			auto path = reader.Get<ace::astring>();
		}
		else
		{
			auto pathColor = reader.Get<ace::astring>();
			auto pathNormal = reader.Get<ace::astring>();
			auto pathSpecular = reader.Get<ace::astring>();

			material->ColorTexture = pathColor;
			material->NormalTexture = pathNormal;
			material->SpecularTexture = pathSpecular;
		}
	}

	void Model_IO::LoadAnimationSource(AnimationSource* as, BinaryReader& reader, const achar* path)
	{
		auto sourceName = reader.Get<ace::astring>();
		auto keyframesCount = reader.Get<int32_t>();

		as->Name = sourceName;
		as->KeyframeAnimations.resize(keyframesCount);

		for (int32_t i = 0; i < keyframesCount; i++)
		{
			LoadKeyframeAnimation(&(as->KeyframeAnimations[i]), reader, path);
		}
	}

	void Model_IO::LoadKeyframeAnimation(KeyframeAnimation* ka, BinaryReader& reader, const achar* path)
	{
		auto name = reader.Get<ace::astring>();
		ka->Name = name;

		auto keyCount = reader.Get<int32_t>();
		ka->Keyframes.resize(keyCount);

		for (int32_t i = 0; i < keyCount; i++)
		{
			ka->Keyframes[i].KeyValue = reader.Get<Vector2DF>();
			ka->Keyframes[i].LeftHandle = reader.Get<Vector2DF>();
			ka->Keyframes[i].RightHandle = reader.Get<Vector2DF>();
			ka->Keyframes[i].InterpolationType = (eInterpolationType) reader.Get<int32_t>();
		}
	}

	void Model_IO::LoadAnimationClip(AnimationClip* ac, BinaryReader& reader, const achar* path)
	{
		auto clipName = reader.Get<ace::astring>();
		auto sourceIndex = reader.Get<int32_t>();

		ac->Name = clipName;
		ac->Index = sourceIndex;
	}
}
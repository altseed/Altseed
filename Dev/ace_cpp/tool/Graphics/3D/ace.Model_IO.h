
#pragma once

#include <ace.common.Base.h>

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>

#include <Graphics/ace.Color.h>
#include <Graphics/ace.Graphics.Common.h>

#include <Utility/ace.BinaryReader.h>

namespace ace
{
	class Model_IO
	{
	public:
		struct Vertex
		{
			Vector3DF	Position;
			Vector3DF	Normal;
			Vector3DF	Binormal;
			Vector2DF	UV1;
			Vector2DF	UV2;
			Color		VColor;
			int32_t		BoneWeights;
			int32_t		BoneIndexes;
			int32_t		BoneIndexesOriginal;
		};

		struct Face
		{
			int32_t		Index1;
			int32_t		Index2;
			int32_t		Index3;
		};

		struct MaterialOffset
		{
			int32_t		MaterialIndex;
			int32_t		FaceOffset;
		};

		struct BoneOffset
		{
			uint8_t		BoneIndex[32];
			int32_t		FaceOffset;
		};

		struct Bone
		{
			astring				Name;
			int32_t				ParentBoneIndex;
			eRotationOrder		RotationType;
			Matrix44			LocalMat;
			Matrix44			GlobalMatInv;
		};

		struct Mesh
		{
			std::vector<Vertex>	Vertices;
			std::vector<Face>	Faces;
			std::vector<MaterialOffset>	MaterialOffsets;
			std::vector<BoneOffset> BoneOffsets;
		};

		struct Deformer
		{
			std::vector<Bone>	Bones;
		};

		struct Material
		{
			astring	ColorTexture;
			astring	NormalTexture;
			astring	SpecularTexture;
		};

		struct MeshGroup
		{
			Mesh Mesh;
			Deformer Deformer;
			std::vector<Material> Materials;
		};

		struct KeyframeAnimation
		{
			astring Name;
			std::vector<FCurveKeyframe>	Keyframes;
		};

		struct AnimationSource
		{
			astring	Name;
			std::vector<KeyframeAnimation>	KeyframeAnimations;
		};

		struct AnimationClip
		{
			astring	Name;
			int32_t	Index;
		};

		/**
			@brief	メッシュ
		*/
		std::vector<MeshGroup>	MeshGroups;
		
		/**
			@brief	アニメーションソース
		*/
		std::vector<AnimationSource>	AnimationSources;

		/**
		@brief	アニメーションクリップ
		*/
		std::vector<AnimationClip>		AnimationClips;

		bool Load(std::vector<uint8_t>&	data, const achar* path);

	private:
		void LoadMeshGroup(MeshGroup* meshGroup, BinaryReader& reader, const achar* path);
		void LoadMesh(Mesh* mesh, BinaryReader& reader, const achar* path);
		void LoadDeformer(Deformer* deformer, BinaryReader& reader, const achar* path);
		void LoadMaterials(std::vector<Material>* materials, BinaryReader& reader, const achar* path);
		void LoadMaterial(Material* material, BinaryReader& reader, const achar* path);

		void LoadAnimationSource(AnimationSource* as, BinaryReader& reader, const achar* path);
		void LoadKeyframeAnimation(KeyframeAnimation* ka, BinaryReader& reader, const achar* path);
		void LoadAnimationClip(AnimationClip* ac, BinaryReader& reader, const achar* path);
	};

	class ModelUtils
	{
	public:
		static void CalculateBoneMatrixes(std::vector<Matrix44>& dst, const std::vector<Model_IO::Bone>& bones, const std::vector<Matrix44>& localMatrixes);

		static Matrix44 CalcMatrix(float position[3], float rotation[4], float scale[3], eRotationOrder rotationType);

		static float GetKeyframeValue(float time, const std::vector<FCurveKeyframe>& keyframes);
	};
}
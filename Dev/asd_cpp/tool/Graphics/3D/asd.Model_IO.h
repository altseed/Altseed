
#pragma once

#include <asd.common.Base.h>

#include <Math/asd.Vector2DF.h>
#include <Math/asd.Vector3DF.h>
#include <Math/asd.Matrix44.h>

#include <Graphics/asd.Color.h>
#include <Graphics/asd.Graphics.Common.h>

#include <Utility/asd.BinaryReader.h>
#include <Utility/asd.BinaryWriter.h>

namespace asd
{
	class Model_IO
	{
	public:
		struct Material
		{
			int32_t Type;
			astring	ColorTexture;
			astring	NormalTexture;
			astring	MetalnessTexture;

			astring	OriginalColorTexture;
			astring	OriginalNormalTexture;
			astring	OriginalMetalnessTexture;
		};

		struct Vertex
		{
			Vector3DF	Position;
			Vector3DF	Normal;
			Vector3DF	Binormal;
			Vector2DF	UV1;
			Vector2DF	UV2;
			Color		VColor;
			uint8_t		BoneWeights[4];
			uint8_t		BoneIndexes[4];
			uint8_t		BoneIndexesOriginal[4];

			bool operator<(const Vertex& r) const
			{
				auto& l = *this;
				if (l.Position.X != r.Position.X) return l.Position.X < r.Position.X;
				if (l.Position.Y != r.Position.Y) return l.Position.Y < r.Position.Y;
				if (l.Position.Z != r.Position.Z) return l.Position.Z < r.Position.Z;

				if (l.Normal.X != r.Normal.X) return l.Normal.X < r.Normal.X;
				if (l.Normal.Y != r.Normal.Y) return l.Normal.Y < r.Normal.Y;
				if (l.Normal.Z != r.Normal.Z) return l.Normal.Z < r.Normal.Z;

				if (l.Binormal.X != r.Binormal.X) return l.Binormal.X < r.Binormal.X;
				if (l.Binormal.Y != r.Binormal.Y) return l.Binormal.Y < r.Binormal.Y;
				if (l.Binormal.Z != r.Binormal.Z) return l.Binormal.Z < r.Binormal.Z;

				if (l.UV1.X != r.UV1.X) return l.UV1.X < r.UV1.X;
				if (l.UV1.Y != r.UV1.Y) return l.UV1.Y < r.UV1.Y;

				if (l.UV2.X != r.UV2.X) return l.UV2.X < r.UV2.X;
				if (l.UV2.Y != r.UV2.Y) return l.UV2.Y < r.UV2.Y;

				if (l.VColor.R != r.VColor.R) return l.VColor.R < r.VColor.R;
				if (l.VColor.G != r.VColor.G) return l.VColor.G < r.VColor.G;
				if (l.VColor.B != r.VColor.B) return l.VColor.B < r.VColor.B;
				if (l.VColor.A != r.VColor.A) return l.VColor.A < r.VColor.A;

				for (int32_t i = 0; i < 4; i++)
				{
					if (l.BoneWeights[i] != r.BoneWeights[i]) return l.BoneWeights[i] < r.BoneWeights[i];
					if (l.BoneIndexes[i] != r.BoneIndexes[i]) return l.BoneIndexes[i] < r.BoneIndexes[i];
					if (l.BoneIndexesOriginal[i] != r.BoneIndexesOriginal[i]) return l.BoneIndexesOriginal[i] < r.BoneIndexesOriginal[i];
				}

				return false;
			}
		};

		struct Face
		{
			int32_t		Indexes[3];
		};

		struct MaterialOffset
		{
			int32_t		MaterialIndex;
			int32_t		FaceOffset;
		};

		struct Bone
		{
			astring				Name;
			int32_t				ParentBoneIndex;
			RotationOrder		RotationType;
			float				Translation[3];
			float				Rotation[4];
			float				Scaling[3];

			Matrix44			LocalMat;
		};

		struct BoneConnector
		{
			int32_t			TargetIndex;
			Matrix44	OffsetMatrix;
		};

		struct DividedMesh
		{
			std::vector<Vertex>	Vertices;
			std::vector<Face>	Faces;
			std::vector<MaterialOffset>	MaterialOffsets;
			std::vector<BoneConnector>	BoneConnectors;
		};

		struct Mesh
		{
			std::vector<DividedMesh>	DividedMeshes;
			std::vector<Material>		Materials;			
		};

		struct Deformer
		{
			std::vector<Bone>	Bones;
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
		std::vector<Mesh>	Meshes;
		
		/**
			@brief	デフォーマー
		*/
		Deformer			Deformer_;
		
		/**
			@brief	アニメーションソース
		*/
		std::vector<AnimationSource>	AnimationSources;

		/**
		@brief	アニメーションクリップ
		*/
		std::vector<AnimationClip>		AnimationClips;

		static void Int32ToUInt8Array(uint8_t dst[4], int32_t src);

		bool Load(const std::vector<uint8_t>& data, const achar* path);

		bool Save(BinaryWriter& writer, const achar* path);

	private:
		void LoadMeshes(std::vector<Mesh>& meshes, BinaryReader& reader, const achar* path);
		void LoadMesh(Mesh& mesh, BinaryReader& reader, const achar* path);
		void LoadDividedMesh(DividedMesh& mesh, BinaryReader& reader, const achar* path);
		void LoadDeformer(Deformer& deformer, BinaryReader& reader, const achar* path, int32_t version);
		void LoadMaterials(std::vector<Material>& materials, BinaryReader& reader, const achar* path);
		void LoadMaterial(Material& material, BinaryReader& reader, const achar* path);

		void LoadAnimationSource(AnimationSource& as, BinaryReader& reader, const achar* path);
		void LoadKeyframeAnimation(KeyframeAnimation& ka, BinaryReader& reader, const achar* path);
		void LoadAnimationClip(AnimationClip& ac, BinaryReader& reader, const achar* path);

		void SaveDeformer(Deformer& deformer, BinaryWriter& writer, const achar* path);

		void SaveMeshes(std::vector<Mesh>& meshes, BinaryWriter& writer, const achar* path);
		void SaveMesh(Mesh& mesh, BinaryWriter& writer, const achar* path);
		void SaveDividedMesh(DividedMesh& mesh, BinaryWriter& writer, const achar* path);

		void SaveMaterials(std::vector<Material>& materials, BinaryWriter& writer, const achar* path);
		void SaveMaterial(Material& material, BinaryWriter& writer, const achar* path);

		void SaveAnimationSource(AnimationSource& as, BinaryWriter& writer, const achar* path);
		void SaveKeyframeAnimation(KeyframeAnimation& ka, BinaryWriter& writer, const achar* path);
		void SaveAnimationClip(AnimationClip& ac, BinaryWriter& writer, const achar* path);
	};

	enum class AnimationCurveTargetType : int32_t
	{
		NoneTarget,
		Position,
		Rotation,
		Scale,
	};

	enum class AnimationCurveTargetAxis : int32_t
	{
		NoneTarget = -1,
		X = 0,
		Y = 1,
		Z = 2,
		W = 3,
	};

	class ModelUtils
	{
	public:
		static void CalculateBoneMatrixes(std::vector<Matrix44>& dst, const std::vector<Model_IO::Bone>& bones, const std::vector<Matrix44>& localMatrixes);

		static Matrix44 CalcRotationMatrix(float rotation[4], RotationOrder rotationType);

		static Matrix44 CalcMatrix(float position[3], float rotation[4], float scale[3], RotationOrder rotationType);

		static Matrix44 CalcMatrix(float position[3], const Matrix44& matR, float scale[3]);

		static float GetKeyframeValue(float time, const std::vector<FCurveKeyframe>& keyframes);

		static bool GetAnimationTarget(astring& targetName, AnimationCurveTargetType& targetType, AnimationCurveTargetAxis& targetAxis, const astring& name);

		static void SetBoneValue(
			float position[3], 
			float rotation[4], 
			float scale[3], 
			AnimationCurveTargetType targetType, 
			AnimationCurveTargetAxis targetAxis, 
			float value);
	};
}
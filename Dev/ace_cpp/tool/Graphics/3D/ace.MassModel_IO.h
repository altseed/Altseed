
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
	class Model_IO;

	class MassModel_IO
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

		struct Material
		{
			astring	ColorTexture;
			astring	NormalTexture;
			astring	SpecularTexture;
		};

		struct AnimationClip
		{
			astring	Name;
			int32_t	Index;
		};

		struct AnimationTexture
		{
			int32_t	AnimationCount;
			int32_t	FrameSkip;
			std::vector<int32_t> FrameCount;
			int32_t	TextureWidth;
			int32_t	TextureHeight;
			std::vector<uint8_t> Buffer;
		};

		std::vector<Vertex>				Vertices;
		std::vector<Face>				Faces;
		std::vector<MaterialOffset>		MaterialOffsets;
		std::vector<Material>			Materials;
		std::vector<AnimationClip>		AnimationClips;
		AnimationTexture				AnimationTexture;

		bool Convert(Model_IO& model);

		void Reset();
	};
}

#pragma once

#include <ace.common.Base.h>

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>
#include <Graphics/ace.Color.h>

#include <Graphics/ace.Graphics.Common.h>

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
			uint32_t		BoneWeights;
			uint32_t		BoneIndexes;
			uint32_t		BoneIndexesOriginal;
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

	};
}
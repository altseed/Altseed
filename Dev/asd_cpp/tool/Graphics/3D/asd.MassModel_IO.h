
#pragma once

#include <asd.common.Base.h>

#include <Math/asd.Vector2DF.h>
#include <Math/asd.Vector3DF.h>
#include <Math/asd.Vector4DF.h>
#include <Math/asd.Matrix44.h>

#include <Graphics/asd.Color.h>
#include <Graphics/asd.Graphics.Common.h>

#include <Utility/asd.BinaryReader.h>
#include <Utility/asd.BinaryWriter.h>

namespace asd
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

		struct AnimationClip
		{
			astring	Name;
			int32_t	Index;
		};

		struct AnimationTexture
		{
			int32_t	AnimationCount;
			std::vector<int32_t> FrameCount;
			int32_t	TextureWidth;
			int32_t	TextureHeight;
			std::vector<Vector4DF> Buffer;
		};

		std::vector<Vertex>				Vertices;
		std::vector<Face>				Faces;
		Material						Material_;
		std::vector<AnimationClip>		AnimationClips;
		AnimationTexture				AnimationTexture_;

		bool Convert(Model_IO& model);

		void Reset();

		bool Load(std::vector<uint8_t>& data, const achar* path);

		bool Save(BinaryWriter& writer, const achar* path);
	};
}
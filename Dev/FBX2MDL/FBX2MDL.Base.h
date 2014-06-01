
#pragma once

#include <fbxsdk.h>

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>
#include <Graphics/ace.Graphics.Common.h>
#include <Graphics/ace.Color.h>

#include <Utility/ace.BinaryWriter.h>

namespace FBX2MDL
{
	class FBXLoader;
	class Scene;
	class Node;
	class Mesh;

	static ace::Matrix44 ToAce(FbxAMatrix& mat)
	{
		ace::Matrix44 mat_;

		for (auto c = 0; c < 4; c++)
		{
			for (auto r = 0; r < 4; r++)
			{
				mat_.Values[c][r] = mat.Get(c, r);
			}
		}

		mat_.SetTransposed();

		return mat_;
	}

	class Node
	{
	private:

	public:
		ace::astring Name;
		std::vector<std::shared_ptr<Node>> Children;

		std::shared_ptr<Mesh> MeshParameter;

		ace::eRotationOrder RotationOrder;

		ace::Matrix44 GeometricMatrix;
		ace::Matrix44 LclMatrix;
	};

	struct KeyFrame
	{
		ace::Vector2DF KeyValue;
		ace::Vector2DF LeftPosition;
		ace::Vector2DF RightPosition;
		int32_t Interpolation;
	};

	struct KeyFrameAnimation
	{
		ace::astring TargetName;
		std::vector<KeyFrame> KeyFrames;
	};

	struct AnimationSource
	{
		ace::astring Name;
		std::vector<KeyFrameAnimation> keyFrameAnimations;
		int32_t StartTime;
		int32_t StopTime;
	};

	class Scene
	{
	public:
		std::shared_ptr<Node> Root;
		std::vector<AnimationSource> AnimationSources;
	};
}

#pragma once

#include <fbxsdk.h>

#include <Math/asd.Vector2DF.h>
#include <Math/asd.Vector3DF.h>
#include <Math/asd.Matrix44.h>
#include <Graphics/asd.Graphics.Common.h>
#include <Graphics/asd.Color.h>

#include <Utility/asd.BinaryWriter.h>

#include <Graphics/3D/asd.Model_IO.h>

#include <functional>
#include <algorithm>

namespace FBX2MDL
{
	class FBXImporter;
	class FBXExporter;
	class Scene;
	class Node;
	class Mesh;

	static asd::Matrix44 ToAce(FbxAMatrix& mat)
	{
		asd::Matrix44 mat_;

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
		asd::astring Name;
		std::vector<std::shared_ptr<Node>> Children;

		std::shared_ptr<Mesh> MeshParameter;

		asd::RotationOrder RotationOrder;
		float LclTranslation[3];
		float LclRotation[4];
		float LclScaling[3];

		asd::Matrix44 GeometricMatrix;
		asd::Matrix44 LclMatrix;
	};

	struct KeyFrame
	{
		asd::Vector2DF KeyValue;
		asd::Vector2DF LeftPosition;
		asd::Vector2DF RightPosition;
		int32_t Interpolation;
	};

	struct KeyFrameAnimation
	{
		asd::astring TargetName;
		std::vector<KeyFrame> KeyFrames;
	};

	struct AnimationSource
	{
		asd::astring Name;
		std::vector<KeyFrameAnimation> keyFrameAnimations;
		int32_t StartTime;
		int32_t StopTime;
	};

	struct Face
	{
		int32_t Index[3];
		int32_t MaterialIndex;

		Face()
		{
			Index[0] = -1;
			Index[1] = -1;
			Index[2] = -1;
			MaterialIndex = -1;
		}
	};

	struct BoneConnector
	{
		asd::astring	Name;
		asd::Matrix44	TransformMatrix;
		asd::Matrix44	TransformLinkMatrix;
		asd::Matrix44	OffsetMatrix;
	};

	struct Material
	{
		asd::astring	Name;
		asd::astring	DiffuseTexturePath;
	};

	class Mesh
	{
	private:

	public:
		asd::astring	Name;

		std::vector<asd::Model_IO::Vertex> Vertexes;
		std::vector<Face> Faces;

		std::vector<BoneConnector>	BoneConnectors;

		std::vector<Material> Materials;
	};

	class Scene
	{
	public:
		std::shared_ptr<Node> Root;
		std::vector<AnimationSource> AnimationSources;
	};
}
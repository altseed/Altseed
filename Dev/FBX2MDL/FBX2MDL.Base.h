
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

	struct Vertex
	{
		ace::Vector3DF Position;
		ace::Vector3DF Normal;
		ace::Vector3DF Binormal;
		ace::Vector2DF UV;
		ace::Vector2DF SubUV;

		ace::Color		Color;
		uint8_t			Weight[4];
		uint8_t			WeightIndexDivided[4];
		uint8_t			WeightIndexOriginal[4];

		Vertex()
		{
			Color = ace::Color(255, 255, 255, 255);

			Weight[0] = 255;
			Weight[1] = 0;
			Weight[2] = 0;
			Weight[3] = 0;

			WeightIndexDivided[0] = 0;
			WeightIndexDivided[1] = 0;
			WeightIndexDivided[2] = 0;
			WeightIndexDivided[3] = 0;

			WeightIndexOriginal[0] = 0;
			WeightIndexOriginal[1] = 0;
			WeightIndexOriginal[2] = 0;
			WeightIndexOriginal[3] = 0;
		}

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

			if (l.UV.X != r.UV.X) return l.UV.X < r.UV.X;
			if (l.UV.Y != r.UV.Y) return l.UV.Y < r.UV.Y;

			if (l.SubUV.X != r.SubUV.X) return l.SubUV.X < r.SubUV.X;
			if (l.SubUV.Y != r.SubUV.Y) return l.SubUV.Y < r.SubUV.Y;

			if (l.Color.R != r.Color.R) return l.Color.R < r.Color.R;
			if (l.Color.G != r.Color.G) return l.Color.G < r.Color.G;
			if (l.Color.B != r.Color.B) return l.Color.B < r.Color.B;
			if (l.Color.A != r.Color.A) return l.Color.A < r.Color.A;

			for (int32_t i = 0; i < 4; i++)
			{
				if (l.Weight[i] != r.Weight[i]) return l.Weight[i] < r.Weight[i];
				if (l.WeightIndexDivided[i] != r.WeightIndexDivided[i]) return l.WeightIndexDivided[i] < r.WeightIndexDivided[i];
				if (l.WeightIndexOriginal[i] != r.WeightIndexOriginal[i]) return l.WeightIndexOriginal[i] < r.WeightIndexOriginal[i];
			}

			return false;
		}
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
		ace::astring	Name;
		ace::Matrix44	TransformMatrix;
		ace::Matrix44	TransformLinkMatrix;
		ace::Matrix44	OffsetMatrix;
	};

	struct Material
	{
		ace::astring	Name;
		ace::astring	DiffuseTexturePath;
	};

	class Mesh
	{
	private:

	public:
		ace::astring	Name;

		std::vector<Vertex> Vertexes;
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
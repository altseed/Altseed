
#pragma once

#include "FBX2MDL.Base.h"

namespace FBX2MDL
{
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
}
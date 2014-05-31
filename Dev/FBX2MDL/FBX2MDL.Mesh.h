
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
	};

	struct Face
	{
		int32_t Index[3];
	};

	struct BoneConnector
	{
		ace::astring	Name;
	};

	class Mesh
	{
	private:

	public:
		ace::astring	Name;

		std::vector<Vertex> Vertexes;

		std::vector<BoneConnector>	BoneConnectors;
	};
}
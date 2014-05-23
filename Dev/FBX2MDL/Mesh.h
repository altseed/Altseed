#pragma once

#include "../ace_cpp/common/Math/ace.Vector3DF.h"
#include "../ace_cpp/common/Math/ace.Vector2DF.h"
#include "Material.h"

struct Vertex
{
	ace::Vector3DF position;
	ace::Vector3DF normal;
	ace::Vector3DF binormal;
	ace::Vector2DF uv;
	ace::Vector2DF subuv;

	int normalAddCount;
	int binormalAddCount;

	int color[4];
	uint8_t weight[4];
	uint8_t weightIndexDivided[4];
	uint8_t weightIndexOriginal[4];
	int weightPtr;

	bool operator == (const Vertex& o)
	{
		return position==o.position && normal==o.normal && uv == o.uv && subuv == o.subuv
		&& color[0]==o.color[0] && color[1] == o.color[1] && color[2] == o.color[2] && color[3] == o.color[3];
	}

	bool operator != (const Vertex& o)
	{
		return !(position==o.position && normal==o.normal && uv == o.uv && subuv == o.subuv
		&& color[0]==o.color[0] && color[1] == o.color[1] && color[2] == o.color[2] && color[3] == o.color[3]);
	}
};

struct Face
{
	int vertexIndex[3];
	int materialIndex;
};

struct FacialMaterial
{
	int faceNum;
	Material &materialRef;

	FacialMaterial(int facenum,Material &materialref)
		:materialRef(materialref)
	{
		faceNum = facenum;
	}

	FacialMaterial& operator=( const FacialMaterial &facialMaterial)
	{
		faceNum=facialMaterial.faceNum;
		materialRef=facialMaterial.materialRef;

		return *this;
	}

};

struct BoneAttachment
{
	uint8_t boneIndex[32];
	int faceNum;
};
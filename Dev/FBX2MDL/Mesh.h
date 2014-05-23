
#pragma once

#include <Math/ace.Vector3DF.h>
#include <Math/ace.Vector2DF.h>
#include "Material.h"

//頂点
struct Vertex
{
	//位置
	ace::Vector3DF position;

	//法線
	ace::Vector3DF normal;

	//従法線
	ace::Vector3DF binormal;

	//UV
	ace::Vector2DF uv;

	//補助UV
	ace::Vector2DF subuv;

	//法線加算回数
	int normalAddCount;

	//従法線加算回数
	int binormalAddCount;

	//頂点カラー
	int color[4];

	//頂点ウェイト
	uint8_t weight[4];

	//頂点ウェイトインデックス(分割済み)
	uint8_t weightIndexDivided[4];

	//頂点ウェイトインデックス(オリジナル)
	uint8_t weightIndexOriginal[4];

	//ウェイト配列ポインタ
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

//面
struct Face
{
	//頂点インデックス
	int vertexIndex[3];

	//面材質インデックス
	int materialIndex;
};

//面材質
struct FacialMaterial
{
	//面数
	int faceNum;

	//面材質への参照
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
#pragma once
#include <fbxsdk.h>
#include <vector>
#include "../ace_cpp/common/Utility/ace.BinaryWriter.h"
#include "../ace_cpp/common/Math/ace.Vector2DF.h"

#define EPS 0.0001

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
		return position==o.position && normal==o.normal && binormal == o.binormal && uv == o.uv && subuv == o.subuv
		&& color[0]==o.color[0] && color[1] == o.color[1] && color[2] == o.color[2] && color[3] == o.color[3];
	}

	bool operator != (const Vertex& o)
	{
		return !(position==o.position && normal==o.normal && binormal == o.binormal && uv == o.uv && subuv == o.subuv
		&& color[0]==o.color[0] && color[1] == o.color[1] && color[2] == o.color[2] && color[3] == o.color[3]);
	}
};

struct Face
{
	int vertexIndex[3];
};

struct Material
{
	int Type;
	std::string texture[3];
};

class MeshLoader
{
	std::vector<Vertex> _baseVertices;
	std::vector<Vertex> _vertices;
	std::vector<Face> _faces;
	std::vector<Material> _materials;

	void _loadPositions(FbxMesh* fbxMesh);

	ace::Vector3DF _loadNormal(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId);
	ace::Vector3DF _loadBinormal(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId);
	ace::Vector2DF _loadUV(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId,int polygonCount,int polygonVert);
	uint8_t* _loadColor(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId);
	void _loadWeight(FbxMesh* fbxMesh);

	void _loadVertices(FbxMesh* fbxMesh);

	void _loadFaceIndices(FbxMesh* fbxMesh);

	void _loadFaceMaterials(FbxMesh* fbxMesh);
	void _loadBoneAttachments(FbxMesh* fbxMesh);

	void _loadTextures(FbxMesh* fbxMesh);
public:
	MeshLoader();

	std::vector<Vertex> GetVertices();
	std::vector<Face> GetFaces();
	std::vector<Material> GetMaterials();

	void Load(FbxMesh* fbxMesh);
	void WriteVertices(ace::BinaryWriter* writer);
	void WriteFaces(ace::BinaryWriter* writer);
	void WriteFaceMaterials(ace::BinaryWriter* writer);
	void WriteBoneAttachments(ace::BinaryWriter* writer);
	void WriteMaterials(ace::BinaryWriter* writer);

};
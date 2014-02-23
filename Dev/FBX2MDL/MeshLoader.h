#pragma once
#include <fbxsdk.h>
#include <vector>
#include "../ace_cpp/common/Utility/ace.BinaryWriter.h"
#include "../ace_cpp/common/Math/ace.Vector2DF.h"

using namespace std;

struct Vertex
{
	ace::Vector3DF position;
	ace::Vector3DF normal;
	ace::Vector3DF binormal;
	ace::Vector2DF uv;
	ace::Vector2DF subuv;

	int color[4];
	uint8_t weight[4];
	uint8_t weightIndexDivided[4];
	uint8_t weightIndexOriginal[4];

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
	string texture[3];
};

class MeshLoader
{
	vector<Vertex> _baseVertices;
	vector<Vertex> _vertices;
	vector<Face> _faces;
	vector<Material> _materials;

	void _loadPositions(FbxMesh* fbxMesh);

	ace::Vector3DF _loadNormal(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId);
	ace::Vector3DF _loadBinormal(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId);
	ace::Vector2DF _loadUV(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId,int i,int j);
	uint8_t* _loadColor(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId);
	void _loadWeight(FbxMesh* fbxMesh);

	void _loadVertices(FbxMesh* fbxMesh);

	void _loadFaceIndices(FbxMesh* fbxMesh);

	void _loadFaceMaterials(FbxMesh* fbxMesh);
	void _loadBoneAttachments(FbxMesh* fbxMesh);

	void _loadTextures(FbxMesh* fbxMesh);
public:
	MeshLoader();

	vector<Vertex> GetVertices();
	vector<Face> GetFaces();
	vector<Material> GetMaterials();

	void Load(FbxMesh* fbxMesh);
	void WriteVertices(ace::BinaryWriter* writer);
	void WriteFaces(ace::BinaryWriter* writer);
	void WriteFaceMaterials(ace::BinaryWriter* writer);
	void WriteBoneAttachments(ace::BinaryWriter* writer);
	void WriteMaterials(ace::BinaryWriter* writer);

};
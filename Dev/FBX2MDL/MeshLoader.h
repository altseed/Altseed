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

	float color[4];
	uint8_t weight[4];
	uint8_t weightIndexDivided[4];
	uint8_t weightIndexOriginal[4];
};

struct Face
{
	int vertexIndex[3];
};

class MeshLoader
{
	vector<Vertex> _vertices;
	vector<Face> _faces;

	void _loadPositions(FbxMesh* fbxMesh);
	void _loadNormals(FbxMesh* fbxMesh);
	void _loadBinormals(FbxMesh* fbxMesh);
	void _loadUVs(FbxMesh* fbxMesh);
	void _loadColors(FbxMesh* fbxMesh);
	void _loadWeights(FbxMesh* fbxMesh);

	void _loadFaceIndices(FbxMesh* fbxMesh);

	void _loadFaceMaterials(FbxMesh* fbxMesh);
	void _loadBoneAttachments(FbxMesh* fbxMesh);
public:
	MeshLoader();

	vector<Vertex> GetVertices();
	vector<Face> GetFaces();

	void Load(FbxMesh* fbxMesh);
	void WriteVertices(ace::BinaryWriter* writer);
	void WriteFaces(ace::BinaryWriter* writer);
	void WriteFaceMaterials(ace::BinaryWriter* writer);
	void WriteBoneAttachments(ace::BinaryWriter* writer);

};
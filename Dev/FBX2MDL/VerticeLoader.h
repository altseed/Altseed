#pragma once
#include <fbxsdk.h>
#include "Vertex.h"
#include <vector>

using namespace std;

class VerticeLoader
{
	FbxMesh* _fbxMesh;
	vector<Vertex> _vertices;

	void _loadPositions();
	void _loadNormals();
	void _loadBinormals();
	void _loadUVs();
	void _loadColors();
	void _loadWeights();
public:
	VerticeLoader(FbxMesh* _fbxMesh);

	vector<Vertex> GetVertices();

};
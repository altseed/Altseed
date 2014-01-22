#pragma once

#include <fbxsdk.h>
#include <vector>

#include "Vertex.h"

using namespace std;

class Mesh
{
	FbxNode *_fbxNode;
	vector<Vertex> _vertices;

	void _loadVertices();
public:
	Mesh(FbxNode* fbxNode);

};
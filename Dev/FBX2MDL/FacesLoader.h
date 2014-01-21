#pragma once
#include <fbxsdk.h>
#include "Face.h"
#include <vector>

using namespace std;

class FacesLoader
{
	FbxMesh* _fbxMesh;
	vector<Face> _faces;

	void _loadIndices();
public:
	FacesLoader(FbxMesh* fbxMesh);

	vector<Face> GetFaces();
};
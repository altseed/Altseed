#pragma once
#include <fbxsdk.h>
#include "Face.h"
#include <vector>
#include "../ace_cpp/common/Utility/ace.BinaryWriter.h"

using namespace std;

class FacesLoader
{
	FbxMesh* _fbxMesh;
	vector<Face> _faces;

	void _loadIndices();
public:
	FacesLoader(FbxMesh* fbxMesh);

	vector<Face> GetFaces();
	void Load();
	void Write(ace::BinaryWriter* writer);
};
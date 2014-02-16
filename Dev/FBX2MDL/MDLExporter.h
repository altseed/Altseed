#pragma once

#include <fbxsdk.h>
#include "../ace_cpp/common/Utility/ace.BinaryWriter.h"
#include "MeshLoader.h"

class MDLExporter
{
	FbxManager* lSdkManager;
	FbxScene* lScene;
	ace::BinaryWriter* binaryWriter;
	MeshLoader* mLoader;

	void GetMesh(FbxNode* pNode);

	void GetMeshProperty(FbxNodeAttribute* pAttribute);

	void PrintHeader();

	void GetMeshGroup(FbxNode* pNode);
public:
	MDLExporter(){}

	MDLExporter(const char* fileName);

	void Convert();

	~MDLExporter();
};
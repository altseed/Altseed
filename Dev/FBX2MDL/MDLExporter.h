#pragma once

#include <fbxsdk.h>
#include "../ace_cpp/common/Utility/ace.BinaryWriter.h"

class MDLExporter
{
	FbxManager* lSdkManager;
	FbxScene* lScene;
	ace::BinaryWriter* binaryWriter;

	void GetMesh(FbxNode* pNode);

	void GetMeshProperty(FbxNodeAttribute* pAttribute);

	void PrintHeader();

	void GetMeshGroup(FbxNode* pNode);

	void GetVertice();
public:
	MDLExporter(){}

	MDLExporter(const char* fileName);

	void Convert();

	~MDLExporter();
};
#pragma once

#include <fbxsdk.h>
#include "../ace_cpp/common/Utility/ace.BinaryWriter.h"
#include "Deformer.h"
#include "MeshLoader.h"

class MDLExporter
{
	FbxManager* lSdkManager;
	FbxScene* lScene;
	ace::BinaryWriter* binaryWriter;
	std::vector<MeshLoader> _meshGroup;
	DeformerManager deformerManager;

	void PrintHeader();

	void GetMeshGroup(FbxNode* pNode);
	void GetMeshProperty(FbxNode* node);

	void GetDeformer(Deformer* parentSkeleton, FbxNode* pNode);
	void GetDeformerProperty(Deformer* parentSkeleton, FbxNode* node, Deformer &skeleton);
public:
	MDLExporter(){}

	MDLExporter(const char* fileName);

	void Convert();

	~MDLExporter();
};
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
	std::vector<MeshGroup> _meshGroups;

	void PrintHeader();

	void GetMeshGroup(FbxNode* pNode,int depth);
	void GetMeshProperty(FbxNode* node);

	void GetDeformer(Deformer* parentSkeleton, FbxNode* pNode,DeformerManager &deformerManager);
	void GetDeformerProperty(Deformer* parentSkeleton, FbxNode* node,Deformer *deformer, DeformerManager &deformerManager);
public:
	MDLExporter(){}

	MDLExporter(const char* fileName);

	void Convert();

	~MDLExporter();
};
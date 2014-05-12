#pragma once

#include <fbxsdk.h>
#include "../ace_cpp/common/Utility/ace.BinaryWriter.h"
#include "Deformer.h"
#include "MeshLoader.h"

#include "Animation.h"

class MDLExporter
{
	FbxManager* lSdkManager;
	FbxScene* lScene;
	ace::BinaryWriter* binaryWriter;
	std::vector<MeshGroup> _meshGroups;
	std::vector<AnimationSource> _animationSources;

	void PrintHeader();

	void GetMeshGroup(FbxNode* pNode,int depth);
	void GetMeshProperty(FbxNode* node);

	void GetDeformer(Deformer* parentSkeleton, FbxNode* pNode,DeformerManager &deformerManager);
	void GetDeformerProperty(Deformer* parentSkeleton, FbxNode* node,Deformer *deformer, DeformerManager &deformerManager);

	void GetMotion(FbxNode *parentNode,FbxNode* node,FbxAnimLayer* pLayer,AnimationSource &animationSource);

	void AnimStackAnalyze(FbxAnimStack* pStack, FbxNode *rootNode,AnimationSource &animationSource);

	void GetSkeletonCurve(FbxNode* fbxNode,FbxAnimLayer* fbxAnimLayer,AnimationSource &animationSource);
	void AnalyzeCurve(std::string target,FbxAnimCurve* pCurve,AnimationSource &animationSource);
public:
	MDLExporter(){}

	MDLExporter(const char* fileName);

	void Convert();

	~MDLExporter();
};
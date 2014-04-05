#include "MDLExporter.h"
#include "../ace_cpp/common/Math/ace.Vector2DF.h"
#include "../ace_cpp/common/Math/ace.Vector3DF.h"
#include "../ace_cpp/common/Math/ace.Matrix44.h"

#include <iostream>

MDLExporter::MDLExporter(const char* fileName){
	// Initialize the SDK manager. This object handles all our memory management.
	lSdkManager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(fileName, -1, lSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	// Create a new scene so that it can be populated by the imported file.
	lScene = FbxScene::Create(lSdkManager, "myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported; so get rid of the importer.
	lImporter->Destroy();

	binaryWriter = new ace::BinaryWriter();

}

void MDLExporter::Convert()
{
	PrintHeader();
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) {

		//ボーンリスト
		{
			std::vector<int> armatureIndices;
			for(int i=0;i<lRootNode->GetChildCount();++i)
			{
				if(lRootNode->GetChild(i)->GetNodeAttribute()->GetAttributeType()==FbxNodeAttribute::eSkeleton)
				{
					armatureIndices.push_back(i);
				}
			}

			_meshGroups.resize((int32_t)armatureIndices.size());

			int armaturePtr=0;
			for (int i = 0; i < (int)armatureIndices.size(); i++)
			{
				Deformer::ResetIndexCount();
				GetDeformer(NULL,lRootNode->GetChild(armatureIndices[i]),_meshGroups[armaturePtr++].deformerManager);
			}
		}

		//アニメーション
		{
			const int stackCount = lScene->GetSrcObjectCount<FbxAnimStack>();

			for(int i=0;i<stackCount;++i)
			{
				AnimationSource animationSource;
				FbxAnimStack *pStack = lScene->GetSrcObject<FbxAnimStack>(i);
				for(int j=0;j<lRootNode->GetChildCount();++j)
				{
					AnimStackAnalyze(pStack,lRootNode->GetChild(j),animationSource);
				}
				_animationSources.push_back(animationSource);
			}
		}

		//メッシュ一覧
		{
			for (int i = 0; i < lRootNode->GetChildCount(); i++)
				GetMeshGroup(lRootNode->GetChild(i),0);
		}
	}


	//出力
	{
		int meshGroupNum = (int) _meshGroups.size();

		//printf("Mesh Group Num = %d\n", meshGroupNum);

		//メッシュ
		binaryWriter->Push(meshGroupNum);

		for (int i = 0; i < meshGroupNum; ++i)
		{
			MeshGroup meshGroup=_meshGroups[i];

			binaryWriter->Push((int32_t)meshGroup.meshLoaders.size());
			for(int j=0;j<meshGroup.meshLoaders.size();++j)
			{
				meshGroup.meshLoaders[j].WriteVertices(binaryWriter);
				meshGroup.meshLoaders[j].WriteFaces(binaryWriter);
				meshGroup.meshLoaders[j].WriteFaceMaterials(binaryWriter);
				meshGroup.meshLoaders[j].WriteBoneAttachments(binaryWriter);
			}
			//ボーン
			binaryWriter->Push((int32_t)meshGroup.deformerManager.GetDeformerNum());

			for(int j=0;j<meshGroup.deformerManager.GetDeformerNum();++j)
			{
				Deformer *deformer = meshGroup.deformerManager.GetDeformerByIndex(j);

				binaryWriter->Push(ace::ToAString(deformer->name.c_str()));
				binaryWriter->Push(deformer->parentIndex);
				binaryWriter->Push(deformer->rotationOrder);
				binaryWriter->Push(deformer->relationMatrix);
				binaryWriter->Push(deformer->invMatrix);
			}

			//材質
			for(int j=0;j<meshGroup.meshLoaders.size();++j)
			{
				meshGroup.meshLoaders[j].WriteMaterials(binaryWriter);
			}
		}
	}

	{
		//アニメーションソース
		binaryWriter->Push(0);
	}

	{
		//アニメーションクリップ
		binaryWriter->Push(0);
	}


	binaryWriter->WriteOut("out.mdl");

}

MDLExporter::~MDLExporter()
{
	lSdkManager->Destroy();

	delete binaryWriter;
}

void MDLExporter::GetMeshGroup(FbxNode* pNode,int depth)
{
	// Print the node's attributes.

	GetMeshProperty(pNode);

	for (int j = 0; j < pNode->GetChildCount(); j++)
		GetMeshGroup(pNode->GetChild(j),depth+1);
}

void MDLExporter::GetMeshProperty(FbxNode* node)
{
	if(node->GetNodeAttribute()->GetAttributeType()!=FbxNodeAttribute::eMesh) return;

	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
	{
		FbxNodeAttribute *pAttribute = node->GetNodeAttributeByIndex(i);
		FbxMesh* mesh = (FbxMesh*) pAttribute;

		if (!mesh->IsTriangleMesh())
		{
			FbxGeometryConverter _converter(lSdkManager);
			mesh = (FbxMesh*) _converter.Triangulate(mesh, true);
		} 

		MeshLoader mLoader;

		int attachmentIndex;

		mLoader.Load(mesh,attachmentIndex,_meshGroups);

		if(attachmentIndex!=-1)
		{
			_meshGroups[attachmentIndex].meshLoaders.push_back(mLoader);
		}
		else
		{
			MeshGroup meshGroup;

			meshGroup.meshLoaders.push_back(mLoader);

			_meshGroups.push_back(meshGroup);
		}

	}

}

void MDLExporter::GetDeformer(Deformer* parentSkeleton, FbxNode* pNode,DeformerManager &deformerManager)
{
	Deformer *deformer;
	deformer=new Deformer();
	GetDeformerProperty(parentSkeleton,pNode,deformer,deformerManager);

	deformerManager.AddDeformer(deformer);

	for(int j=0;j<pNode->GetChildCount();++j)
	{
		GetDeformer(deformer,pNode->GetChild(j),deformerManager);
	}
}

void MDLExporter::AnimStackAnalyze(FbxAnimStack* pStack, FbxNode *rootNode,AnimationSource &animationSource)
{
	const int layerCount = pStack->GetMemberCount<FbxAnimLayer>();

	if(layerCount>0)
	{
		const char* animationName = pStack->GetName();

		FbxTime startTime = pStack->LocalStart;
		FbxTime endTime = pStack->LocalStop;

		AnimationSource animationSource;
		animationSource.animationName=std::string(animationName);
		animationSource.startTime=startTime;
		animationSource.stopTime=endTime;

		_animationSources.push_back(animationSource);
	}

	for(int i=0;i<layerCount;++i)
	{
		FbxAnimLayer *pLayer = pStack->GetMember<FbxAnimLayer>();
		GetMotion(NULL,rootNode,pLayer,animationSource);
		break;
	}
}

void MDLExporter::GetMotion(FbxNode *parentNode,FbxNode* node,FbxAnimLayer* pLayer,AnimationSource &animationSource)
{
	if(node->GetNodeAttribute()->GetAttributeType()==FbxNodeAttribute::eSkeleton)
	{
		GetSkeletonCurve(node,pLayer,animationSource);
	}

	for(int i=0;i<node->GetChildCount();++i)
	{
		GetMotion(node,node->GetChild(i),pLayer,animationSource);
	}
}

void MDLExporter::GetSkeletonCurve(FbxNode* fbxNode,FbxAnimLayer* fbxAnimLayer,AnimationSource &animationSource)
{
	std::string boneName = fbxNode->GetName();

	AnalyzeCurve(boneName+".pos.x",fbxNode->LclTranslation.GetCurve(fbxAnimLayer,FBXSDK_CURVENODE_COMPONENT_X),animationSource);
	AnalyzeCurve(boneName+".pos.y",fbxNode->LclTranslation.GetCurve(fbxAnimLayer,FBXSDK_CURVENODE_COMPONENT_Y),animationSource);
	AnalyzeCurve(boneName+".pos.z",fbxNode->LclTranslation.GetCurve(fbxAnimLayer,FBXSDK_CURVENODE_COMPONENT_Z),animationSource);

	AnalyzeCurve(boneName+".rot.x",fbxNode->LclRotation.GetCurve(fbxAnimLayer,FBXSDK_CURVENODE_COMPONENT_X),animationSource);
	AnalyzeCurve(boneName+".rot.y",fbxNode->LclRotation.GetCurve(fbxAnimLayer,FBXSDK_CURVENODE_COMPONENT_Y),animationSource);
	AnalyzeCurve(boneName+".rot.z",fbxNode->LclRotation.GetCurve(fbxAnimLayer,FBXSDK_CURVENODE_COMPONENT_Z),animationSource);

	AnalyzeCurve(boneName+".scl.x",fbxNode->LclScaling.GetCurve(fbxAnimLayer,FBXSDK_CURVENODE_COMPONENT_X),animationSource);
	AnalyzeCurve(boneName+".scl.y",fbxNode->LclScaling.GetCurve(fbxAnimLayer,FBXSDK_CURVENODE_COMPONENT_Y),animationSource);
	AnalyzeCurve(boneName+".scl.z",fbxNode->LclScaling.GetCurve(fbxAnimLayer,FBXSDK_CURVENODE_COMPONENT_Z),animationSource);
}

void MDLExporter::AnalyzeCurve(std::string target,FbxAnimCurve* pCurve,AnimationSource &animationSource)
{
	KeyFrameAnimation keyFrameAnimation;
	keyFrameAnimation.targetName=target;

	const int keyCount = pCurve->KeyGetCount();

	int hour,minute,second,frame,field,residual;

	for(int i=0;i<keyCount;++i)
	{
		float value = pCurve->KeyGetValue(i);
		FbxTime time = pCurve->KeyGetTime(i);
		int interpolation = pCurve->KeyGetInterpolation(i);
		time.GetTime(hour,minute,second,frame,field,residual,FbxTime::eFrames60);

		KeyFrame keyFrame;
		keyFrame.keyValue=ace::Vector2DF(value,(hour * 60 * 60 * 60) + (minute * 60 * 60) + (second * 60) + frame);
		keyFrame.interpolation=interpolation;
		keyFrameAnimation.keyFrames.push_back(keyFrame);
	}
	animationSource.keyFrameAnimations.push_back(keyFrameAnimation);
}

void MDLExporter::GetDeformerProperty(Deformer* parentSkeleton, FbxNode* node,Deformer *deformer, DeformerManager &deformerManager)
{
	if(node->GetNodeAttribute()->GetAttributeType()!=FbxNodeAttribute::eSkeleton) return;


	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
	{
		FbxSkeleton *sk = (FbxSkeleton*) node->GetNodeAttributeByIndex(i);
		deformer->name = std::string(sk->GetNode()->GetName());

		deformer->parentIndex = (parentSkeleton == NULL) ? -1 : parentSkeleton->index;

		FbxAMatrix transform = node->EvaluateGlobalTransform();
		FbxAMatrix invMatrix = transform.Inverse();

		FbxAMatrix parentTransform = node->GetParent()->EvaluateGlobalTransform();
		FbxAMatrix invParentMatrix = parentTransform.Inverse();

		FbxAMatrix relationMatrix = invParentMatrix * transform;

		for(int j=0;j<4;++j)
		{
			for(int k=0;k<4;++k)
			{
				deformer->invMatrix.Values[j][k]=invMatrix.Get(j,k);
				deformer->relationMatrix.Values[j][k]=relationMatrix.Get(j,k);
			}
		}

		fbxsdk_2014_2_1::EFbxRotationOrder fbxRotationOrder;
		node->GetRotationOrder(FbxNode::eSourcePivot, fbxRotationOrder);


		switch(fbxRotationOrder)
		{
		case eEulerXYZ :
			deformer->rotationOrder=12;	break ;
		case eEulerXZY :
			deformer->rotationOrder=11;	break ;
		case eEulerYZX :
			deformer->rotationOrder=16;	break ;
		case eEulerYXZ :
			deformer->rotationOrder=15;	break ;
		case eEulerZXY :
			deformer->rotationOrder=13;	break ;
		case eEulerZYX :
			deformer->rotationOrder=14;	break ;
		case eSphericXYZ :
			break ;	
		}
	}
}

void MDLExporter::PrintHeader()
{
	binaryWriter->Push((uint8_t)'M');
	binaryWriter->Push((uint8_t)'D');
	binaryWriter->Push((uint8_t)'L');
	binaryWriter->Push((uint8_t) 0);

	binaryWriter->Push(1);
}
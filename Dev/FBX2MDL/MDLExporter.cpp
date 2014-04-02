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
				GetDeformer(NULL,lRootNode->GetChild(armatureIndices[i]),_meshGroups[armaturePtr++].deformerManager);
			}
		}

		//メッシュ一覧
		{
			for (int i = 0; i < lRootNode->GetChildCount(); i++)
				GetMeshGroup(lRootNode->GetChild(i),0);
		}
	}


	//メッシュグループ
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
			binaryWriter->Push(0);
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
	Deformer *deformer=NULL;
	GetDeformerProperty(parentSkeleton,pNode,deformer,deformerManager);

	for(int j=0;j<pNode->GetChildCount();++j)
	{
		GetDeformer(deformer,pNode->GetChild(j),deformerManager);
	}
}

void MDLExporter::GetDeformerProperty(Deformer* parentSkeleton, FbxNode* node,Deformer *deformer, DeformerManager &deformerManager)
{
	if(node->GetNodeAttribute()->GetAttributeType()!=FbxNodeAttribute::eSkeleton) return;

	deformer=new Deformer();

	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
	{
		FbxSkeleton *sk = (FbxSkeleton*) node->GetNodeAttributeByIndex(i);
		deformer->name = std::string(sk->GetNode()->GetName());

		deformer->parentIndex = (parentSkeleton == NULL) ? -1 : parentSkeleton->index;

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

	deformerManager.AddDeformer(deformer);
}

void MDLExporter::PrintHeader()
{
	binaryWriter->Push((uint8_t)'M');
	binaryWriter->Push((uint8_t)'D');
	binaryWriter->Push((uint8_t)'L');
	binaryWriter->Push((uint8_t) 0);

	binaryWriter->Push(1);
}
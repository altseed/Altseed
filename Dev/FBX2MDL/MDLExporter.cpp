#include "MDLExporter.h"
#include "../ace_cpp/common/Math/ace.Vector2DF.h"
#include "../ace_cpp/common/Math/ace.Vector3DF.h"
#include "../ace_cpp/common/Math/ace.Matrix44.h"

#include "MeshLoader.h"
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

	_deformerManager = DeformerManager();

}

void MDLExporter::Convert()
{
	PrintHeader();
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) {

		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			GetDeformer(NULL,lRootNode->GetChild(i));

		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			GetMeshGroup(lRootNode->GetChild(i));
	}


	//メッシュグループ
	{
		int meshGroupNum = (int) _meshGroup.size();

		printf("Mesh Group Num = %d\n", meshGroupNum);

		//メッシュ
		binaryWriter->Push(meshGroupNum);

		for (int i = 0; i < meshGroupNum; ++i)
		{
			_meshGroup[i].WriteVertices(binaryWriter);
			_meshGroup[i].WriteFaces(binaryWriter);
			_meshGroup[i].WriteFaceMaterials(binaryWriter);
			_meshGroup[i].WriteBoneAttachments(binaryWriter);

			//ボーン
			binaryWriter->Push(0);
			//材質
			_meshGroup[i].WriteMaterials(binaryWriter);
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

void MDLExporter::GetMeshGroup(FbxNode* pNode)
{
	if(pNode->GetNodeAttribute()->GetAttributeType()!=FbxNodeAttribute::eMesh) return;
	// Print the node's attributes.

	GetMeshProperty(pNode);


	for (int j = 0; j < pNode->GetChildCount(); j++)
		GetMeshGroup(pNode->GetChild(j));
}

void MDLExporter::GetMeshProperty(FbxNode* node)
{
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
	{
		FbxNodeAttribute *pAttribute = node->GetNodeAttributeByIndex(i);
		FbxMesh* mesh = (FbxMesh*) pAttribute;

		if (!mesh->IsTriangleMesh())
		{
			FbxGeometryConverter _converter(lSdkManager);
			mesh = (FbxMesh*) _converter.Triangulate(mesh, true);
		}

		MeshLoader mLoader = MeshLoader(_deformerManager);

		mLoader.Load(mesh);

		_meshGroup.push_back(mLoader);
	}

}

void MDLExporter::GetDeformer(Deformer* parentSkeleton, FbxNode* pNode)
{
	if(pNode->GetNodeAttribute()->GetAttributeType()!=FbxNodeAttribute::eSkeleton) return;

	Deformer* deformer = new Deformer();
	GetDeformerProperty(parentSkeleton,pNode,deformer);

	_deformerManager.AddDeformer(deformer);

	for(int j=0;j<pNode->GetChildCount();++j)
	{
		GetDeformer(deformer,pNode->GetChild(j));
	}
}

void MDLExporter::GetDeformerProperty(Deformer* parentSkeleton, FbxNode* node, Deformer *skeleton)
{
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
	{
		FbxSkeleton *sk = (FbxSkeleton*) node->GetNodeAttributeByIndex(i);
		skeleton->name = std::string(sk->GetNode()->GetName());

		skeleton->parentIndex = (parentSkeleton == NULL) ? -1 : parentSkeleton->index;

		fbxsdk_2014_2_1::EFbxRotationOrder fbxRotationOrder;
		node->GetRotationOrder(FbxNode::eSourcePivot, fbxRotationOrder);

		
		switch(fbxRotationOrder)
		{
		case eEulerXYZ :
			skeleton->rotationOrder=12;	break ;
		case eEulerXZY :
			skeleton->rotationOrder=11;	break ;
		case eEulerYZX :
			skeleton->rotationOrder=16;	break ;
		case eEulerYXZ :
			skeleton->rotationOrder=15;	break ;
		case eEulerZXY :
			skeleton->rotationOrder=13;	break ;
		case eEulerZYX :
			skeleton->rotationOrder=14;	break ;
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
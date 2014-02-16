#include "MDLExporter.h"
#include "../ace_cpp/common/Math/ace.Vector2DF.h"
#include "../ace_cpp/common/Math/ace.Vector3DF.h"
#include "../ace_cpp/common/Math/ace.Matrix44.h"

#include "MeshLoader.h"

MDLExporter::MDLExporter(const char* fileName){
	// Initialize the SDK manager. This object handles all our memory management.
    lSdkManager = FbxManager::Create();
    
    // Create the IO settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
    
    // Use the first argument as the filename for the importer.
    if(!lImporter->Initialize(fileName, -1, lSdkManager->GetIOSettings())) { 
        printf("Call to FbxImporter::Initialize() failed.\n"); 
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString()); 
        exit(-1); 
    }
    
    // Create a new scene so that it can be populated by the imported file.
    lScene = FbxScene::Create(lSdkManager,"myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file is imported; so get rid of the importer.
    lImporter->Destroy();

	binaryWriter=new ace::BinaryWriter();

	mLoader=new MeshLoader();

}

void MDLExporter::Convert()
{
	PrintHeader();
	FbxNode* lRootNode = lScene->GetRootNode();
    if(lRootNode) {
        for(int i = 0; i < lRootNode->GetChildCount(); i++)
			GetMeshGroup(lRootNode->GetChild(i));
    }
	
	binaryWriter->Push(1);
	//バージョン
	{

		mLoader->WriteVertices(binaryWriter);
		mLoader->WriteFaces(binaryWriter);
		mLoader->WriteFaceMaterials(binaryWriter);
		mLoader->WriteBoneAttachments(binaryWriter);

		binaryWriter->Push(0);

		binaryWriter->Push(0);
	}
		/*
		binaryWriter->Push("");
		binaryWriter->Push(0);
		binaryWriter->Push(10);

		ace::Matrix44 matrix=ace::Matrix44();
		binaryWriter->Push(matrix);
		binaryWriter->Push(matrix);
		*/

		
		/*
		binaryWriter->Push(1);
		binaryWriter->Push("");
		*/

	{
		binaryWriter->Push(0);
		binaryWriter->Push(0);
	}


	binaryWriter->WriteOut("out.mdl");

}

MDLExporter::~MDLExporter()
{
	lSdkManager->Destroy();

	delete binaryWriter;

	delete mLoader;
}

void MDLExporter::GetMeshGroup(FbxNode* pNode)
{
    // Print the node's attributes.
    for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
		GetMeshProperty(pNode->GetNodeAttributeByIndex(i));


    for(int j = 0; j < pNode->GetChildCount(); j++)
        GetMeshGroup(pNode->GetChild(j));
}

void MDLExporter::GetMesh(FbxNode* pNode)
{
	GetMeshProperty(pNode->GetNodeAttribute());
}

void MDLExporter::GetMeshProperty(FbxNodeAttribute* pAttribute)
{
	//メッシュじゃない
	if(pAttribute->GetAttributeType()!=FbxNodeAttribute::eMesh) return;

	FbxMesh* mesh=(FbxMesh*)pAttribute;

	mLoader->Load(mesh);
}

void MDLExporter::PrintHeader()
{
	binaryWriter->Push((uint8_t)'M');
	binaryWriter->Push((uint8_t)'D');
	binaryWriter->Push((uint8_t)'L');
	binaryWriter->Push((uint8_t)0);

	binaryWriter->Push(1);
}
#include "MDLExporter.h"
#include "../ace_cpp/common/Math/ace.Vector2DF.h"
#include "../ace_cpp/common/Math/ace.Vector3DF.h"


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

}

void MDLExporter::Convert()
{
	PrintHeader();
	FbxNode* lRootNode = lScene->GetRootNode();
    if(lRootNode) {
        for(int i = 0; i < lRootNode->GetChildCount(); i++)
            PrintMeshGroup(lRootNode->GetChild(i));
    }
}

MDLExporter::~MDLExporter()
{
	lSdkManager->Destroy();

	delete binaryWriter;
}


void MDLExporter::PrintMeshProperty(FbxNodeAttribute* pAttribute) {
	if(!pAttribute&&pAttribute->GetAttributeType()!=FbxNodeAttribute::EType::eMesh) return;
 
	FbxNodeAttribute::EType typeName=pAttribute->GetAttributeType();
    FbxString attrName = pAttribute->GetName();

	FbxMesh *pMesh=(FbxMesh*)pAttribute;

	int controlNum = pMesh->GetControlPointsCount();

	ace::Vector3DF* controlAry = new ace::Vector3DF[controlNum];

	FbxVector4* src = pMesh->GetControlPoints();

	for(int i=0;i<controlNum;++i)
	{
		controlAry[i].X=src[i][0]/src[i][3];
		controlAry[i].Y=src[i][1]/src[i][3];
		controlAry[i].Z=src[i][2]/src[i][3];
	}

	//頂点/法線・従法線
	FbxLayerElementArrayTemplate<FbxVector4>* normalElement;
	FbxLayerElementArrayTemplate<FbxVector4>* binormalElement;

	pMesh->GetNormals(&normalElement);
	pMesh->GetBinormals(&binormalElement);

	ace::Vector3DF* normalBuffer=new ace::Vector3DF[normalElement->GetCount()];
	ace::Vector3DF* binormalBuffer = new ace::Vector3DF[binormalElement->GetCount()];


	for(int i=0;i<normalElement->GetCount();++i)
	{
		normalBuffer[i].X=(float)normalElement->GetAt(i)[0];
		normalBuffer[i].Y=(float)normalElement->GetAt(i)[1];
		normalBuffer[i].Z=(float)normalElement->GetAt(i)[2];
	}

	for(int i=0;i<binormalElement->GetCount();++i)
	{
		binormalBuffer[i].X=(float)binormalElement->GetAt(i)[0];
		binormalBuffer[i].Y=(float)binormalElement->GetAt(i)[1];
		binormalBuffer[i].Z=(float)binormalElement->GetAt(i)[2];
	}

	//UV
	int layerCount=pMesh->GetLayerCount();
	FbxLayerElementUV* elem;
	for(int i=0;i<layerCount;++i)
	{
		FbxLayer* layer = pMesh->GetLayer(i);
		elem=layer->GetUVs();
		if(elem==0) continue;

		int UVNum = elem->GetDirectArray().GetCount();
		int indexNum = elem->GetIndexArray().GetCount();
		int size= UVNum > indexNum ? UVNum : indexNum;
		ace::Vector2DF* buffer = new ace::Vector2DF[size];

		FbxLayerElement::EMappingMode mappingMode = elem->GetMappingMode();
		FbxLayerElement::EReferenceMode refMode = elem->GetReferenceMode();

		if(mappingMode == FbxLayerElement::eByPolygonVertex&&refMode == FbxLayerElement::eDirect)
		{
			for(int i=0;i<size;++i)
			{
				buffer[i].X=(float)elem->GetDirectArray().GetAt(i)[0];
				buffer[i].Y=(float)elem->GetDirectArray().GetAt(i)[1];
			}
		}
		else if(refMode==FbxLayerElement::eIndexToDirect)
		{
			for(int i=0;i<size;++i)
			{
				int index = elem->GetIndexArray().GetAt(i);
				buffer[i].X=(float)elem->GetDirectArray().GetAt(index)[0];
				buffer[i].Y=(float)elem->GetDirectArray().GetAt(index)[1];
			}
		}

	}

	//頂点カラー



	//ポリゴン
	int PolygonNum = pMesh->GetPolygonCount();
	int PolygonVertexNum = pMesh->GetPolygonVertexCount();
	int *IndexAry = pMesh->GetPolygonVertices();

	for(int p=0;p<PolygonNum;++p)
	{
		int IndexNumInPolygon = pMesh->GetPolygonSize(p);
		for(int n=0;n<IndexNumInPolygon;++n)
		{
			int IndexNumber = pMesh->GetPolygonVertex(p,n);
		}
	}
}

void MDLExporter::PrintMeshGroup(FbxNode* pNode)
{
	//メッシュの出力
	PrintMesh(pNode);
	//ボーンの出力

	//材質の出力
}

void MDLExporter::PrintMesh(FbxNode* pNode) {
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get(); 
    FbxDouble3 rotation = pNode->LclRotation.Get(); 
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the node's attributes.
    for(int i = 0; i < pNode->GetNodeAttributeCount(); i++)
        PrintMeshProperty(pNode->GetNodeAttributeByIndex(i));

    // Recursively print the children.
    for(int j = 0; j < pNode->GetChildCount(); j++)
        PrintMesh(pNode->GetChild(j));

}

void MDLExporter::PrintHeader()
{
	binaryWriter->Push("MDL");
	binaryWriter->Push((int32_t)0);
}
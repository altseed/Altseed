#include "MeshLoader.h"


MeshLoader::MeshLoader()
{

}

vector<Vertex> MeshLoader::GetVertices()
{
	return _vertices;
}

void MeshLoader::_loadPositions(FbxMesh* fbxMesh)
{

	int controlNum = fbxMesh->GetControlPointsCount();

	FbxVector4* src = fbxMesh->GetControlPoints();

	for(int i=0;i<controlNum;++i)
	{
		Vertex vertex;
		ace::Vector3DF controlAry;
		controlAry.X=src[i][0];
		controlAry.Y=src[i][1];
		controlAry.Z=src[i][2];
		vertex.position=controlAry;

		_vertices.push_back(vertex);
	}
}

void MeshLoader::_loadNormals(FbxMesh* fbxMesh)
{
	FbxLayerElementNormal* ElementNormal = fbxMesh->GetLayer(0)->GetNormals();

	if(ElementNormal->GetMappingMode() == FbxLayerElement::eByPolygonVertex&&ElementNormal->GetReferenceMode() == FbxLayerElement::eDirect)
	{
		printf("PolyogonVertex Mapping\nDirect Reference\nNormal Count = %d\n",ElementNormal->GetDirectArray().GetCount());
		
		for (auto ite=_vertices.begin();ite!=_vertices.end();++ite)
		{
			ite->normal=ace::Vector3DF(0,0,0);
		}

		int faceIndexPtr=0;

		for ( int i = 0; i < ElementNormal->GetDirectArray().GetCount(); ++i ) {
			
			ace::Vector3DF vector;

			vector.X = (float)ElementNormal->GetDirectArray().GetAt(i)[0];
			vector.Y = (float)ElementNormal->GetDirectArray().GetAt(i)[1];
			vector.Z = (float)ElementNormal->GetDirectArray().GetAt(i)[2];

			printf("Normal (%f, %f, %f)\n",vector.X,vector.Y,vector.Z);

			_vertices[_faces[faceIndexPtr].vertexIndex[i%3]].normal+=vector;

			if((i+1)%3==0) ++faceIndexPtr;
			
		}

		for(auto ite=_vertices.begin();ite!=_vertices.end();++ite)
		{
			ace::Vector3DF normalized;
			ace::Vector3DF::Normal(normalized,ite->normal);
			ite->normal=normalized;
		}
	}
	else if(ElementNormal->GetMappingMode() == FbxLayerElement::eByPolygonVertex&&ElementNormal->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
	{
		ElementNormal->RemapIndexTo(FbxLayerElement::eByControlPoint);

		printf("PolyogonVertex Mapping\nIndexToDirect Reference\nNormal Count = %d\n",ElementNormal->GetDirectArray().GetCount());
		for ( int i = 0; i < ElementNormal->GetIndexArray().GetCount(); ++i ) {

			int index = ElementNormal->GetIndexArray().GetAt(i);

			ace::Vector3DF vector;
			vector.X = (float)ElementNormal->GetDirectArray().GetAt(index)[0];
			vector.Y = (float)ElementNormal->GetDirectArray().GetAt(index)[1];
			vector.Z = (float)ElementNormal->GetDirectArray().GetAt(index)[2];
			
			printf("Normal (%f, %f, %f)\n",vector.X,vector.Y,vector.Z);

			_vertices[index].normal=vector;
		}
	}
	else if(ElementNormal->GetMappingMode() == FbxLayerElement::eByControlPoint&&ElementNormal->GetReferenceMode() == FbxLayerElement::eDirect)
	{
		printf("ControlPoint Mapping\nDirect Reference\nNormal Count = %d\n",ElementNormal->GetDirectArray().GetCount());
		for ( int i = 0; i < ElementNormal->GetDirectArray().GetCount(); ++i ) {
			ace::Vector3DF vector;
			vector.X = (float)ElementNormal->GetDirectArray().GetAt(i)[0];
			vector.Y = (float)ElementNormal->GetDirectArray().GetAt(i)[1];
			vector.Z = (float)ElementNormal->GetDirectArray().GetAt(i)[2];
			
			printf("Normal (%f, %f, %f)\n",vector.X,vector.Y,vector.Z);

			_vertices[i].normal=vector;
		}
	}
	else if(ElementNormal->GetMappingMode() == FbxLayerElement::eByControlPoint&&ElementNormal->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
	{
		printf("ControlPoint Mapping\nIndexToDirect Reference\nNormal Count = %d\n",ElementNormal->GetDirectArray().GetCount());
		for ( int i = 0; i < ElementNormal->GetIndexArray().GetCount(); ++i ) {

			int index = ElementNormal->GetIndexArray().GetAt(i);

			ace::Vector3DF vector;
			vector.X = (float)ElementNormal->GetDirectArray().GetAt(index)[0];
			vector.Y = (float)ElementNormal->GetDirectArray().GetAt(index)[1];
			vector.Z = (float)ElementNormal->GetDirectArray().GetAt(index)[2];
			
			printf("Normal (%f, %f, %f)\n",vector.X,vector.Y,vector.Z);

			_vertices[index].normal=vector;
		}
	}
}

	
void MeshLoader::_loadBinormals(FbxMesh* fbxMesh)
{
	FbxLayerElementBinormal* ElementBinormal;
	int ptr=0;
	do
	{
		ElementBinormal = fbxMesh->GetLayer(ptr++)->GetBinormals();
	}while(ElementBinormal==0);

	if(ElementBinormal->GetMappingMode() == FbxLayerElement::eByPolygonVertex&&ElementBinormal->GetReferenceMode() == FbxLayerElement::eDirect)
	{
		printf("PolyogonVertex Mapping\nDirect Reference\nNormal Count = %d\n",ElementBinormal->GetDirectArray().GetCount());
		
		for (auto ite=_vertices.begin();ite!=_vertices.end();++ite)
		{
			ite->binormal=ace::Vector3DF(0,0,0);
		}

		int faceIndexPtr=0;

		for ( int i = 0; i < ElementBinormal->GetDirectArray().GetCount(); ++i ) {
			
			ace::Vector3DF vector;

			vector.X = (float)ElementBinormal->GetDirectArray().GetAt(i)[0];
			vector.Y = (float)ElementBinormal->GetDirectArray().GetAt(i)[1];
			vector.Z = (float)ElementBinormal->GetDirectArray().GetAt(i)[2];

			printf("Normal (%f, %f, %f)\n",vector.X,vector.Y,vector.Z);

			_vertices[_faces[faceIndexPtr].vertexIndex[i%3]].binormal+=vector;

			if((i+1)%3==0) ++faceIndexPtr;
			
		}

		for(auto ite=_vertices.begin();ite!=_vertices.end();++ite)
		{
			ace::Vector3DF normalized;
			ace::Vector3DF::Normal(normalized,ite->binormal);
			ite->binormal=normalized;
		}
	}
	else if(ElementBinormal->GetMappingMode() == FbxLayerElement::eByPolygonVertex&&ElementBinormal->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
	{
		ElementBinormal->RemapIndexTo(FbxLayerElement::eByControlPoint);

		printf("PolyogonVertex Mapping\nIndexToDirect Reference\nNormal Count = %d\n",ElementBinormal->GetDirectArray().GetCount());
		for ( int i = 0; i < ElementBinormal->GetIndexArray().GetCount(); ++i ) {

			int index = ElementBinormal->GetIndexArray().GetAt(i);

			ace::Vector3DF vector;
			vector.X = (float)ElementBinormal->GetDirectArray().GetAt(index)[0];
			vector.Y = (float)ElementBinormal->GetDirectArray().GetAt(index)[1];
			vector.Z = (float)ElementBinormal->GetDirectArray().GetAt(index)[2];
			
			printf("Normal (%f, %f, %f)\n",vector.X,vector.Y,vector.Z);

			_vertices[index].binormal=vector;
		}
	}
	else if(ElementBinormal->GetMappingMode() == FbxLayerElement::eByControlPoint&&ElementBinormal->GetReferenceMode() == FbxLayerElement::eDirect)
	{
		printf("ControlPoint Mapping\nDirect Reference\nNormal Count = %d\n",ElementBinormal->GetDirectArray().GetCount());
		for ( int i = 0; i < ElementBinormal->GetDirectArray().GetCount(); ++i ) {
			ace::Vector3DF vector;
			vector.X = (float)ElementBinormal->GetDirectArray().GetAt(i)[0];
			vector.Y = (float)ElementBinormal->GetDirectArray().GetAt(i)[1];
			vector.Z = (float)ElementBinormal->GetDirectArray().GetAt(i)[2];
			
			printf("Normal (%f, %f, %f)\n",vector.X,vector.Y,vector.Z);

			_vertices[i].binormal=vector;
		}
	}
	else if(ElementBinormal->GetMappingMode() == FbxLayerElement::eByControlPoint&&ElementBinormal->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
	{
		printf("ControlPoint Mapping\nIndexToDirect Reference\nNormal Count = %d\n",ElementBinormal->GetDirectArray().GetCount());
		for ( int i = 0; i < ElementBinormal->GetIndexArray().GetCount(); ++i ) {

			int index = ElementBinormal->GetIndexArray().GetAt(i);

			ace::Vector3DF vector;
			vector.X = (float)ElementBinormal->GetDirectArray().GetAt(index)[0];
			vector.Y = (float)ElementBinormal->GetDirectArray().GetAt(index)[1];
			vector.Z = (float)ElementBinormal->GetDirectArray().GetAt(index)[2];
			
			printf("Normal (%f, %f, %f)\n",vector.X,vector.Y,vector.Z);

			_vertices[index].binormal=vector;
		}
	}
}
	
void MeshLoader::_loadUVs(FbxMesh* fbxMesh)
{
	FbxStringList lUVSetNameList;
    fbxMesh->GetUVSetNames(lUVSetNameList);

    //iterating over all uv sets
    for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
    {
        //get lUVSetIndex-th uv set
        const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
        const FbxGeometryElementUV* lUVElement = fbxMesh->GetElementUV(lUVSetName);

        if(!lUVElement)
            continue;

        // only support mapping mode eByPolygonVertex and eByControlPoint
        if( lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
            lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint )
            return;

        //index array, where holds the index referenced to the uv data
        const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
        const int lIndexCount= (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

        //iterating through the data by polygon
        const int lPolyCount = fbxMesh->GetPolygonCount();

        if( lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint )
        {
            for( int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex )
            {
                // build the max index array that we need to pass into MakePoly
                const int lPolySize = fbxMesh->GetPolygonSize(lPolyIndex);
                for( int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex )
                {
                    FbxVector2 lUVValue;

                    //get the index of the current vertex in control points array
                    int lPolyVertIndex = fbxMesh->GetPolygonVertex(lPolyIndex,lVertIndex);

                    //the UV index depends on the reference mode
                    int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

                    lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

                    //User TODO:
                    //Print out the value of UV(lUVValue) or log it to a file

					_vertices[lUVIndex].uv.X=lUVValue[0];
					_vertices[lUVIndex].uv.Y=lUVValue[1];
                }
            }
        }
        else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
        {
            int lPolyIndexCounter = 0;
            for( int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex )
            {
                // build the max index array that we need to pass into MakePoly
                const int lPolySize = fbxMesh->GetPolygonSize(lPolyIndex);
                for( int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex )
                {
                    if (lPolyIndexCounter < lIndexCount)
                    {
                        FbxVector2 lUVValue;

                        //the UV index depends on the reference mode
                        int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

                        lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
					
						_vertices[lUVIndex].uv.X=lUVValue[0];
						_vertices[lUVIndex].uv.Y=lUVValue[1];

                        lPolyIndexCounter++;
                    }
                }
            }
        }
    }
}
	
void MeshLoader::_loadColors(FbxMesh* fbxMesh)
{
 
	//--- 頂点カラーセット数を取得 ---//
	int vColorLayerCount = fbxMesh->GetElementVertexColorCount();

	//--- レイヤー数だけ回る ---//
	for (int i = 0; vColorLayerCount > i; i++) {
   
		//--- 法線セットを取得 ---//
		FbxGeometryElementVertexColor* color = fbxMesh->GetElementVertexColor(i);
 
		//--- マッピングモードの取得
		FbxGeometryElement::EMappingMode mappingMode = color->GetMappingMode();
    
		//--- リファレンスモードの取得 ---//
		FbxGeometryElement::EReferenceMode referenceMode = color->GetReferenceMode();
 
		//--- マッピングモードの判別 ---//
		switch(mappingMode) {
		case FbxGeometryElement::eByControlPoint:
    
			break;
		case FbxGeometryElement::eByPolygon:
    
			break;
		case FbxGeometryElement::eByPolygonVertex:
			//--- リファレンスモードの判別 ---//
			switch(referenceMode) {
			case FbxGeometryElement::eIndexToDirect:
				{
					FbxLayerElementArrayTemplate<int>* index = &color->GetIndexArray();
					//--- 頂点の数だけ頂点カラーを取得 ---//
					for(int j = 0; j<index->GetCount(); j++) {
						_vertices[j].color[0] = (float)color->GetDirectArray().GetAt(index->GetAt(j))[0];
						_vertices[j].color[1] = (float)color->GetDirectArray().GetAt(index->GetAt(j))[1];
						_vertices[j].color[2] = (float)color->GetDirectArray().GetAt(index->GetAt(j))[2];
						_vertices[j].color[3] = (float)color->GetDirectArray().GetAt(index->GetAt(j))[3];
					}
				}
				break;
          
			case FbxGeometryElement::eDirect:

				break;
			case FbxGeometryElement::eIndex:
            
				break;
			default:
           
				break;
			}
			break;
		default:
        
			break;
		}
 
	}
}
	
void MeshLoader::_loadWeights(FbxMesh* fbxMesh)
{
	int skinCount=fbxMesh->GetDeformerCount(FbxDeformer::eSkin);

	for(int i=0;i<skinCount;++i)
	{
		FbxSkin* skin = (FbxSkin*)fbxMesh->GetDeformer(i,FbxDeformer::eSkin);

		int clusterNum = skin->GetClusterCount();

		for(int j=0;j<clusterNum;++j)
		{
			FbxCluster* cluster = skin->GetCluster(j);

			int pointNum = cluster->GetControlPointIndicesCount();
			int* pointAry = cluster->GetControlPointIndices();
			double* weightAry = cluster->GetControlPointWeights();

			for(int k=0;k<pointNum;++k)
			{
				int index = pointAry[i];
				float weight = (float)weightAry[i];

				_vertices[index].weight[j]=weight;
			}
		}
	}
}

void MeshLoader::_loadFaceIndices(FbxMesh* fbxMesh)
{
	int PolygonVertexNum=fbxMesh->GetPolygonVertexCount();
	int *IndexAry = fbxMesh->GetPolygonVertices();

	Face face;
	int faceIndexer=0;
	for(int i=0;i<PolygonVertexNum;++i)
	{
		face.vertexIndex[faceIndexer++]=IndexAry[i];
		if(faceIndexer==3)
		{
			faceIndexer=0;
			_faces.push_back(face);
		}
	}
}

void MeshLoader::Load(FbxMesh* fbxMesh)
{
	_loadPositions(fbxMesh);

	_loadFaceIndices(fbxMesh);

	_loadNormals(fbxMesh);
	//_loadBinormals(fbxMesh);
	//_loadUVs();
	//_loadColors();
	//_loadWeights();
}

void MeshLoader::WriteVertices(ace::BinaryWriter* writer)
{
	printf("Control Point Num = %d\n",_vertices.size());
	writer->Push((int32_t)_vertices.size());

	ace::Vector3DF zero3 = ace::Vector3DF(0,0,0);
	ace::Vector2DF zero2 = ace::Vector2DF(0,0);


	for(int i=0;i<_vertices.size();++i)
	{
		printf("CP(%f, %f, %f)\n",_vertices[i].position.X,_vertices[i].position.Y,_vertices[i].position.Z);
		writer->Push(_vertices[i].position);

		writer->Push(_vertices[i].normal);
		writer->Push(zero3);

		writer->Push(zero2);
		writer->Push(zero2);
		
		//頂点カラー
		writer->Push((uint8_t)255);
		writer->Push((uint8_t)255);
		writer->Push((uint8_t)255);
		writer->Push((uint8_t)255);

		//頂点ウェイト
		writer->Push((uint8_t)255);
		writer->Push((uint8_t)0);
		writer->Push((uint8_t)0);
		writer->Push((uint8_t)0);

		for (int j = 0; j < 4; ++j)
		{
			//writer->Push(_vertices[i].weightIndexOriginal[j]);
			writer->Push((uint8_t)0);
		}

		for (int j = 0; j < 4; ++j)
		{
			//writer->Push(_vertices[i].weightIndexDivided[j]);
			writer->Push((uint8_t)0);
		}
	}
}

void MeshLoader::WriteFaces(ace::BinaryWriter* writer)
{

	printf("Face Num = %d\n",_faces.size());
	writer->Push((int32_t) _faces.size());
	for (auto ite = _faces.begin(); ite != _faces.end(); ++ite)
	{
		printf("FI(");
		for (int i = 0; i < 3; ++i)
		{
			printf("%d, ",ite->vertexIndex[i]);
			writer->Push((int32_t) ite->vertexIndex[i]);
		}
		printf(")\n");
	}
}

void MeshLoader::_loadFaceMaterials(FbxMesh* fbxMesh)
{

}


void MeshLoader::_loadBoneAttachments(FbxMesh* fbxMesh)
{

}

void MeshLoader::WriteFaceMaterials(ace::BinaryWriter* writer)
{
	writer->Push((int32_t)0);
}
	
void MeshLoader::WriteBoneAttachments(ace::BinaryWriter* writer)
{
	writer->Push((int32_t)0);
}
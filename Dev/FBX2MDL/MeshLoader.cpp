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
	FbxGeometryElementNormal* lNormalElement = fbxMesh->GetElementNormal();
	if(!lNormalElement)
	{
		return;
	}
		
	if( lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint )	
	{
		for(int lVertexIndex = 0; lVertexIndex < fbxMesh->GetControlPointsCount(); lVertexIndex++)
		{
			int lNormalIndex = 0;
			if( lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect )
				lNormalIndex = lVertexIndex;

			if(lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
				lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

			FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);

			_vertices[lVertexIndex].normal=ace::Vector3DF(lNormal[0]/lNormal[3],lNormal[1]/lNormal[3],lNormal[2]/lNormal[3]);
		}
	}
	else if(lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		int lIndexByPolygonVertex = 0;

		int polygonCount=fbxMesh->GetPolygonCount();

		for(int lPolygonIndex = 0; lPolygonIndex < polygonCount; lPolygonIndex++)
		{
			int lPolygonSize = fbxMesh->GetPolygonSize(lPolygonIndex);

			for(int i = 0; i < lPolygonSize; i++)
			{
				int lNormalIndex = 0;
                    
				if( lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect )
					lNormalIndex = lIndexByPolygonVertex;
                    
				if(lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

				FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);

				//_vertices[lNormalIndex].normal=ace::Vector3DF(lNormal[0]/lNormal[3],lNormal[1]/lNormal[3],lNormal[2]/lNormal[3]);
				printf("[%d]=(%f, %f, %f)\n",i,lNormal[0],lNormal[1],lNormal[2]);

				lIndexByPolygonVertex++;
			}      
		}
			
	}
}

	
void MeshLoader::_loadBinormals(FbxMesh* fbxMesh)
{

	FbxGeometryElementBinormal* lBinormalElement = fbxMesh->GetElementBinormal();
    
	if(lBinormalElement)
	{
    
		if( lBinormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint )
		{
        
			for(int lVertexIndex = 0; lVertexIndex < fbxMesh->GetControlPointsCount(); lVertexIndex++)
			{
				int lBinormalIndex = 0;
                
				if( lBinormalElement->GetReferenceMode() == FbxGeometryElement::eDirect )
					lBinormalIndex = lVertexIndex;

				if(lBinormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					lBinormalIndex = lBinormalElement->GetIndexArray().GetAt(lVertexIndex);

				FbxVector4 lNormal = lBinormalElement->GetDirectArray().GetAt(lBinormalIndex);

				_vertices[lVertexIndex].binormal=ace::Vector3DF(lNormal[0]/lNormal[3],lNormal[1]/lNormal[3],lNormal[2]/lNormal[3]);
                
			}
            
		}
		else if(lBinormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
        
			int lIndexByPolygonVertex = 0;

			for(int lPolygonIndex = 0; lPolygonIndex < fbxMesh->GetPolygonCount(); lPolygonIndex++)
			{
				int lPolygonSize = fbxMesh->GetPolygonSize(lPolygonIndex);

				for(int i = 0; i < lPolygonSize; i++)
				{
					int lBinormalIndex = 0;

					if( lBinormalElement->GetReferenceMode() == FbxGeometryElement::eDirect )
						lBinormalIndex = lIndexByPolygonVertex;
                    
					if(lBinormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						lBinormalIndex = lBinormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

					FbxVector4 lNormal = lBinormalElement->GetDirectArray().GetAt(lBinormalIndex);

					_vertices[i].binormal=ace::Vector3DF(lNormal[0]/lNormal[3],lNormal[1]/lNormal[3],lNormal[2]/lNormal[3]);

					lIndexByPolygonVertex++;
                    
				}
			}
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
	//_loadNormals();
	//_loadBinormals();
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
		writer->Push(_vertices[i].position);

		writer->Push(zero3);
		writer->Push(zero3);

		writer->Push(zero2);
		writer->Push(zero2);

		//writer->Push(_vertices[i].normal);
		//writer->Push(_vertices[i].binormal);
		//writer->Push(_vertices[i].uv.X);
		//writer->Push(_vertices[i].uv.Y);
		
		for (int j = 0; j < 4; ++j)
		{
			//writer->Push(_vertices[i].color[j]);
			writer->Push((uint8_t)0);
		}

		for (int j = 0; j < 4; ++j)
		{
			//writer->Push(_vertices[i].weight[j]);
			writer->Push((int8_t)0);
		}

		for (int j = 0; j < 4; ++j)
		{
			//writer->Push(_vertices[i].weightIndexOriginal[j]);
			writer->Push((int8_t)0);
		}

		for (int j = 0; j < 4; ++j)
		{
			//writer->Push(_vertices[i].weightIndexDivided[j]);
			writer->Push((int8_t)0);
		}
	}
}

void MeshLoader::WriteFaces(ace::BinaryWriter* writer)
{
	writer->Push((int32_t) _faces.size());
	for (auto ite = _faces.begin(); ite != _faces.end(); ++ite)
	{
		for (int i = 0; i < 3; ++i)
		{
			writer->Push((int32_t) ite->vertexIndex[i]);
		}
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
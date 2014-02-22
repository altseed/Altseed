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
		vertex.normal=ace::Vector3DF(0,0,0);
		vertex.binormal=ace::Vector3DF(0,0,0);
		vertex.uv=ace::Vector2DF(0,0);
		vertex.subuv=ace::Vector2DF(0,0);
		for(int j=0;j<4;++j)
		{
			vertex.color[j]=0;
			vertex.weight[j]=0;
			vertex.weightIndexDivided[j]=0;
			vertex.weightIndexOriginal[j]=0;

		}

		_vertices.push_back(vertex);
	}
}

void MeshLoader::_loadNormals(FbxMesh* fbxMesh)
{
	FbxLayerElementNormal* ElementNormal = fbxMesh->GetLayer(0)->GetNormals();

	if(ElementNormal->GetMappingMode() == FbxLayerElement::eByPolygonVertex&&ElementNormal->GetReferenceMode() == FbxLayerElement::eDirect)
	{
		int faceIndexPtr=0;

		for ( int i = 0; i < ElementNormal->GetDirectArray().GetCount(); ++i ) {
			
			ace::Vector3DF vector;

			vector.X = (float)ElementNormal->GetDirectArray().GetAt(i)[0];
			vector.Y = (float)ElementNormal->GetDirectArray().GetAt(i)[1];
			vector.Z = (float)ElementNormal->GetDirectArray().GetAt(i)[2];

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
		for ( int i = 0; i < ElementNormal->GetIndexArray().GetCount(); ++i ) {

			int index = ElementNormal->GetIndexArray().GetAt(i);

			ace::Vector3DF vector;
			vector.X = (float)ElementNormal->GetDirectArray().GetAt(index)[0];
			vector.Y = (float)ElementNormal->GetDirectArray().GetAt(index)[1];
			vector.Z = (float)ElementNormal->GetDirectArray().GetAt(index)[2];

			_vertices[index].normal=vector;
		}
	}
	else if(ElementNormal->GetMappingMode() == FbxLayerElement::eByControlPoint&&ElementNormal->GetReferenceMode() == FbxLayerElement::eDirect)
	{
		for ( int i = 0; i < ElementNormal->GetDirectArray().GetCount(); ++i ) {
			ace::Vector3DF vector;
			vector.X = (float)ElementNormal->GetDirectArray().GetAt(i)[0];
			vector.Y = (float)ElementNormal->GetDirectArray().GetAt(i)[1];
			vector.Z = (float)ElementNormal->GetDirectArray().GetAt(i)[2];

			_vertices[i].normal=vector;
		}
	}
	else if(ElementNormal->GetMappingMode() == FbxLayerElement::eByControlPoint&&ElementNormal->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
	{
		for ( int i = 0; i < ElementNormal->GetIndexArray().GetCount(); ++i ) {

			int index = ElementNormal->GetIndexArray().GetAt(i);

			ace::Vector3DF vector;
			vector.X = (float)ElementNormal->GetDirectArray().GetAt(index)[0];
			vector.Y = (float)ElementNormal->GetDirectArray().GetAt(index)[1];
			vector.Z = (float)ElementNormal->GetDirectArray().GetAt(index)[2];

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

		int faceIndexPtr=0;

		for ( int i = 0; i < ElementBinormal->GetDirectArray().GetCount(); ++i ) {
			
			ace::Vector3DF vector;

			vector.X = (float)ElementBinormal->GetDirectArray().GetAt(i)[0];
			vector.Y = (float)ElementBinormal->GetDirectArray().GetAt(i)[1];
			vector.Z = (float)ElementBinormal->GetDirectArray().GetAt(i)[2];

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

		for ( int i = 0; i < ElementBinormal->GetIndexArray().GetCount(); ++i ) {

			int index = ElementBinormal->GetIndexArray().GetAt(i);

			ace::Vector3DF vector;
			vector.X = (float)ElementBinormal->GetDirectArray().GetAt(index)[0];
			vector.Y = (float)ElementBinormal->GetDirectArray().GetAt(index)[1];
			vector.Z = (float)ElementBinormal->GetDirectArray().GetAt(index)[2];

			_vertices[index].binormal=vector;
		}
	}
	else if(ElementBinormal->GetMappingMode() == FbxLayerElement::eByControlPoint&&ElementBinormal->GetReferenceMode() == FbxLayerElement::eDirect)
	{
		for ( int i = 0; i < ElementBinormal->GetDirectArray().GetCount(); ++i ) {
			ace::Vector3DF vector;
			vector.X = (float)ElementBinormal->GetDirectArray().GetAt(i)[0];
			vector.Y = (float)ElementBinormal->GetDirectArray().GetAt(i)[1];
			vector.Z = (float)ElementBinormal->GetDirectArray().GetAt(i)[2];

			_vertices[i].binormal=vector;
		}
	}
	else if(ElementBinormal->GetMappingMode() == FbxLayerElement::eByControlPoint&&ElementBinormal->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
	{
		for ( int i = 0; i < ElementBinormal->GetIndexArray().GetCount(); ++i ) {

			int index = ElementBinormal->GetIndexArray().GetAt(i);

			ace::Vector3DF vector;
			vector.X = (float)ElementBinormal->GetDirectArray().GetAt(index)[0];
			vector.Y = (float)ElementBinormal->GetDirectArray().GetAt(index)[1];
			vector.Z = (float)ElementBinormal->GetDirectArray().GetAt(index)[2];

			_vertices[index].binormal=vector;
		}
	}
}
	
void MeshLoader::_loadUVs(FbxMesh* fbxMesh)
{
	int i, j, lPolygonCount = fbxMesh->GetPolygonCount();
    FbxVector4* lControlPoints = fbxMesh->GetControlPoints(); 

	vector<int> addNum(fbxMesh->GetControlPointsCount(),0);

    int vertexId = 0;
    for (i = 0; i < lPolygonCount; i++)
    {
		int lPolygonSize = fbxMesh->GetPolygonSize(i);
		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = fbxMesh->GetPolygonVertex(i, j);
			for (int l = 0; l < fbxMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* leUV = fbxMesh->GetElementUV( l);

				switch (leUV->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						//Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
						_vertices[lControlPointIndex].uv.X=(float)leUV->GetDirectArray().GetAt(lControlPointIndex)[0];
						_vertices[lControlPointIndex].uv.Y=(float)leUV->GetDirectArray().GetAt(lControlPointIndex)[1];
						++addNum[lControlPointIndex];
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
							//Display2DVector(header, leUV->GetDirectArray().GetAt(id));
							_vertices[lControlPointIndex].uv.X=(float)leUV->GetDirectArray().GetAt(id)[0];
							_vertices[lControlPointIndex].uv.Y=(float)leUV->GetDirectArray().GetAt(id)[1];
							++addNum[lControlPointIndex];
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{
						int lTextureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
							{
								//Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
								_vertices[lControlPointIndex].uv.X=(float)leUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
								_vertices[lControlPointIndex].uv.Y=(float)leUV->GetDirectArray().GetAt(lTextureUVIndex)[1];
								++addNum[lControlPointIndex];
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;
				}
			}
			++vertexId;
		}
	}

	int vsize = (int)_vertices.size();
	for(i=0;i<vsize;++i)
	{
		if(addNum[i]==0) continue;
		_vertices[i].uv.X/=addNum[i];
		_vertices[i].uv.Y/=addNum[i];
	}
}
	
void MeshLoader::_loadColors(FbxMesh* fbxMesh)
{
 
	int i, j, lPolygonCount = fbxMesh->GetPolygonCount();
    FbxVector4* lControlPoints = fbxMesh->GetControlPoints(); 

	vector<int> addNum(fbxMesh->GetControlPointsCount(),0);

    int vertexId = 0;
    for (i = 0; i < lPolygonCount; i++)
    {
		int lPolygonSize = fbxMesh->GetPolygonSize(i);
		for (j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = fbxMesh->GetPolygonVertex(i, j);

			for (int l = 0; l < fbxMesh->GetElementVertexColorCount(); l++)
			{
				FbxGeometryElementVertexColor* leVtxc = fbxMesh->GetElementVertexColor( l);

				switch (leVtxc->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						//DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
						_vertices[lControlPointIndex].color[0]=(uint8_t)(leVtxc->GetDirectArray().GetAt(lControlPointIndex)[0]*255);
						_vertices[lControlPointIndex].color[1]=(uint8_t)(leVtxc->GetDirectArray().GetAt(lControlPointIndex)[1]*255);
						_vertices[lControlPointIndex].color[2]=(uint8_t)(leVtxc->GetDirectArray().GetAt(lControlPointIndex)[2]*255);
						_vertices[lControlPointIndex].color[3]=(uint8_t)(leVtxc->GetDirectArray().GetAt(lControlPointIndex)[3]*255);
						++addNum[lControlPointIndex];
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
							//DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
							_vertices[lControlPointIndex].color[0]=(uint8_t)(leVtxc->GetDirectArray().GetAt(id)[0]*255);
							_vertices[lControlPointIndex].color[1]=(uint8_t)(leVtxc->GetDirectArray().GetAt(id)[1]*255);
							_vertices[lControlPointIndex].color[2]=(uint8_t)(leVtxc->GetDirectArray().GetAt(id)[2]*255);
							_vertices[lControlPointIndex].color[3]=(uint8_t)(leVtxc->GetDirectArray().GetAt(id)[3]*255);
							++addNum[lControlPointIndex];
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{
						switch (leVtxc->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							//DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
							_vertices[lControlPointIndex].color[0]+=(uint8_t)(leVtxc->GetDirectArray().GetAt(vertexId)[0]*255);
							_vertices[lControlPointIndex].color[1]+=(uint8_t)(leVtxc->GetDirectArray().GetAt(vertexId)[1]*255);
							_vertices[lControlPointIndex].color[2]+=(uint8_t)(leVtxc->GetDirectArray().GetAt(vertexId)[2]*255);
							_vertices[lControlPointIndex].color[3]+=(uint8_t)(leVtxc->GetDirectArray().GetAt(vertexId)[3]*255);
							++addNum[lControlPointIndex];
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leVtxc->GetIndexArray().GetAt(vertexId);
								//DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
								_vertices[lControlPointIndex].color[0]+=(uint8_t)(leVtxc->GetDirectArray().GetAt(id)[0]*255);
								_vertices[lControlPointIndex].color[1]+=(uint8_t)(leVtxc->GetDirectArray().GetAt(id)[1]*255);
								_vertices[lControlPointIndex].color[2]+=(uint8_t)(leVtxc->GetDirectArray().GetAt(id)[2]*255);
								_vertices[lControlPointIndex].color[3]+=(uint8_t)(leVtxc->GetDirectArray().GetAt(id)[3]*255);
								++addNum[lControlPointIndex];
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;
				}
			}
			++vertexId;
		}
	}

	int vsize = (int)_vertices.size();
	for(i=0;i<vsize;++i)
	{
		if(addNum[i]==0) continue;
		_vertices[i].color[0]/=addNum[i];
		_vertices[i].color[1]/=addNum[i];
		_vertices[i].color[2]/=addNum[i];
		_vertices[i].color[3]/=addNum[i];
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
	_loadUVs(fbxMesh);
	_loadColors(fbxMesh);
	//_loadWeights();
	_loadTextures(fbxMesh);
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

		printf("NV(%f, %f, %f)\n",_vertices[i].normal.X,_vertices[i].normal.Y,_vertices[i].normal.Z);
		writer->Push(_vertices[i].normal);
		writer->Push(zero3);

		printf("UV(%f, %f)\n",_vertices[i].uv.X,_vertices[i].uv.Y);
		writer->Push(_vertices[i].uv);
		writer->Push(_vertices[i].subuv);

		//頂点カラー
		writer->Push((uint8_t)_vertices[i].color[0]);
		writer->Push((uint8_t)_vertices[i].color[1]);
		writer->Push((uint8_t)_vertices[i].color[2]);
		writer->Push((uint8_t)_vertices[i].color[3]);

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

void MeshLoader::WriteMaterials(ace::BinaryWriter* writer)
{
	printf("Material Num = %d\n",_materials.size());
	writer->Push((int32_t)_materials.size());
	for(auto ite=_materials.begin();ite!=_materials.end();++ite)
	{
		writer->Push(ite->Type);
		for(int i=0;i<3;++i)
		{
			printf("%s\n",ite->texture[i].c_str());
			writer->Push(ace::ToAString(ite->texture[i].c_str()));
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

void MeshLoader::_loadTextures(FbxMesh* fbxMesh)
{
	FbxNode* node = fbxMesh->GetNode();

	int materialCount = node->GetMaterialCount();

	const char* mats[3]={FbxSurfaceMaterial::sDiffuse,FbxSurfaceMaterial::sNormalMap,FbxSurfaceMaterial::sSpecular};

	for (int i = 0; materialCount > i; i++) {

		FbxSurfaceMaterial* material = node->GetMaterial(i);
		Material mat;
		mat.Type=0;
		for(int j=0;j<3;++j)
		{
			FbxProperty prop = material->FindProperty(mats[j]);

			int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

			if(0 < layeredTextureCount) {

				for(int j = 0; layeredTextureCount > j; j++) {

					FbxLayeredTexture* layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);
					int textureCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();

					for(int k = 0; textureCount > k; k++) {
						FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(k);

						if(texture) {
							//--- テクスチャ名を取得 ---//
							//std::string textureName = texture->GetName();
							std::string textureName = texture->GetRelativeFileName();

							//--- UVSet名を取得 ---//
							std::string UVSetName = texture->UVSet.Get().Buffer();

							mat.texture[j]=textureName;
						}
					}
				}
			}
			else {
				//--- テクスチャ数を取得 ---//
				int fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

				if(0 < fileTextureCount) {
					for(int j = 0; fileTextureCount > j; j++) {
						FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(j);
						if(texture) {
							//std::string textureName = texture->GetName();
							std::string textureName = texture->GetRelativeFileName();

							//--- UVSet名を取得 ---//
							std::string UVSetName = texture->UVSet.Get().Buffer();

							mat.texture[j]=textureName;
						}
					}
				}
			}
		}
		_materials.push_back(mat);
	}
}


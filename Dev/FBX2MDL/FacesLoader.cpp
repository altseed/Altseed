#include "FacesLoader.h"

void FacesLoader::_loadIndices()
{
	int PolygonVertexNum=_fbxMesh->GetPolygonVertexCount();
	int *IndexAry = _fbxMesh->GetPolygonVertices();

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

FacesLoader::FacesLoader(FbxMesh* fbxMesh)
{
	_fbxMesh=fbxMesh;
}

vector<Face> FacesLoader::GetFaces()
{
	return _faces;
}
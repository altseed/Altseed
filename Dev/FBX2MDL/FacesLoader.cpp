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

void FacesLoader::Load()
{
	_loadIndices();
}

void FacesLoader::Write(ace::BinaryWriter* writer)
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
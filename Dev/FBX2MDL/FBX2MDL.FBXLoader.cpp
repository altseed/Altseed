
#include "FBX2MDL.FBXLoader.h"
#include "FBX2MDL.Mesh.h"

namespace FBX2MDL
{
	ace::Vector3DF FBXLoader::LoadNormal(FbxLayerElementNormal* normals, int32_t vertexID, int32_t ctrlPointIndex)
	{
		ace::Vector3DF normal;

		if (normals->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			switch (normals->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				normal.X = normals->GetDirectArray().GetAt(vertexID)[0];
				normal.Y = normals->GetDirectArray().GetAt(vertexID)[1];
				normal.Z = normals->GetDirectArray().GetAt(vertexID)[2];
				break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int id = normals->GetIndexArray().GetAt(vertexID);
				normal.X = normals->GetDirectArray().GetAt(id)[0];
				normal.Y = normals->GetDirectArray().GetAt(id)[1];
				normal.Z = normals->GetDirectArray().GetAt(id)[2];
			}
				break;
			}
		}

		return normal;
	}

	ace::Vector2DF FBXLoader::LoadUV(FbxMesh* fbxMesh, FbxLayerElementUV* uvs, int32_t vertexID, int32_t ctrlPointIndex, int32_t polygonIndex, int32_t polygonPointIndex)
	{
		ace::Vector2DF uv;

		switch (uvs->GetMappingMode())
		{
			case FbxGeometryElement::eByControlPoint:
				switch (uvs->GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					{
						uv.X = uvs->GetDirectArray().GetAt(ctrlPointIndex)[0];
						uv.Y = uvs->GetDirectArray().GetAt(ctrlPointIndex)[1];
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						 auto id = uvs->GetIndexArray().GetAt(ctrlPointIndex);
						 uv.X = uvs->GetDirectArray().GetAt(id)[0];
						 uv.Y = uvs->GetDirectArray().GetAt(id)[1];
						 break;
					}	
				}
				break;

			case FbxGeometryElement::eByPolygonVertex:
			{
				auto textureUVIndex = fbxMesh->GetTextureUVIndex(polygonIndex, polygonPointIndex);
				switch (uvs->GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						uv.X = uvs->GetDirectArray().GetAt(textureUVIndex)[0];
						uv.Y = uvs->GetDirectArray().GetAt(textureUVIndex)[1];
						break;
					}
				}
				break;
			}
		default:
			break;
		}

		return uv;
	}

	ace::Color FBXLoader::LoadVertexColor(FbxMesh* fbxMesh, FbxLayerElementVertexColor* colors, int32_t vertexID, int32_t ctrlPointIndex, int32_t polygonIndex, int32_t polygonPointIndex)
	{
		ace::Color color;

		switch (colors->GetMappingMode())
		{
		default:
			break;

		case FbxGeometryElement::eByControlPoint:
			switch (colors->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				color.R = static_cast<uint8_t>(colors->GetDirectArray().GetAt(ctrlPointIndex)[0] * 255);
				color.G = static_cast<uint8_t>(colors->GetDirectArray().GetAt(ctrlPointIndex)[1] * 255);
				color.B = static_cast<uint8_t>(colors->GetDirectArray().GetAt(ctrlPointIndex)[2] * 255);
				color.A = static_cast<uint8_t>(colors->GetDirectArray().GetAt(ctrlPointIndex)[3] * 255);
				break;

			case FbxGeometryElement::eIndexToDirect:
			{
				auto id = colors->GetIndexArray().GetAt(ctrlPointIndex);
				color.R = static_cast<uint8_t>(colors->GetDirectArray().GetAt(id)[0] * 255);
				color.G = static_cast<uint8_t>(colors->GetDirectArray().GetAt(id)[1] * 255);
				color.B = static_cast<uint8_t>(colors->GetDirectArray().GetAt(id)[2] * 255);
				color.A = static_cast<uint8_t>(colors->GetDirectArray().GetAt(id)[3] * 255);
			}
				break;
			default:
				break;
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
			switch (colors->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				color.R = static_cast<uint8_t>(colors->GetDirectArray().GetAt(vertexID)[0] * 255);
				color.G = static_cast<uint8_t>(colors->GetDirectArray().GetAt(vertexID)[1] * 255);
				color.B = static_cast<uint8_t>(colors->GetDirectArray().GetAt(vertexID)[2] * 255);
				color.A = static_cast<uint8_t>(colors->GetDirectArray().GetAt(vertexID)[3] * 255);
				break;

			case FbxGeometryElement::eIndexToDirect:
				auto id = colors->GetIndexArray().GetAt(vertexID);
				color.R = static_cast<uint8_t>(colors->GetDirectArray().GetAt(id)[0] * 255);
				color.G = static_cast<uint8_t>(colors->GetDirectArray().GetAt(id)[1] * 255);
				color.B = static_cast<uint8_t>(colors->GetDirectArray().GetAt(id)[2] * 255);
				color.A = static_cast<uint8_t>(colors->GetDirectArray().GetAt(id)[3] * 255);
				break;
			}

			break;
		}
		
		return color;
	}

	void FBXLoader::LoadSkin(FbxMesh* fbxMesh, std::vector<BoneConnector>& boneConnectors, std::vector<FbxVertexWeight>& weights)
	{
		weights.resize(fbxMesh->GetControlPointsCount());

		auto skinCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);

		for (auto skinInd = 0; skinInd < skinCount; skinInd++)
		{
			auto skin = (FbxSkin*) fbxMesh->GetDeformer(skinInd, FbxDeformer::eSkin);
		
			auto clusterCount = skin->GetClusterCount();
			for (auto clusterInd = 0; clusterInd < clusterCount; clusterInd++)
			{
				auto cluster = skin->GetCluster(clusterInd);
				if (cluster->GetLink() == nullptr) continue;
				
				// ボーン取得
				auto name = cluster->GetLink()->GetName();

				FbxAMatrix m1, m2;
				cluster->GetTransformMatrix(m1);
				cluster->GetTransformLinkMatrix(m2);

				int32_t id = boneConnectors.size();

				BoneConnector connector;
				connector.Name = ace::ToAString(name);

				boneConnectors.push_back(connector);

				// 頂点ごとのウエイトを取得
				auto indexCount = cluster->GetControlPointIndicesCount();
				auto vindices = cluster->GetControlPointIndices();
				auto vweights = cluster->GetControlPointWeights();

				for (auto ind = 0; ind < indexCount; ind++)
				{
					FbxWeight data;
					data.Index = id;
					data.Weight = (float) vweights[ind];

					weights[vindices[ind]].Data.push_back(data);
				}
			}
		}

		// ウエイト計算
		for (auto& weight : weights)
		{
			if (weight.Data.size() == 0)
			{
				assert(weight.Indexes[0] == 255);
				assert(weight.Indexes[1] == 0);
			}
			else
			{
				weight.Data.push_back(FbxWeight());
				weight.Data.push_back(FbxWeight());
				weight.Data.push_back(FbxWeight());
				std::sort(weight.Data.begin(), weight.Data.end(), FbxWeight());

				float fSum = 0.0f;
				for (auto ind = 0; ind < 4; ind++)
				{
					fSum += weight.Data[ind].Weight;
				}

				if (fSum > 0)
				{
					for (auto ind = 0; ind < 4; ind++)
					{
						weight.Indexes[ind] = weight.Data[ind].Index;
						weight.Weights[ind] = (weight.Data[ind].Weight / fSum) * 255;
					}

					weight.Weights[0] = 255 - weight.Weights[1] - weight.Weights[2] - weight.Weights[3];
				}
				else
				{
					assert(weight.Indexes[0] == 255);
					assert(weight.Indexes[1] == 0);
				}
			}
		}
	}

	void FBXLoader::LoadMesh(Mesh* mesh, FbxMesh* fbxMesh)
	{
		assert(mesh != nullptr);
		assert(fbxMesh->GetLayerCount() > 0);

		auto node = fbxMesh->GetNode();
		auto layer = fbxMesh->GetLayer(0);
		auto uvs = layer->GetUVs();
		auto vcolors = layer->GetVertexColors();
		auto normals = layer->GetNormals();
		auto binormals = layer->GetBinormals();

		auto controlPoints = fbxMesh->GetControlPoints();
		auto controlPointsCount = fbxMesh->GetControlPointsCount();

		auto polygonCount = fbxMesh->GetPolygonCount();

		std::vector<FbxVertexWeight> weights;
		LoadSkin(fbxMesh, mesh->BoneConnectors, weights);

		mesh->Name = ace::ToAString(node->GetName());

		for (int32_t i = 0; i < controlPointsCount; i++)
		{
			Vertex v;
			v.Position.X = controlPoints[i][0];
			v.Position.Y = controlPoints[i][1];
			v.Position.Z = controlPoints[i][2];
			mesh->Vertexes.push_back(v);
		}

		for (int32_t polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++)
		{
			int polygonPointCount = fbxMesh->GetPolygonSize(polygonIndex);

			for (int32_t polygonPointIndex = 0; polygonPointIndex < polygonPointCount; polygonPointIndex++)
			{
				auto ctrlPointIndex = fbxMesh->GetPolygonVertex(polygonIndex, polygonPointIndex);
			
				if (uvs != nullptr)
				{

				}
			}
		}
	}
}

#include "FBX2MDL.FBXImporter.h"

namespace FBX2MDL
{
	void FBXImporter::CalcTangentSpace(const Vertex& v1, const Vertex& v2, const Vertex& v3, ace::Vector3DF& binormal, ace::Vector3DF& tangent)
	{
		binormal = ace::Vector3DF();
		tangent = ace::Vector3DF();

		ace::Vector3DF cp0[3];
		cp0[0] = ace::Vector3DF(v1.Position.X, v1.UV.X, v1.UV.Y);
		cp0[1] = ace::Vector3DF(v1.Position.Y, v1.UV.X, v1.UV.Y);
		cp0[2] = ace::Vector3DF(v1.Position.Z, v1.UV.X, v1.UV.Y);

		ace::Vector3DF cp1[3];
		cp1[0] = ace::Vector3DF(v2.Position.X, v2.UV.X, v2.UV.Y);
		cp1[1] = ace::Vector3DF(v2.Position.Y, v2.UV.X, v2.UV.Y);
		cp1[2] = ace::Vector3DF(v2.Position.Z, v2.UV.X, v2.UV.Y);

		ace::Vector3DF cp2[3];
		cp2[0] = ace::Vector3DF(v3.Position.X, v3.UV.X, v3.UV.Y);
		cp2[1] = ace::Vector3DF(v3.Position.Y, v3.UV.X, v3.UV.Y);
		cp2[2] = ace::Vector3DF(v3.Position.Z, v3.UV.X, v3.UV.Y);

		double u[3];
		double v[3];
		
		for (int32_t i = 0; i < 3; i++)
		{
			auto v1 = cp1[i] - cp0[i];
			auto v2 = cp2[i] - cp1[i];
			auto abc = ace::Vector3DF::Cross(v1, v2);

			if (abc.X == 0.0f)
			{
				return;
			}
			else
			{
				u[i] = -abc.Y / abc.X;
				v[i] = -abc.Z / abc.X;
			}
		}

		tangent = ace::Vector3DF(u[0], u[1], u[2]);
		tangent.Normalize();

		binormal = ace::Vector3DF(v[0], v[1], v[2]);
		binormal.Normalize();
	}

	ace::Vector3DF FBXImporter::LoadPosition(FbxMesh* fbxMesh, int32_t ctrlPointIndex)
	{
		ace::Vector3DF position;

		auto controlPoints = fbxMesh->GetControlPoints();
		
		position.X = controlPoints[ctrlPointIndex][0];
		position.Y = controlPoints[ctrlPointIndex][1];
		position.Z = controlPoints[ctrlPointIndex][2];
	
		return position;
	}

	ace::Vector3DF FBXImporter::LoadNormal(FbxLayerElementNormal* normals, int32_t vertexID, int32_t ctrlPointIndex)
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

	ace::Vector2DF FBXImporter::LoadUV(FbxMesh* fbxMesh, FbxLayerElementUV* uvs, int32_t vertexID, int32_t ctrlPointIndex, int32_t polygonIndex, int32_t polygonPointIndex)
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

		// 上下逆
		uv.Y = 1.0 - uv.Y;
		
		return uv;
	}

	ace::Color FBXImporter::LoadVertexColor(FbxMesh* fbxMesh, FbxLayerElementVertexColor* colors, int32_t vertexID, int32_t ctrlPointIndex, int32_t polygonIndex, int32_t polygonPointIndex)
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

	void FBXImporter::LoadSkin(FbxMesh* fbxMesh, std::vector<BoneConnector>& boneConnectors, std::vector<FbxVertexWeight>& weights)
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

				auto m1_ = ToAce(m1);
				auto m2_ = ToAce(m2);

				int32_t id = boneConnectors.size();

				BoneConnector connector;
				connector.Name = ace::ToAString(name);

				auto m2_inv = m2_.GetInverted();
				auto m = m2_inv * m1_;
				
				//  親ボーン行列 * 自分のボーン行列 * TransformLinkMatrix^-1 * TransformMatrix * 頂点
				connector.TransformMatrix = m1_;
				connector.TransformLinkMatrix = m2_;
				connector.OffsetMatrix = m;
				
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

	void FBXImporter::LoadMaterial(FbxMesh* fbxMesh, FbxLayerElementMaterial* materials, std::vector<Material>& dst)
	{
		auto node = fbxMesh->GetNode();
		if (node == nullptr) return;
	
		auto materialCount = node->GetMaterialCount();

		for (auto materialIndex = 0; materialIndex < materialCount; materialIndex++)
		{
			auto material = node->GetMaterial(materialIndex);

			auto name = material->GetName();

			auto m = Material();

			m.Name = ace::ToAString(name);

			// テクスチャ取得
			FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			auto fileTextureCount = prop.GetSrcObjectCount<FbxFileTexture>();

			for (auto fileTextureIndex = 0; fileTextureIndex < fileTextureCount; fileTextureIndex++)
			{
				auto texture = prop.GetSrcObject<FbxFileTexture>(fileTextureIndex);
				if (texture != nullptr)
				{
					m.DiffuseTexturePath = ace::ToAString(texture->GetRelativeFileName());
				}
			}

			dst.push_back(m);
		}

	}

	std::shared_ptr<Mesh> FBXImporter::LoadMesh(FbxMesh* fbxMesh)
	{
		assert(fbxMesh->GetLayerCount() > 0);

		auto node = fbxMesh->GetNode();
		auto layer = fbxMesh->GetLayer(0);
		auto uvs = layer->GetUVs();
		auto vcolors = layer->GetVertexColors();
		auto normals = layer->GetNormals();
		auto binormals = layer->GetBinormals();
		auto materials = layer->GetMaterials();

		auto controlPoints = fbxMesh->GetControlPoints();
		auto controlPointsCount = fbxMesh->GetControlPointsCount();

		auto polygonCount = fbxMesh->GetPolygonCount();

		std::vector<FbxVertexWeight> weights;
		std::vector<FbxFace> faces;
		std::vector<BoneConnector> boneConnectors;
		std::vector<Material> materials_;

		LoadSkin(fbxMesh, boneConnectors, weights);

		// ポリゴン走査
		int32_t vertexID = 0;
		for (int32_t polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++)
		{
			int polygonPointCount = fbxMesh->GetPolygonSize(polygonIndex);

			FbxFace face;

			for (int32_t polygonPointIndex = 0; polygonPointIndex < polygonPointCount; polygonPointIndex++)
			{
				auto ctrlPointIndex = fbxMesh->GetPolygonVertex(polygonIndex, polygonPointIndex);

				Vertex v;

				v.Position = LoadPosition(fbxMesh, ctrlPointIndex);
				
				for (auto i = 0; i < 4; i++)
				{
					v.Weight[i] = weights[ctrlPointIndex].Weights[i];
					v.WeightIndexOriginal[i] = weights[ctrlPointIndex].Indexes[i];
				}

				if (normals != nullptr)
				{
					v.Normal = LoadNormal(normals, vertexID, ctrlPointIndex);
				}

				if (uvs != nullptr)
				{
					v.UV = LoadUV(fbxMesh, uvs, vertexID, ctrlPointIndex, polygonIndex, polygonPointIndex);
				}

				if (vcolors != nullptr)
				{
					v.Color = LoadVertexColor(fbxMesh, vcolors, vertexID, ctrlPointIndex, polygonIndex, polygonPointIndex);
				}

				face.Vertecies.push_back(v);
				vertexID++;
			}

			faces.push_back(face);
		}

		// 面の表裏入れ替え
		for (auto& face : faces)
		{
			std::reverse(face.Vertecies.begin(), face.Vertecies.end());
		}

		// 材質インデックス設定
		if (materials != nullptr)
		{
			auto mappingMode = materials->GetMappingMode();
			auto referenceMode = materials->GetReferenceMode();

			if (mappingMode == FbxLayerElement::eAllSame)
			{
				auto indexArrayCount = materials->GetIndexArray().GetCount();
				if (indexArrayCount > 0)
				{
					auto index = materials->GetIndexArray().GetAt(0);
					for (auto& face : faces)
					{
						face.MaterialIndex = index;
					}
				}
			}
			else
			{
				if (materials->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					auto indexArrayCount = materials->GetIndexArray().GetCount();
					for (auto i = 0; i < indexArrayCount; i++)
					{
						auto index = materials->GetIndexArray().GetAt(i);
						faces[i].MaterialIndex = index;
					}
				}
			}
		}

		if (materials != nullptr)
		{
			LoadMaterial(fbxMesh, materials, materials_);
		}
		
		// メッシュで使用可能な形式に変換
	
		// 頂点変換テーブル作成
		int32_t vInd = 0;
		std::map<Vertex, int32_t> v2ind;
		std::map<int32_t, Vertex> ind2v;

		for (auto& face : faces)
		{
			if (face.Vertecies.size() != 3) continue;

			for (auto& vertex : face.Vertecies)
			{
				auto it = v2ind.find(vertex);
				if (it == v2ind.end())
				{
					v2ind[vertex] = vInd;
					ind2v[vInd] = vertex;
					vInd++;
				}
			}
		}

		// 設定
		auto mesh = std::make_shared<Mesh>();
		mesh->Name = ace::ToAString(node->GetName());
		mesh->BoneConnectors = boneConnectors;
		mesh->Materials = materials_;

		mesh->Vertexes.resize(vInd);
		for (auto& iv : ind2v)
		{
			mesh->Vertexes[iv.first] = iv.second;
		}

		for (auto& face : faces)
		{
			if (face.Vertecies.size() != 3) continue;

			Face f;
			f.Index[0] = v2ind[face.Vertecies[0]];
			f.Index[1] = v2ind[face.Vertecies[1]];
			f.Index[2] = v2ind[face.Vertecies[2]];

			f.MaterialIndex = face.MaterialIndex;

			mesh->Faces.push_back(f);
		}

		// Binormal,Tangent計算
		std::map<int32_t, VertexNormals> vInd2Normals;

		for (const auto& face : mesh->Faces)
		{
			ace::Vector3DF binormal, tangent;
			CalcTangentSpace(
				mesh->Vertexes[face.Index[0]],
				mesh->Vertexes[face.Index[1]],
				mesh->Vertexes[face.Index[2]],
				binormal,
				tangent);

			for (auto i = 0; i < 3; i++)
			{
				vInd2Normals[face.Index[i]].Binormal += binormal;
				vInd2Normals[face.Index[i]].Tangent += tangent;
				vInd2Normals[face.Index[i]].Count += 1;
			}
		}

		for (auto& vn : vInd2Normals)
		{
			vn.second.Binormal /= vn.second.Count;
			vn.second.Tangent /= vn.second.Count;
		}

		for (auto& vn : vInd2Normals)
		{
			mesh->Vertexes[vn.first].Binormal = vn.second.Binormal;
			//mesh->Vertexes[vn.first].Tangent = vn.second.Tangent;
		}

		return mesh;
	}

	std::shared_ptr<Node> FBXImporter::LoadHierarchy(std::shared_ptr<Node> parent, FbxNode* fbxNode, FbxManager* fbxManager)
	{
		FbxMesh* mesh = nullptr;
		std::shared_ptr<Node> node = std::make_shared<Node>();

		node->Name = ace::ToAString(fbxNode->GetName());

		auto attribute_ = fbxNode->GetNodeAttribute();

		if (attribute_ != nullptr)
		{
			auto attributeType = attribute_->GetAttributeType();
			switch (attributeType)
			{
			case FbxNodeAttribute::eMesh:
				mesh = fbxNode->GetMesh();

				if (!mesh->IsTriangleMesh())
				{
					FbxGeometryConverter converter(fbxManager);
					mesh = (FbxMesh*) converter.Triangulate(mesh, true);
				}

				node->MeshParameter = LoadMesh(mesh);
				break;
			case FbxNodeAttribute::eSkeleton:
				break;
			default:
				return std::shared_ptr<Node>();
				break;
			}
		}
		else
		{
			// ルート
		}

		// 回転方向
		fbxsdk_2015_1::EFbxRotationOrder fbxRotationOrder;
		fbxNode->GetRotationOrder(FbxNode::eDestinationPivot, fbxRotationOrder);

		switch (fbxRotationOrder)
		{
		case eEulerXYZ:
			node->RotationOrder = ace::ROTATION_ORDER_XYZ;
			break;
		case eEulerXZY:
			node->RotationOrder = ace::ROTATION_ORDER_XZY;
			break;
		case eEulerYZX:
			node->RotationOrder = ace::ROTATION_ORDER_YZX;
			break;
		case eEulerYXZ:
			node->RotationOrder = ace::ROTATION_ORDER_YXZ;
			break;
		case eEulerZXY:
			node->RotationOrder = ace::ROTATION_ORDER_ZXY;
			break;
		case eEulerZYX:
			node->RotationOrder = ace::ROTATION_ORDER_ZYX;
			break;
		case eSphericXYZ:
			break;
		}

		// デフォルト
		auto lclT = fbxNode->LclTranslation.Get();
		auto lclR = fbxNode->LclRotation.Get();
		auto lclS = fbxNode->LclScaling.Get();
		node->LclMatrix = CalcMatrix(node->RotationOrder, lclT[0], lclT[1], lclT[2], lclR[0], lclR[1], lclR[2], lclS[0], lclS[1], lclS[2]);

		// ジオメトリ
		auto geT = fbxNode->GeometricTranslation.Get();
		auto geR = fbxNode->GeometricRotation.Get();
		auto geS = fbxNode->GeometricScaling.Get();
		node->GeometricMatrix = CalcMatrix(node->RotationOrder, geT[0], geT[1], geT[2], geR[0], geR[1], geR[2], geS[0], geS[1], geS[2]);

		// 子の走査
		for (auto i = 0; i < fbxNode->GetChildCount(); i++)
		{
			auto childNode = LoadHierarchy(node, fbxNode->GetChild(i), fbxManager);
			if (childNode != nullptr)
			{
				node->Children.push_back(childNode);
			}
		}

		return node;
	}

	ace::Matrix44 FBXImporter::CalcMatrix(ace::eRotationOrder order, float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz)
	{
		ace::Matrix44 matT, matRx, matRy, matRz, matS;
		matT.SetTranslation(tx, ty, tz);
		matRx.SetRotationX(rx / 180.0 * 3.141592);
		matRy.SetRotationY(ry / 180.0 * 3.141592);
		matRz.SetRotationZ(rz / 180.0 * 3.141592);
		matS.SetScale(sx, sy, sz);

		if (order == ace::ROTATION_ORDER_XYZ) return matT * matRz * matRy * matRx * matS;
		if (order == ace::ROTATION_ORDER_XZY) return matT * matRy * matRz * matRx * matS;
		if (order == ace::ROTATION_ORDER_YZX) return matT * matRx * matRz * matRy * matS;

		if (order == ace::ROTATION_ORDER_YXZ) return matT * matRz * matRx * matRy * matS;
		if (order == ace::ROTATION_ORDER_ZXY) return matT * matRy * matRx * matRz * matS;
		if (order == ace::ROTATION_ORDER_ZYX) return matT * matRx * matRy * matRz * matS;

		return ace::Matrix44();
	}

	void FBXImporter::LoadAnimationSource(FbxAnimStack* fbxAnimStack, FbxNode* fbxRootNode, AnimationSource &animationSource)
	{
		const int32_t layerCount = fbxAnimStack->GetMemberCount<FbxAnimLayer>();
		if (layerCount == 0) return;

		auto animationName = fbxAnimStack->GetName();

		FbxTime startTime = fbxAnimStack->LocalStart;
		FbxTime endTime = fbxAnimStack->LocalStop;

		animationSource.Name = ace::ToAString(animationName);
		//animationSource.StartTime = startTime;
		//animationSource.StopTime = endTime;


		for (auto i = 0; i < layerCount; ++i)
		{
			auto layer = fbxAnimStack->GetMember<FbxAnimLayer>();
			LoadCurve(fbxRootNode, layer, animationSource);
			// break;
		}
	}

	void FBXImporter::LoadCurve(FbxNode* fbxNode, FbxAnimLayer* fbxAnimLayer, AnimationSource &animationSource)
	{
		auto boneName = ace::ToAString(fbxNode->GetName());

		auto transXCurve = fbxNode->LclTranslation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		auto transYCurve = fbxNode->LclTranslation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		auto transZCurve = fbxNode->LclTranslation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);

		auto rotXCurve = fbxNode->LclRotation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		auto rotYCurve = fbxNode->LclRotation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		auto rotZCurve = fbxNode->LclRotation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);

		auto sclXCurve = fbxNode->LclScaling.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		auto sclYCurve = fbxNode->LclScaling.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		auto sclZCurve = fbxNode->LclScaling.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);

		if (transXCurve != nullptr) LoadCurve(boneName + ace::ToAString(".pos.x"), transXCurve, animationSource);
		if (transYCurve != nullptr) LoadCurve(boneName + ace::ToAString(".pos.y"), transYCurve, animationSource);
		if (transZCurve != nullptr) LoadCurve(boneName + ace::ToAString(".pos.z"), transZCurve, animationSource);

		if (rotXCurve != nullptr) LoadCurve(boneName + ace::ToAString(".rot.x"), rotXCurve, animationSource);
		if (rotYCurve != nullptr) LoadCurve(boneName + ace::ToAString(".rot.y"), rotYCurve, animationSource);
		if (rotZCurve != nullptr) LoadCurve(boneName + ace::ToAString(".rot.z"), rotZCurve, animationSource);

		if (sclXCurve != nullptr) LoadCurve(boneName + ace::ToAString(".scl.x"), sclXCurve, animationSource);
		if (sclYCurve != nullptr) LoadCurve(boneName + ace::ToAString(".scl.y"), sclYCurve, animationSource);
		if (sclZCurve != nullptr) LoadCurve(boneName + ace::ToAString(".scl.z"), sclZCurve, animationSource);

		// 子の処理
		for (auto i = 0; i< fbxNode->GetChildCount(); i++)
		{
			LoadCurve(fbxNode->GetChild(i), fbxAnimLayer, animationSource);
		}
	}

	void FBXImporter::LoadCurve(ace::astring target, FbxAnimCurve* curve, AnimationSource &animationSource)
	{
		KeyFrameAnimation keyFrameAnimation;
		keyFrameAnimation.TargetName = target;

		const auto keyCount = curve->KeyGetCount();

		int hour, minute, second, frame, field, residual;

		for (auto i = 0; i < keyCount; i++)
		{
			float value = curve->KeyGetValue(i);
			auto time = curve->KeyGetTime(i);
			auto interpolation = curve->KeyGetInterpolation(i);
			time.GetTime(hour, minute, second, frame, field, residual, FbxTime::eFrames60);

			KeyFrame keyFrame;
			keyFrame.KeyValue = ace::Vector2DF(60 * (hour * 60 * 60 + minute * 60 + second) + frame, value);
			keyFrame.LeftPosition = keyFrame.KeyValue;
			keyFrame.RightPosition = keyFrame.KeyValue;

			switch (interpolation)
			{
			case fbxsdk_2015_1::FbxAnimCurveDef::eInterpolationConstant:
			{
				keyFrame.Interpolation = 1;
			}
				break;
			case fbxsdk_2015_1::FbxAnimCurveDef::eInterpolationLinear:
			{
				keyFrame.Interpolation = 2;
			}
				break;
			case fbxsdk_2015_1::FbxAnimCurveDef::eInterpolationCubic:
			{
				keyFrame.Interpolation = 3;
			}
				break;
			}

			keyFrameAnimation.KeyFrames.push_back(keyFrame);
		}
		animationSource.keyFrameAnimations.push_back(keyFrameAnimation);
	}

	std::shared_ptr<Scene> FBXImporter::LoadScene(FbxScene* fbxScene, FbxManager* fbxManager)
	{
		// 座標系変換
		FbxAxisSystem axis(FbxAxisSystem::eOpenGL);
		axis.ConvertScene(fbxScene);

		auto scene = std::make_shared<Scene>();

		// ノード
		auto root = fbxScene->GetRootNode();
		scene->Root = LoadHierarchy(nullptr, root, fbxManager);

		// アニメーション
		auto animStackCount = fbxScene->GetSrcObjectCount<FbxAnimStack>();

		for (auto animStackIndex = 0; animStackIndex < animStackCount; animStackIndex++)
		{
			AnimationSource source;

			auto animStack = fbxScene->GetSrcObject<FbxAnimStack>(animStackIndex);

			fbxScene->SetCurrentAnimationStack(animStack);
			LoadAnimationSource(animStack, root, source);

			scene->AnimationSources.push_back(source);
		}

		return scene;
	}
}
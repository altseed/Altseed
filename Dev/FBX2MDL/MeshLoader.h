#pragma once
#include <fbxsdk.h>
#include <vector>
#include "../ace_cpp/common/Utility/ace.BinaryWriter.h"
#include "../ace_cpp/common/Math/ace.Vector2DF.h"
#include "Mesh.h"
#include "Deformer.h"
#include "Material.h"
#define EPS 0.0001

class MeshLoader;

/*
メッシュグループ構造体
*/
struct MeshGroup
{
	std::vector<MeshLoader> meshLoaders;
	DeformerManager deformerManager;
	std::vector<Material> materials;

	//メッシュグループの材質一覧の情報を書き出す
	void WriteMaterials(ace::BinaryWriter* writer);
};

class MeshLoader
{
	//メッシュ名
	std::string m_name;

	//頂点リスト(UV並びに法線の値による分割前)
	std::vector<Vertex> m_baseVertices;

	//頂点リスト(UV並びに法線の値による分割後)
	std::vector<Vertex> m_vertices;

	//面リスト
	std::vector<Face> m_faces;

	//面材質リスト
	std::vector<FacialMaterial> m_facialMaterials;

	int m_faceContinue;
	int m_preFaceIndex;

	//頂点位置一覧読み込み関数
	void _loadPositions(FbxMesh* fbxMesh);

	//法線読み込み関数
	ace::Vector3DF _loadNormal(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId);

	//従法線読み込み関数
	ace::Vector3DF _loadBinormal(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId,bool &found);

	//UV読み込み関数
	ace::Vector2DF _loadUV(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId,int polygonCount,int polygonVert);

	//頂点カラー読み込み関数
	std::vector<uint8_t> _loadColor(FbxMesh* fbxMesh,int lControlPointIndex,int vertexId);

	//頂点ウェイトおよびインデックス読み込み関数
	void _loadWeight(FbxMesh* fbxMesh,int& attachedIndex,std::vector<MeshGroup> &meshGroups);

	//頂点情報と面情報の読み込み関数
	void _loadVerticesAndFaces(FbxMesh* fbxMesh);
	//void _loadFaceIndices(FbxMesh* fbxMesh);

	//テクスチャ（面材質）一覧読み込み関数
	void _loadTextures(FbxMesh* fbxMesh);
public:
	MeshLoader();
	
	std::vector<Material> materials;
	std::vector<Vertex> GetVertices();
	std::vector<Face> GetFaces();

	//メッシュの情報を読み込む関数
	void Load(FbxMesh* fbxMesh,int& attachmentIndex,std::vector<MeshGroup> &meshGroups);

	//頂点一覧書き出し関数
	void WriteVertices(ace::BinaryWriter* writer);

	//面一覧書き出し関数
	void WriteFaces(ace::BinaryWriter* writer);

	//面材質一覧書き出し関数
	void WriteFaceMaterials(ace::BinaryWriter* writer);

	//ボーン対応書き出し関数
	void WriteBoneAttachments(ace::BinaryWriter* writer);

	//材質一覧書き出し関数
	void WriteMaterials(ace::BinaryWriter* writer);

	MeshLoader& operator=( const MeshLoader &meshLoader)
	{
		m_baseVertices=meshLoader.m_baseVertices;
		m_vertices=meshLoader.m_vertices;
		m_faces=meshLoader.m_faces;
		m_facialMaterials=meshLoader.m_facialMaterials;

		return *this;
	}

};
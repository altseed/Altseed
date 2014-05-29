#pragma once

#include "FBX2MDL.Base.h"

#include "Deformer.h"
#include "MeshLoader.h"

#include "Animation.h"

//大元クラス
class MDLExporter
{
	//書き出しファイル名
	std::string m_outName;

	//SDKマネージャ
	FbxManager* m_SdkManager;

	//シーン
	FbxScene* m_Scene;

	//バイナリファイル書き出しクラス
	ace::BinaryWriter* m_binaryWriter;

	//メッシュグループ一覧
	std::vector<MeshGroup> m_meshGroups;

	//アニメーションソース一覧
	std::vector<AnimationSource> m_animationSources;

	//ファイルのヘッダを書き出す関数
	void PrintHeader();

	//メッシュを取得して、適切なメッシュグループに分類する
	void GetMeshGroup(FbxNode* pNode, int depth);

	//メッシュが持つ値を取得する関数
	void GetMeshProperty(FbxNode* node);

	//デフォーマを取得する関数(再帰的に全て調べあげる)
	void GetDeformer(Deformer* parentSkeleton, FbxNode* pNode, DeformerManager &deformerManager);

	//デフォーマが持つ値を取得する関数
	void GetDeformerProperty(Deformer* parentSkeleton, FbxNode* node, Deformer *deformer, DeformerManager &deformerManager);

	//モーションを取得する関数(再帰的に全て調べあげる)
	void GetMotion(FbxNode *parentNode, FbxNode* node, FbxAnimLayer* pLayer, AnimationSource &animationSource);

	//各々のアニメーションに関して調査を行う
	void AnimStackAnalyze(FbxAnimStack* pStack, FbxNode *rootNode, AnimationSource &animationSource);

	//ボーンの拡大、回転、移動の時間による値を取得する
	void GetSkeletonCurve(FbxNode* fbxNode, FbxAnimLayer* fbxAnimLayer, AnimationSource &animationSource);

	//指定した変換のアニメーションのカーブを解析する
	void AnalyzeCurve(std::string target, FbxAnimCurve* pCurve, AnimationSource &animationSource, float defaultValue);

public:
	MDLExporter(){}

	MDLExporter(const char* fileName, const char *exportName);

	//変換実行
	void Convert();

	~MDLExporter();
};
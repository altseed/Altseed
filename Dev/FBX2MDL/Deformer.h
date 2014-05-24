
#pragma once

#include "FBX2MDL.Base.h"

struct Deformer
{
	//ボーン名
	std::string name;

	//親ボーンインデックス
	int parentIndex;

	//回転順序
	int rotationOrder;

	//描画用逆行列
	ace::Matrix44 invMatrix;

	//親に対する変換行列
	ace::Matrix44 relationMatrix;

	//インデックス
	int index;

	//インデックス割り当て変数
	static int indexCount;
public:

	Deformer()
	{
		index = GetIndexCount();
	}

	//割り当てインデックス取得
	static int GetIndexCount()
	{
		return indexCount++;
	}

	//割り当てインデックスリセット
	static void ResetIndexCount()
	{
		indexCount=0;
	}
};

//メッシュグループが持つデフォーマ一覧を管理するクラス
class DeformerManager
{
	std::vector<Deformer*> m_deformer;

public:
	DeformerManager(){}
	~DeformerManager();

	//デフォーマ追加
	void AddDeformer(Deformer*);

	//デフォーマ名を元にデフォーマの格納インデックスを取得
	int GetIndexByName(std::string name);

	//デフォーマ名を元にデフォーマへの参照を取得
	Deformer* GetDeformerByName(std::string name);

	//インデックスを元にデフォーへの参照を取得
	Deformer* GetDeformerByIndex(int index);

	//デフォーマの情報を書き出す
	void WriteDeformerInformation(ace::BinaryWriter* binaryWriter);

	//デフォーマ数の取得
	int GetDeformerNum();
};
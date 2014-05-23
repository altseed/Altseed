#pragma once

#include "../ace_cpp/common/Math/ace.Vector2DF.h"
#include "../ace_cpp/common/Utility/ace.BinaryWriter.h"
#include "fbxsdk.h"
#include <vector>
#include <string>

//アニメーションクリップ構造体
struct AnimationClip
{
	//アニメーションクリップ名
	std::string animationName;

	//アニメーションソースのインデックス
	int animationSourceIndex;
};

//キーフレーム構造体
struct KeyFrame
{
	//時間と値
	ace::Vector2DF keyValue;

	//左手座標系
	ace::Vector2DF leftPosition;

	//右手座標系
	ace::Vector2DF rightPosition;

	//保管方法
	int interpolation;
};

struct KeyFrameAnimation
{
	//キーフレームアニメーション名
	std::string targetName;

	//キーフレーム一覧
	std::vector<KeyFrame> keyFrames;
};

//アニメーションソース構造体
struct AnimationSource
{
	//アニメーション名
	std::string animationName;

	//キーフレーム一覧
	std::vector<KeyFrameAnimation> keyFrameAnimations;

	//開始時間
	FbxTime startTime;

	//終了時間
	FbxTime stopTime;

	//アニメーションソース一覧書き出し関数
	void WriteAnimationSource(ace::BinaryWriter *binaryWriter);
};


#pragma once

#include "../ace_cpp/common/Math/ace.Vector2DF.h"
#include <vector>
#include <string>

struct AnimationClip
{
	std::string animationName;
	int animationSourceIndex;
};

struct KeyFrame
{
	ace::Vector2DF keyValue;
	ace::Vector2DF leftPosition;
	ace::Vector2DF rightPosition;
	int interpolation;
};

struct KeyFrameAnimation
{
	std::string targetName;
	std::vector<KeyFrame> keyFrames;
};

struct AnimationSource
{
	std::string animationName;
	std::vector<KeyFrameAnimation> keyFrameAnimations;

	FbxTime startTime;
	FbxTime stopTime;
};


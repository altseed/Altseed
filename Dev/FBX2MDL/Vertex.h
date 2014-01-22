#pragma once
#include "../ace_cpp/common/Math/ace.Vector3DF.h"
#include "../ace_cpp/common/Math/ace.Vector2DF.h"
#include "../ace_cpp/common/ace.common.Base.h"

struct Vertex
{
	ace::Vector3DF position;
	ace::Vector3DF normal;
	ace::Vector3DF binormal;
	ace::Vector2DF uv;

	float color[4];
	uint8_t weight[4];
	uint8_t weightIndexDivided[4];
	uint8_t weightIndexOriginal[4];
};
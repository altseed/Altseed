#pragma once

#include <string>
#include "../ace_cpp/common/Math/ace.Matrix44.h"
struct Deformer
{
	std::string name;
	int parentIndex;
	int rotationOrder;
	ace::Matrix44 invMatrix;
	ace::Matrix44 transformMatrix;
	int index;

	static int indexCount;
public:

	Deformer()
	{
		index = GetIndexCount();
	}

	static int GetIndexCount()
	{
		return indexCount++;
	}
};


class DeformerManager
{
	std::vector<Deformer*> _deformer;

public:
	DeformerManager(){}
	~DeformerManager();
	void AddDeformer(Deformer*);

	int GetIndexByName(std::string name);

	Deformer* GetDeformerByName(std::string name);

	Deformer* GetDeformerByIndex(int index);
};
#include "Deformer.h"

int Deformer::indexCount;

void DeformerManager::AddDeformer(Deformer* deformer)
{
	_deformer.push_back(deformer);
}

int DeformerManager::GetIndexByName(std::string name)
{
	for(int i=0;i<_deformer.size();++i)
	{
		if(_deformer[i]->name==name) return i;
	}
	return -1;
}

Deformer* DeformerManager::GetDeformerByName(std::string name)
{
	for(int i=0;i<_deformer.size();++i)
	{
		if(_deformer[i]->name==name) return _deformer[i];
	}

	return NULL;
}

Deformer* DeformerManager::GetDeformerByIndex(int index)
{
	return _deformer[index];
}

DeformerManager::~DeformerManager()
{

	
}

void DeformerManager::WriteDeformerInformation(ace::BinaryWriter* binaryWriter)
{
	binaryWriter->Push((int32_t)_deformer.size());

	for(int i=0;i<_deformer.size();++i)
	{
		binaryWriter->Push(ace::ToAString(_deformer[i]->name.c_str()));
		binaryWriter->Push((int32_t)_deformer[i]->parentIndex);
		binaryWriter->Push((int32_t)_deformer[i]->rotationOrder);
		binaryWriter->Push(_deformer[i]->transformMatrix);
		binaryWriter->Push(_deformer[i]->invMatrix);
	}
}

int DeformerManager::GetDeformerNum()
{
	return (int)_deformer.size();
}
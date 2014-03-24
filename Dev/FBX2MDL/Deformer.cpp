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
	
	for(int i=0;i<_deformer.size();++i)
	{
		delete _deformer[i];
	}

	_deformer.clear();
	
}
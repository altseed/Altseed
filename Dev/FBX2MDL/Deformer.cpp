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
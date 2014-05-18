#include "Deformer.h"

int Deformer::indexCount;

void DeformerManager::AddDeformer(Deformer* deformer)
{
	m_deformer.push_back(deformer);
}

int DeformerManager::GetIndexByName(std::string name)
{
	for(int i=0;i<m_deformer.size();++i)
	{
		if(m_deformer[i]->name==name) return i;
	}
	return -1;
}

Deformer* DeformerManager::GetDeformerByName(std::string name)
{
	for(int i=0;i<m_deformer.size();++i)
	{
		if(m_deformer[i]->name==name) return m_deformer[i];
	}

	return NULL;
}

Deformer* DeformerManager::GetDeformerByIndex(int index)
{
	return m_deformer[index];
}

DeformerManager::~DeformerManager()
{

}

void DeformerManager::WriteDeformerInformation(ace::BinaryWriter* binaryWriter)
{
	printf("Deformer Num:%d\n",m_deformer.size());

	binaryWriter->Push((int32_t)m_deformer.size());

	for(int i=0;i<m_deformer.size();++i)
	{
		binaryWriter->Push(ace::ToAString(m_deformer[i]->name.c_str()));
		binaryWriter->Push((int32_t)m_deformer[i]->parentIndex);
		binaryWriter->Push((int32_t)m_deformer[i]->rotationOrder);
		binaryWriter->Push(m_deformer[i]->relationMatrix);
		binaryWriter->Push(m_deformer[i]->invMatrix);
	}
}

int DeformerManager::GetDeformerNum()
{
	return (int)m_deformer.size();
}

#include "ace.Deformer_Imp.h"

namespace ace
{
	Deformer_Imp::Deformer_Imp()
	{
	
	}

	Deformer_Imp::~Deformer_Imp()
	{
	}

	void Deformer_Imp::AddBone(const achar* name, int32_t parentBoneIndex, eRotationOrder rotationOrder, const Matrix44& localMat)
	{
		auto bone = Model_IO::Bone();

		bone.Name = astring(name);
		bone.ParentBoneIndex = parentBoneIndex;
		bone.RotationType = rotationOrder;
		bone.LocalMat = localMat;

		m_bones.push_back(bone);

		if (m_nameToIndex.find(bone.Name) == m_nameToIndex.end())
		{
			m_nameToIndex[bone.Name] = m_bones.size() - 1;
		}	
	}

	int32_t Deformer_Imp::GetBoneIndex(const astring& name) const
	{
		if (m_nameToIndex.find(name) == m_nameToIndex.end())
		{
			return -1;
		}

		return m_nameToIndex.at(name);
	}
}
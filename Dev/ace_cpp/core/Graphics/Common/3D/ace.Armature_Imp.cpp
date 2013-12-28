
#include "ace.Armature_Imp.h"

namespace ace
{
	Armature_Imp::Armature_Imp()
	{
	
	}

	Armature_Imp::~Armature_Imp()
	{
	
	}

	void Armature_Imp::AddBone(const achar* name, int32_t parentBoneIndex, BoneRotationType rotationType, Matrix44 localMat, Matrix44 globalMatInv)
	{
		auto bone = Bone();

		bone.Name = astring(name);
		bone.ParentBoneIndex = parentBoneIndex;
		bone.RotationType = rotationType;
		bone.LocalMat = localMat;
		bone.GlobalMatInv = globalMatInv;

		m_bones.push_back(bone);

		if (m_nameToIndex.find(bone.Name) == m_nameToIndex.end())
		{
			m_nameToIndex[bone.Name] = m_bones.size() - 1;
		}	
	}

	int32_t Armature_Imp::GetBoneIndex(astring& name)
	{
		if (m_nameToIndex.find(name) == m_nameToIndex.end())
		{
			return -1;
		}

		return m_nameToIndex[name];
	}
}
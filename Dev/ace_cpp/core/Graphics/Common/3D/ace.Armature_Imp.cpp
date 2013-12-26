
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
	}
}

#include "asd.Deformer_Imp.h"

namespace asd
{
	Deformer_Imp::Deformer_Imp()
	{
	
	}

	Deformer_Imp::~Deformer_Imp()
	{
	}

	void Deformer_Imp::AddBone(const achar* name, int32_t parentBoneIndex, RotationOrder rotationOrder, const Matrix44& localMat, 
		const Vector3DF& translation, const Vector4DF& rotation, const Vector3DF& scaling)
	{
		auto bone = Model_IO::Bone();

		bone.Name = astring(name);
		bone.ParentBoneIndex = parentBoneIndex;
		bone.RotationType = rotationOrder;
		bone.LocalMat = localMat;

		bone.Translation[0] = translation.X;
		bone.Translation[1] = translation.Y;
		bone.Translation[2] = translation.Z;

		bone.Rotation[0] = rotation.X;
		bone.Rotation[1] = rotation.Y;
		bone.Rotation[2] = rotation.Z;
		bone.Rotation[3] = rotation.W;

		bone.Scaling[0] = scaling.X;
		bone.Scaling[1] = scaling.Y;
		bone.Scaling[2] = scaling.Z;

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
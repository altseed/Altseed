
#pragma once

#include "ace.Armature.h"
#include "../../../ace.Core.Base_Imp.h"

namespace ace
{
	class Armature_Imp
		: public Armature
		, public ReferenceObject
	{
	public:
		struct Bone
		{
			astring				Name;
			int32_t				ParentBoneIndex;
			BoneRotationType	RotationType;
			Matrix44			LocalMat;
			Matrix44			GlobalMatInv;
		};

		std::vector<Bone>	m_bones;

	public:
		Armature_Imp();
		virtual ~Armature_Imp();

		void AddBone(const achar* name, int32_t parentBoneIndex, BoneRotationType rotationType, Matrix44 localMat, Matrix44 globalMatInv) override;

		std::vector<Bone>& GetBones() { return m_bones; }

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}
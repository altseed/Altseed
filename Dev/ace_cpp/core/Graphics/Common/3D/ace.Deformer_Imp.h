
#pragma once

#include "ace.Deformer.h"
#include "../../../ace.Core.Base_Imp.h"

#include <unordered_map>

namespace ace
{
	class Deformer_Imp
		: public Deformer
		, public ReferenceObject
	{
	public:
		struct Bone
		{
			astring				Name;
			int32_t				ParentBoneIndex;
			eRotationOrder	RotationType;
			Matrix44			LocalMat;
			Matrix44			GlobalMatInv;
		};

		std::vector<Bone>	m_bones;
		std::unordered_map<astring, int32_t>	m_nameToIndex;

	public:
		Deformer_Imp();
		virtual ~Deformer_Imp();

		void AddBone(const achar* name, int32_t parentBoneIndex, eRotationOrder rotationType, const Matrix44& localMat, const Matrix44& globalMatInv) override;

		std::vector<Bone>& GetBones() { return m_bones; }

		int32_t GetBoneIndex(const astring& name) const;

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}
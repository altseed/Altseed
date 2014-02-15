
#pragma once

#include "ace.Deformer.h"
#include "../../../ace.Core.Base_Imp.h"

#include <unordered_map>

#include <Graphics/3D/ace.Model_IO.h>

namespace ace
{
	class Deformer_Imp
		: public Deformer
		, public ReferenceObject
	{
	public:
		std::vector<Model_IO::Bone>	m_bones;
		std::map<astring, int32_t>	m_nameToIndex;

	public:
		Deformer_Imp();
		virtual ~Deformer_Imp();

		void AddBone(const achar* name, int32_t parentBoneIndex, eRotationOrder rotationOrder, const Matrix44& localMat, const Matrix44& globalMatInv) override;

		std::vector<Model_IO::Bone>& GetBones() { return m_bones; }

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

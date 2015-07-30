
#pragma once

#include <unordered_map>
#include <Graphics/3D/asd.Model_IO.h>

#include "asd.Deformer.h"
#include "../../../asd.Core.Base_Imp.h"


namespace asd
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

		void AddBone(const achar* name, int32_t parentBoneIndex, RotationOrder rotationOrder, 
			const Matrix44& localMat, const Vector3DF& translation, const Vector4DF& rotation, const Vector3DF& scaling) override;

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

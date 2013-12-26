
#pragma once

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>
#include <Graphics/ace.Color.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.ReferenceObject.h"

namespace ace
{
	enum BoneRotationType
	{
		BONE_ROTATION_TYPE_QUATERNION = 10,
		BONE_ROTATION_TYPE_XZY = 11,
		BONE_ROTATION_TYPE_XYZ = 12,
		BONE_ROTATION_TYPE_ZXY = 13,
		BONE_ROTATION_TYPE_ZYX = 14,
		BONE_ROTATION_TYPE_YXZ = 15,
		BONE_ROTATION_TYPE_YZX = 16,
		BONE_ROTATION_TYPE_AXIS = 18,
		BONE_ROTATION_TYPE_MAX = 0xfffffff
	};

	class Armature
	{
	private:
		

	public:
		Armature() {}
		virtual ~Armature() {}

		virtual void AddBone(const achar* name, int32_t parentBoneIndex, BoneRotationType rotationType, Matrix44 localMat, Matrix44 globalMatInv) = 0;
	};
}
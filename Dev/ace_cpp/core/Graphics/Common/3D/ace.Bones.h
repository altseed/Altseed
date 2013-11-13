
#pragma once

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>
#include <Graphics/ace.Color.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

namespace ace
{
	class Bones
	{
	private:
		struct Bone
		{
			astring		Name;
			int32_t		ParentBoneIndex;
			Matrix44	LocalMat;
			Matrix44	GlobalMatInv;
		};

		std::vector<Bone>	m_bones;

	public:

	};
}
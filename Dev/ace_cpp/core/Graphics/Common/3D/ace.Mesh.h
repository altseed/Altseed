
#pragma once

#include <ace.common.Base.h>

#include "../../../ace.Core.Base.h"

#include "../../../ace.ReferenceObject.h"

namespace ace
{
	class Mesh
		: public IReference
	{
	protected:
		Mesh(){}
		virtual ~Mesh(){}
	public:

		virtual void AddVertex(
			const Vector3DF& position,
			const Vector3DF& normal,
			const Vector3DF& binormal,
			const Vector2DF& uv1,
			const Vector2DF& uv2,
			const Color& color,
			int32_t boneWeights,
			int32_t boneIndexes) = 0;

		virtual void AddFace(int32_t index1, int32_t index2, int32_t index3) = 0;

		virtual void AddMaterialOffset(int32_t materialIndex, int32_t faceOffset) = 0;
		virtual void SendToGPUMemory() = 0;
	};
};
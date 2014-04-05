
#pragma once

#include "ace.CoreObject3D.h"

namespace ace
{
	class CoreModelObject3D
		: public CoreObject3D
	{
	protected:
		CoreModelObject3D(){}
		virtual ~CoreModelObject3D(){}

	public:
		virtual void SetModel(Model* model) = 0;

		virtual void AddMeshGroup() = 0;

		virtual int32_t GetMeshGroupCount() = 0;

		virtual void AddMesh(int32_t meshGroupIndex, Mesh* mesh) = 0;

		virtual void SetDeformer(int32_t meshGroupIndex, Deformer* deformer) = 0;
	};
}

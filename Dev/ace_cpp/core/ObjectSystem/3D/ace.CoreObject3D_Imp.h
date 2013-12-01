
#pragma once

#include "ace.CoreObject3D.h"
#include "../../ace.ReferenceObject.h"

#include <Math/ace.Vector3DF.h>

namespace ace
{
	class CoreObject3D_Imp
		: public CoreObject3D
	{
	protected:
		RenderedObject3D*	m_commonObject;

	public:
		CoreObject3D_Imp();
		virtual ~CoreObject3D_Imp();

		virtual Vector3DF GetPosition() const;
		virtual void SetPosition(Vector3DF position);

		virtual void SetLayer(CoreLayer3D* layer);
	};
}

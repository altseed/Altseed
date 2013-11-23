
#pragma once

#include "ace.CoreObject3D.h"

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

		Vector3DF GetPosition() const;
		void SetPosition(Vector3DF position);

		virtual void SetLayer(CoreLayer3D* layer);
	};
}

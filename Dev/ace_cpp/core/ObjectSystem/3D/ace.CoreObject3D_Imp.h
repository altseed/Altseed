
#pragma once

#include "ace.CoreObject3D.h"
#include "../../ace.ReferenceObject.h"

#include <Math/ace.Vector3DF.h>

namespace ace
{
	class CoreObject3D_Imp
		: public CoreObject3D
	{
	private:
		bool m_isDrawn;

	protected:
		RenderedObject3D*	m_commonObject;
		CoreLayer3D*		m_owner;

	public:
		CoreObject3D_Imp();
		virtual ~CoreObject3D_Imp();

		virtual bool GetIsDrawn() const;
		virtual void SetIsDrawn(bool value);

		virtual Vector3DF GetPosition() const;
		virtual void SetPosition(Vector3DF position);

		virtual Vector3DF GetRotation() const;
		virtual void SetRotation(const Vector3DF& rot);

		virtual Vector3DF GetScale() const;
		virtual void SetScale(const Vector3DF& scale);

		virtual void SetLayer(CoreLayer3D* layer);
	};
}

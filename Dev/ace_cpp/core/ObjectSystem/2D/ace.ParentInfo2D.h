#pragma once
#include "ace.CoreObject2D.h"
#include "../ace.eChildMode.h"

namespace ace
{
	class ParentInfo2D
	{
	private:
		CoreObject2D* m_parent;
		eChildMode m_childMode;

	public:
		ParentInfo2D(CoreObject2D* parent, eChildMode mode)
			: m_parent(parent)
			, m_childMode(mode)
		{
			assert(parent != nullptr);
			SafeAddRef(parent);
		}

		~ParentInfo2D()
		{
			SafeRelease(m_parent);
		}

		CoreObject2D& GetParent() const
		{
			return *m_parent;
		}

		eChildMode GetChildMode() const
		{
			return m_childMode;
		}

		void SetChildMode(eChildMode mode)
		{
			m_childMode = mode;
		}
	};
}
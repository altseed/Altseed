#pragma once
#include "asd.CoreObject2D.h"
#include "../asd.ChildMode.h"

namespace asd
{
	class ParentInfo2D
	{
	private:
		CoreObject2D* m_parent;
		ChildMode m_childMode;

	public:
		ParentInfo2D(CoreObject2D* parent, ChildMode mode)
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

		ChildMode GetChildMode() const
		{
			return m_childMode;
		}

		void SetChildMode(ChildMode mode)
		{
			m_childMode = mode;
		}
	};
}
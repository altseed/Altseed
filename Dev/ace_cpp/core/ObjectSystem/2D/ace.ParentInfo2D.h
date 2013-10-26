#pragma once
#include "ace.ICoreObject2D.h"
#include "../ace.eChildMode.h"

namespace ace
{
	class ParentInfo2D
	{
	private:
		ICoreObject2D* m_parent;
		eChildMode m_childMode;

	public:
		ParentInfo2D(ICoreObject2D* parent, eChildMode mode)
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

		ICoreObject2D& GetParent() const
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
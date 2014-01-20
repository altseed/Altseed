#pragma once
#include "ace.CoreLayer2D.h"

namespace ace
{
	class ObjectInfo2D
	{
	private:
		CoreLayer2D* m_layer;
		bool m_isDrawn;
		bool m_isAlive;

	public:
		ObjectInfo2D();
		virtual ~ObjectInfo2D()
		{
		}

		bool GetIsDrawn() const;
		void SetIsDrawn(bool value);

		bool GetIsAlive() const;
		void SetIsAlive(bool value);

#if !SWIG
		void SetLayer(CoreLayer2D* layer);
		CoreLayer2D* GetLayer();
#endif
	};
}
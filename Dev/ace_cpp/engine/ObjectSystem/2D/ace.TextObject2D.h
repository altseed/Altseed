#pragma once

#include "ace.Object2D.h"

namespace ace
{
	class TextObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreTextObject2D> m_coreObject;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<TextObject2D> Ptr;

		TextObject2D();
		virtual ~TextObject2D();



	};
}
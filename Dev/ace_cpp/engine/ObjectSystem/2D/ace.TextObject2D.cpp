#include "ace.TextObject2D.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	TextObject2D::TextObject2D()
		: m_coreObject(nullptr)
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateTextObject2D());
	}

	TextObject2D::~TextObject2D()
	{
	}

	CoreObject2D* TextObject2D::GetCoreObject() const
	{
		return m_coreObject.get();
	}
}
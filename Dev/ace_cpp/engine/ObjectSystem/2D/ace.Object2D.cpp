#include "ace.Object2D.h"
using namespace std;

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	Object2D::Object2D()
		: m_coreObject(nullptr)
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateObject2D());
	}

	Object2D::~Object2D()
	{
	}

	ICoreObject2D* Object2D::GetCoreObject() const
	{
		return m_coreObject.get();
	}

	void Object2D::OnStart()
	{
	}

	void Object2D::OnUpdate()
	{
	}

	void Object2D::OnDrawingAdditionally()
	{
	}
}
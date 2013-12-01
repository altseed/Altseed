#include "ace.Layer.h"
using namespace std;

namespace ace
{
	Layer::Layer()
		: m_scene(nullptr)
		, m_isUpdated(true)
		, m_isDrawn(true)
	{
	}

	Layer::~Layer()
	{
		m_commonObject.reset();
	}

	Scene* Layer::GetScene() const
	{
		return m_scene;
	}

	void Layer::SetScene(Scene* value)
	{
		m_scene = value;
	}

	std::shared_ptr<CoreLayer> Layer::GetCoreLayer() const
	{ 
		return m_commonObject; 
	}

	bool Layer::GetIsUpdated() const
	{
		return m_isUpdated;
	}

	void Layer::SetIsUpdated(bool value)
	{
		m_isUpdated = value;
	}

	bool Layer::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void Layer::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
		m_commonObject->SetIsDrawn(value);
	}

	int Layer::GetDrawingPriority() const
	{
		return m_commonObject->GetDrawingPriority();
	}

	void Layer::SetDrawingPriority(int value)
	{
		m_commonObject->SetDrawingPriority(value);
	}
}
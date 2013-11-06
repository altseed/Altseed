#include "ace.Layer.h"
using namespace std;

namespace ace
{
	Layer::Layer()
		: m_scene(nullptr)
	{
	}

	Scene* Layer::GetScene() const
	{
		return m_scene;
	}

	void Layer::SetScene(Scene* value)
	{
		m_scene = value;
	}
}
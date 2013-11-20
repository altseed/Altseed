
#include "ace.CoreLayer3D_Imp.h"
#include "../PostEffect/ace.CorePostEffect_Imp.h"
#include "../../Window/ace.Window_Imp.h"
#include "../../Graphics/Common/ace.Graphics_Imp.h"

#include "ace.CoreObject3D.h"

namespace ace
{
	CoreLayer3D_Imp::CoreLayer3D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize)
		: CoreLayer_Imp(graphics, windowSize)
		, m_objects(std::list<ObjectPtr>())
		, m_renderer(nullptr)
	{
		m_renderer = new Renderer3D(graphics);
	}

	CoreLayer3D_Imp::~CoreLayer3D_Imp()
	{
		SafeDelete(m_renderer);

		for (auto& object : m_objects)
		{
			SafeRelease(object);
		}
	}

	void CoreLayer3D_Imp::AddObject(ObjectPtr object3D)
	{

	}

	void CoreLayer3D_Imp::RemoveObject(ObjectPtr object3D)
	{

	}

	void CoreLayer3D_Imp::Draw()
	{

	}
};

#include "ace.CoreLayer3D_Imp.h"
#include "../PostEffect/ace.CorePostEffect_Imp.h"
#include "../../Window/ace.Window_Imp.h"
#include "../../Graphics/Common/ace.Graphics_Imp.h"

#include "ace.CoreObject3D.h"
#include "ace.CoreObject3D_Imp.h"

namespace ace
{
	CoreLayer3D_Imp::CoreLayer3D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize)
		: CoreLayer_Imp(graphics, windowSize)
		, m_objects(std::set<ObjectPtr>())
		, m_renderer(nullptr)
	{
		m_renderer = new Renderer3D(graphics);
	}

	CoreLayer3D_Imp::~CoreLayer3D_Imp()
	{
		for (auto& object : m_objects)
		{
			object->SetLayer(nullptr);
			object->Release();
		}
		m_objects.clear();

		SafeDelete(m_renderer);
	}

	void CoreLayer3D_Imp::AddObject(ObjectPtr object3D)
	{
		if (m_objects.count(object3D) == 0)
		{
			m_objects.insert(object3D);
			object3D->SetLayer(this);
			SafeAddRef(object3D);
		}	
	}

	void CoreLayer3D_Imp::RemoveObject(ObjectPtr object3D)
	{
		if (m_objects.count(object3D) != 0)
		{
			m_objects.erase(object3D);
			object3D->SetLayer(nullptr);
			SafeRelease(object3D);
		}
	}

	void CoreLayer3D_Imp::BeginDrawing()
	{
		m_targetToLayer = -1;
	}

	void CoreLayer3D_Imp::Draw()
	{
		if (!m_isDrawn)
		{
			return;
		}

		m_renderer->Flip();
		m_renderer->Rendering();
	}

	void CoreLayer3D_Imp::EndDrawing()
	{

	}

	void CoreLayer3D_Imp::EndDrawingAfterEffects()
	{

	}

};
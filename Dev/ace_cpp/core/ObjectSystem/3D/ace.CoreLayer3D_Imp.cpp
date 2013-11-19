
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
		, m_layerRenderer(nullptr)
	{
		m_renderer = new Renderer3D(graphics);
		m_layerRenderer = new LayerRenderer(graphics);
		m_layerRenderer->SetWindowSize(windowSize);

		{
			ace::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = windowSize.X;
			lpos[1].Y = 0;
			lpos[2].X = windowSize.X;
			lpos[2].Y = windowSize.Y;
			lpos[3].X = 0;
			lpos[3].Y = windowSize.Y;
			m_layerRenderer->SetLayerPosition(lpos);
		}
	}

	CoreLayer3D_Imp::~CoreLayer3D_Imp()
	{
		SafeDelete(m_renderer);
		SafeRelease(m_layerRenderer);

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
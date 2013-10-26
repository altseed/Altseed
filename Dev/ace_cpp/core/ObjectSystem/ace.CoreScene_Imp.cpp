
#include "ace.CoreScene_Imp.h"
#include "../Utility/ace.IDObjectContainer.h"

using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreScene_Imp::CoreScene_Imp()
		: m_layers(list<LayerPtr>())
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreScene_Imp::~CoreScene_Imp()
	{
		for (auto& layer : m_layers)
		{
			SafeRelease(layer);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::AddLayer(LayerPtr layer)
	{
		m_layers.push_back(layer);
		SafeAddRef(layer);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::RemoveLayer(LayerPtr layer)
	{
		m_layers.remove(layer);
		SafeRelease(layer);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreScene_Imp::Draw()
	{
		m_layers.sort([](CoreLayer* x, CoreLayer* y)
		{
			return x->GetDrawingPriority() >= y->GetDrawingPriority();
		});

		for (auto& layer : m_layers)
		{
			layer->Draw();
		}
	}
}
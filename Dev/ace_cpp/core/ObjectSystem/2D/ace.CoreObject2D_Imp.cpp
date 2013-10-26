#include "../../Graphics/Common/2D/ace.Renderer2D_Imp.h"
#include "ace.CoreObject2D_Imp.h"

namespace ace
{
	CoreObject2D_Imp::CoreObject2D_Imp()
		: m_transform(TransformInfo2D())
		, m_layer(nullptr)
	{
	}

	void CoreObject2D_Imp::Draw()
	{
	}

	void CoreObject2D_Imp::SetLayer(CoreLayer2D* layer)
	{
		m_layer = layer;
	}

	void CoreObject2D_Imp::AddChild(ICoreObject2D& child, eChildMode mode)
	{
		child.SetParent(*this, mode);
	}

	void CoreObject2D_Imp::RemoveChild(ICoreObject2D& child)
	{
		child.ClearParent();
	}
}
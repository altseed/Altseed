
#pragma once

#include "ace.CoreLayer.h"

namespace ace
{
	class CoreLayer_Imp 
		: public CoreLayer
	{
	protected:

		Graphics_Imp*	m_graphics;

		bool			m_isDrawn;
		int32_t			m_drawingPriority;

		std::vector<CorePostEffect*>	m_postEffects;

		CoreLayer_Imp(Graphics* graphics);
		virtual ~CoreLayer_Imp();
	public:

		virtual int GetDrawingPriority() const;
		virtual void SetDrawingPriority(int value);

		virtual bool GetIsDrawn() const;
		virtual void SetIsDrawn(bool value);
	};
}
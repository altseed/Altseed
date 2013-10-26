
#pragma once

#include "ace.CoreFadeOut.h"
#include "../../ace.Core.Base_Imp.h"

namespace ace
{
	/**
	@brief	フェードアウトの内部クラス
	*/
	class FadeOut_Imp
		: public FadeOut
	{
	private:
		LayerRenderer*	m_renderer;

	public:
		FadeOut_Imp(LayerRenderer* renderer);
		virtual ~FadeOut_Imp();

		void DrawCurrentScreenToTriangle(
			Vector2DF pos1, Vector2DF uv1, Color col1,
			Vector2DF pos2, Vector2DF uv2, Color col2,
			Vector2DF pos3, Vector2DF uv3, Color col3);
	};
}
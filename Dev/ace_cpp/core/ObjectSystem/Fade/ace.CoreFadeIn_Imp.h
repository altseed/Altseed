
#pragma once

#include "ace.CoreFadeIn.h"

namespace ace
{
	/**
	@brief	フェードインの内部クラス
	*/
	class FadeIn_Imp
		: public FadeIn
	{
	public:
		FadeIn_Imp(){}
		virtual ~FadeIn_Imp(){}

		void DrawCurrentScreenToTriangle(
			Vector2DF pos1, Vector2DF uv1, Color col1,
			Vector2DF pos2, Vector2DF uv2, Color col2,
			Vector2DF pos3, Vector2DF uv3, Color col3);

		void DrawPreviousScreenToTriangle(
			Vector2DF pos1, Vector2DF uv1, Color col1,
			Vector2DF pos2, Vector2DF uv2, Color col2,
			Vector2DF pos3, Vector2DF uv3, Color col3);
	};
}
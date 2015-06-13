#pragma once

#include "asd.Profiler.h"
#include "asd.Profiler_Imp.h"
#include "../Window/asd.Window_Imp.h"

namespace asd
{
	/**
		@brief	プロファイリング結果を画面に表示するクラス。
	*/
	class ProfilerViewer
	{
	public:
		ProfilerViewer() {}
		virtual ~ProfilerViewer() {}

		/**
			@brief	プロファイリング結果を描画する。
		*/
		virtual void Draw() = 0;
	};
}
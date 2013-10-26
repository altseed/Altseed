#pragma once

#include "ace.Profiler.h"
#include "ace.Profiler_Imp.h"
#include "../Window/ace.Window_Imp.h"

namespace ace
{
	/**
		@brief	プロファイリング結果を画面に表示するクラス。
	*/
	class ProfilerViewer
	{
	public:
		ProfilerViewer() {}
		virtual ~ProfilerViewer() {}

		virtual void SetProfiler(Profiler_Imp* profiler) = 0;

		/**
			@brief	プロファイリング結果を描画する。
		*/
		virtual void Draw() = 0;
	};
}
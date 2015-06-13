#pragma once
#include <list>
#include "../asd.ReferenceObject.h"
#include "asd.LayerProfile.h"

namespace asd
{
	class LayerProfiler : public ReferenceObject
	{
	protected:
		LayerProfiler() {}
		virtual ~LayerProfiler() {}

	public:
		/**
		@brief	レイヤーのプロファイリングデータを記録する。
		@param	name		レイヤー名。
				objectCount	レイヤーに登録されているオブジェクト数。
				time		更新時間。
		*/
		virtual void Record(const achar* name, int objectCount, int time) = 0;

		/**
		@brief	プロファイリングデータを消去する。更新ごとに呼び出す必要がある。
		*/
		virtual void Refresh() = 0;
	};
}
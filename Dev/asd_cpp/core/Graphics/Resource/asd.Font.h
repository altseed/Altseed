#pragma once

#include <asd.common.Base.h>
#include "../../asd.ReferenceObject.h"

namespace asd
{
	/**
	@brief	フォントの情報が記録されているクラス
	*/
	class Font
		:public IReference
	{
	private:

	protected:
		Font(){}
		virtual ~Font(){}
	public:
		/**
		@brief	描画テキストと描画方向を与えると、その文字の描画領域を返す。
		@param	text	描画テキスト
		@param	writingDirection	描画方向
		@return	文字の描画領域
		*/
		virtual Vector2DI CalcTextureSize(const achar* text, WritingDirection writingDirection) = 0;
	};
}
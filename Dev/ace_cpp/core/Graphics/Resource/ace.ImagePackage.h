
#pragma once

#include <ace.common.Base.h>

#include <Math/ace.RectI.h>

#include "ace.Texture2D.h"
#include "../../ace.ReferenceObject.h"

namespace ace
{
	/**
	@brief	複数の画像が格納されているクラス
	*/
	class ImagePackage
		: public IReference
	{
	private:

	protected:
		ImagePackage() {}
		virtual ~ImagePackage(){}

		virtual Texture2D* GetImage_(int32_t index) = 0;

	public:
		
		/**
			@brief	格納されている画像の枚数を取得する。
			@return	画像の枚数
		*/
		virtual int32_t GetImageCount() = 0;

		/**
			@brief	格納されている画像の名称を取得する。
			@param	index	インデックス
			@return	名称
		*/
		virtual const achar* GetImageName(int32_t index) = 0;

		/**
		@brief	格納されている画像が配置される領域を取得する。
		@param	index	インデックス
		@return	領域
		*/
		virtual RectI GetImageArea(int32_t index) = 0;

		/**
			@brief	画像を取得する。
			@param	index	インデックス
			@return	画像
		*/
		std::shared_ptr<Texture2D> GetImage(int32_t index)
		{
			auto image = GetImage_(index);
			return CreateSharedPtrWithReleaseDLL(image);
		}
	};
}
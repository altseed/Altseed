#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "../../../ace.ReferenceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Texture2D
		: public IReference
	{
	protected:
		eTextureClassType	m_type;
		
	public:
		Texture2D(){}
		virtual ~Texture2D(){}

		/**
			@brief	テクスチャのサイズを取得する。
			@return	サイズ
		*/
		virtual Vector2DI GetSize() const = 0;

		/**
		@brief	フィルタを取得する。
		@return	フィルタ
		*/
		virtual eTextureFilterType GetFilter() const = 0;

		/**
		@brief	フィルタを設定する。
		@param	filter	フィルタ
		*/
		virtual void SetFilter(eTextureFilterType filter) = 0;

		/**
		@brief	テクスチャをファイルに保存する。
		@param	path	出力先
		@return	成否
		*/
		virtual bool Save(const achar* path) = 0;

		/**
			@brief	テクスチャのクラスの種類を取得する。
			@return	種類
		*/
		virtual eTextureClassType GetType() { return TEXTURE_CLASS_TEXTURE2D; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
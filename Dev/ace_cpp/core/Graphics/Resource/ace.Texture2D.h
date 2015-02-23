#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "ace.Texture.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	struct TextureLockInfomation
	{
		void* Pixels;
		int Pitch;
		Vector2DI Size;
	};

	class Texture2D
		: public Texture
	{
	protected:
		TextureClassType	m_type;
		
	public:
		Texture2D(){}
		virtual ~Texture2D(){}

		/**
			@brief	テクスチャのサイズを取得する。
			@return	サイズ
		*/
		virtual Vector2DI GetSize() const = 0;

		/**
			@brief	テクスチャのフォーマットを取得する。
			@return	フォーマット
		*/
		virtual TextureFormat GetFormat() const = 0;

		/**
		@brief	テクスチャをファイルに保存する。
		@param	path	出力先
		@return	成否
		*/
		virtual bool Save(const achar* path) = 0;

		/**
			@brief	テクスチャをロックし編集可能にする。
			@param	info	テクスチャ情報
			@return	成否
		*/
		virtual bool Lock(TextureLockInfomation& info) = 0;

		/**
		@brief	テクスチャをアンロックする。
		*/
		virtual void Unlock() = 0;

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual TextureClassType GetType() { return TextureClassType::Texture2D; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
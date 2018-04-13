#pragma once

#include "../asd.ReferenceObject.h"
#include "../asd.Core.Base.h"
#include "asd.File.h"

namespace asd
{
	/**
		@brief	一括してファイルを読み込むクラス
	*/
	class StaticFile 
		: public IReference
	{
	private:

	protected:
		StaticFile() {}
		virtual ~StaticFile() { };
	public:
		
		/**
			@brief	読み込まれたバッファを取得する。
			@return	バッファ
		*/
		virtual const std::vector<uint8_t>& GetBuffer() const = 0;

		/**
			@brief	ファイルのパスを取得する。
			@return	パス
			@note
			パッケージ外から読み込まれた場合は絶対パス、もしくは実行ファイルからの相対パスを返す。
			パッケージ内から読み込まれた場合は「パッケージへのパス?パッケージ内のファイルへのパス」を返す。
		*/
		virtual const char16_t* GetFullPath() const = 0;

		/**
			@brief	読み込まれたバッファの先頭のポインタを取得する。
			@return	ポインタ
		*/
		virtual void* GetData() = 0;

		/**
			@brief	読み込まれたバッファのサイズを取得する。
			@return	サイズ
		*/
		virtual int32_t GetSize() = 0;

		/**
			@brief	パッケージ内からファイルが読み込まれているかを取得する。
			@return	パッケージ内からファイルが読み込まれているか?
		*/
		virtual bool GetIsInPackage() const = 0;
	};
}
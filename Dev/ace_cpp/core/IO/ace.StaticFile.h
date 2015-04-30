#pragma once

#include "../ace.ReferenceObject.h"
#include "../ace.Core.Base.h"
#include "ace.File.h"

namespace ace
{
	/**
		@brief	一括してファイルを読み込むクラス
	*/
	class StaticFile 
		: public IReference
	{
	private:
	public:
		virtual ~StaticFile() { };

#ifndef SWIG
		/**
			@brief	読み込まれたバッファを取得する。
			@return	バッファ
		*/
		virtual const std::vector<uint8_t>& GetBuffer() const = 0;
#endif
		/**
			@brief	ファイルのパスを取得する。
			@return	パス
		*/
		virtual const achar* GetFullPath() const = 0;

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
	};
}
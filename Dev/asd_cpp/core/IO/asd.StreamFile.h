#pragma once

#include "../asd.ReferenceObject.h"
#include "../asd.Core.Base.h"

namespace asd
{
	/**
		@brief	部分的にファイルを読み込むクラス
	*/
	class StreamFile
		: public IReference
	{
		friend class Accessor;

	protected:
		virtual int32_t Read_(int32_t size) = 0;
		virtual void* GetTempBuffer_() = 0;
		virtual int32_t GetTempBufferSize_() = 0;

		StreamFile() {}
		virtual ~StreamFile() { };

	public:

		/**
		@brief	ファイルのサイズを取得する。
		@return	サイズ
		*/
		virtual int32_t GetSize() const = 0;

		/**
		@brief	ファイル内で現在読み込んでいる位置を取得する。
		@return	現在読み込んでいる位置
		*/
		virtual int32_t GetCurrentPosition() const = 0;

		/**
			@brief	ファイルからデータを読み込み、一時的なバッファに格納する。
			@param	size	読み込むサイズ
			@return	読み込んだサイズ
		*/
		virtual int32_t Read(int32_t size) = 0;

		/**
		@brief	読み込まれた一時的なバッファの先頭のポインタを取得する。
		@return	ポインタ
		*/
		virtual void* GetTempBuffer() = 0;

		/**
		@brief	読み込まれた一時的なバッファのサイズを取得する。
		@return	サイズ
		*/
		virtual int32_t GetTempBufferSize() = 0;

#if !SWIG
		/**
			@brief	指定したサイズ分、ファイルを読み込む。
			@param	buffer	出力先
			@param	size	読み込まれるサイズ
			@note
			#Ignore
		*/
		void Read(std::vector<uint8_t>& buffer, int32_t size)
		{
			auto result = Read_(size);
			buffer.resize(result);
			memcpy(buffer.data(), GetTempBuffer_(), result);
		}
#endif
	};
}
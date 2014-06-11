#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <Math/ace.Vector2DI.h>

#include "ace.Texture2D.h"
#include "../ace.DeviceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Texture2D_Imp
		: public Texture2D
		, public DeviceObject
	{
	protected:
		/**
			@brief	内部のバッファに画像ファイルから画像を展開する。
			@param	data	ファイルデータのポインタ
			@param	size	ファイルのデータサイズ
			@param	rev	上限反転で読み込む
		*/
		bool InternalLoad(void* data, int32_t size, bool rev);
		
		/**
			@brief	内部に展開された画像を開放する。
		*/
		void InternalUnload();

		uint8_t*	m_internalTextureData;
		int32_t		m_internalTextureWidth;
		int32_t		m_internalTextureHeight;

		Vector2DI	m_size;

		eTextureFilterType	m_filter;
		eTextureFormat		m_format;

		Texture2D_Imp(Graphics* graphics);
		virtual ~Texture2D_Imp();
	public:

		Vector2DI GetSize() const { return m_size; }
		eTextureFormat GetFormat() const override { return m_format; }

		eTextureFilterType GetFilter() const { return m_filter; }
		void SetFilter(eTextureFilterType filter) { m_filter = filter; }

	// リロード
#if !SWIG
	public:
		virtual void Reload(void* data, int32_t size) = 0;
#endif

	// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Window.h"
#include "../ace.Core.Base_Imp.h"

#include <Math/ace.Vector2DI.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
		@brief	ウインドウを管理するクラス
		@note	現在、Windowクラスのインスタンスが複数存在した時の挙動は保証していない。
	*/
	class Window_Imp
		: public Window
		, public ReferenceObject
	{
	protected:
		Vector2DI	m_size;

		Window_Imp(){}
		virtual ~Window_Imp(){}
	public:

		/**
			@brief	ウインドウを生成する。
			@param	width	[in]	クライアント領域の横幅
			@param	height	[in]	クライアント領域の縦幅
			@param	achar	[in]	タイトル
			@return	ウインドウ
		*/
		static Window_Imp* Create(int32_t width, int32_t height, const achar* title);

		Vector2DI GetSize() const { return m_size; }

#ifndef SWIG
		virtual GLFWwindow* GetWindow() = 0;
#endif

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
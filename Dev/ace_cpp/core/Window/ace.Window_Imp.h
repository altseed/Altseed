
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
		Log*		m_logger;

		Window_Imp(Log* logger);
		virtual ~Window_Imp();
	public:
		/**
			@brief	ウインドウを生成する。
			@param	width	クライアント領域の横幅
			@param	height	クライアント領域の縦幅
			@param	achar	タイトル
			@param	logger	ログ
			@return	ウインドウ
		*/
		static Window_Imp* Create(int32_t width, int32_t height, const achar* title, Log* logger = nullptr);

		Vector2DI GetSize() const { return m_size; }

#ifndef SWIG
		std::function<void()> OnFocused;

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
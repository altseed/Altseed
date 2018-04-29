
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Window.h"
#include "../asd.Core.Base_Imp.h"

#include <Math/asd.Vector2DI.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
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
		ap::Window*	window = nullptr;

		Vector2DI	m_size;
		Log*		m_logger;
		Cursor*		currentCursor = nullptr;

		Window_Imp(Log* logger);
		virtual ~Window_Imp();
	public:
		/**
			@brief	ウインドウを生成する。
			@param	width	クライアント領域の横幅
			@param	height	クライアント領域の縦幅
			@param	title	タイトルバーの文字列
			@param	logger	ログ
			@param	isFullScreen	フルスクリーンか
			@return	ウインドウ
		*/
		static Window_Imp* Create(
			int32_t width, 
			int32_t height, 
			const achar* title, 
			Log* logger, 
			bool isResizable,
			WindowPositionType windowPositionType,
			GraphicsDeviceType graphicsDeviceType,
			ColorSpaceType colorSpaceType, 
			bool isFullScreen);

		bool DoEvent() override;

		void SetTitle(const achar* title) override;

		virtual void Close() override;

		virtual void* GetWindowHandle() const override;

		Vector2DI GetSize() const override { return m_size; }

		void SetSize(Vector2DI size) override;

		void SetCursor(Cursor* cursor);

		const achar* GetClipboardString();

		void SetClipboardString(const achar* s);

#ifndef SWIG
		std::function<void()> OnFocused;

		std::function<void(int,int)> OnChangedSize;

		ap::Window* GetWindow();
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
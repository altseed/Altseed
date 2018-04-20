
#pragma once

#include "../asd.Core.Base.h"
#include "../asd.ReferenceObject.h"

namespace asd
{
	class Core_Imp;

	/**
		@brief	ツールを開発するための低レイヤーな命令群のクラス
	*/
	class Tool
		: public ReferenceObject
	{
		friend class Core_Imp;

	private:
		Window* window = nullptr;
		Graphics* graphics = nullptr;

	protected:
		Tool(Window* window, Graphics* graphics);
		virtual ~Tool();
	public:

		void Initialize();
		void Terminate();
		void NewFrame();
		void Render();

		// Imgui Windows

		/**
			@brief	ウインドウの表示を開始する。
			@param	name	ウインドウ名
		*/
		bool Begin(const char16_t* name);

		/**
			@brief	ウインドウの表示を終了する。
		*/
		void End();

		// Widgets: Text

		/**
		@brief	テキストを表示する。
		@param	text	テキスト名
		*/
		void Text(const char16_t* text);

		// Imgui Widgets: Main

		/**
			@brief	ボタンを表示する。
			@param	label	ラベル名
		*/
		bool Button(const char16_t* label);
	};
}
﻿
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

		/**
			@brief	画像を表示する。
			@param	user_texture	画像
			@param	size	大きさ
		*/
		void Image(Texture2D* user_texture, const Vector2DF& size);

		// Widgets: Combo Box

		/**
		@brief	コンボボックスを表示する。
		@param	label	ラベル名
		@param	preview_value	コンボボックスの先頭に表示される文字列
		*/
		bool BeginCombo(const char16_t* label, const char16_t* preview_value);

		/**
		@brief	コンボボックスを表示を終了する。
		@note
		BeginComboがtrueを返した場合のみ、実行する。
		*/
		void EndCombo();

		// Widgets: Input with Keyboard

		/**
			@brief	文字を入力する。
			@param	label	ラベル名
			@param	buf	文字が格納されるバッファ
			@param	buf_size	バッファサイズ
			@note
			bufはutf-8である。
		*/
		bool InputText(const char16_t* label, int8_t* buf, int32_t buf_size);

		/**
			@brief	整数を入力する。
			@param	label	ラベル名
			@param	v	値
		*/
		bool InputInt(const char16_t* label, int* v);

		// Widgets: Color Editor/Picker

		/**
			@brief	色を変更する。
			@param	label	ラベル名
			@param	vs		色(RGB+A)
		*/
		bool ColorEdit4(const char16_t* label, float* vs);

		// Widgets: Selectable / Lists

		/**
			@brief	リストボックスを表示する。
			@param	label	ラベル名
			@param	current_item	選択されているアイテム
			@param	items	アイテム(;区切り)
			@note
			仕様が大きく変更される可能性がある。
		*/
		bool ListBox(const char16_t* label, int* current_item, const char16_t* items);

		// Focus, Activation

		/**
			@brief	現在選択されているアイテムにフォーカスを設定する。
		*/
		void SetItemDefaultFocus();

		/**
			@brief	ファイルを開くダイアログを開く。
			@param	filterList	フィルタ
			@param	defaultPath	最初に表示するディレクトリのパス
			@return	開くパス
		*/
		const char16_t* OpenDialog(const char16_t* filterList, const char16_t* defaultPath);

		/**
		@brief	ファイルを保存するダイアログを開く。
		@param	filterList	フィルタ
		@param	defaultPath	最初に表示するディレクトリのパス
		@return	保存するパス
		*/
		const char16_t* SaveDialog(const char16_t* filterList, const char16_t* defaultPath);
	};
}
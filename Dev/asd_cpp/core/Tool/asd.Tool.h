
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
		Mouse* mouse = nullptr;

	protected:
		Tool(Window* window, Graphics* graphics, Mouse* mouse);
		virtual ~Tool();

		void Initialize();
		void Terminate();
		void NewFrame();
		void Render();
	public:

		// Ex
		/**
		@brief	フルスクリーンでウインドウの表示を開始する。
		@param	name	ウインドウ名
		@param	offset	ウインドウ上部の位置のオフセット
		*/
		bool BeginFullscreen(const char16_t* name, int32_t offset);

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

		// Cursor / Layout

		/**
			@brief	分割線を表示する。
		*/
		void Separator();

		/**
			@brief	次に表示する要素を改行せずに表示する。
		*/
		void SameLine();

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
			@brief	選択可能な文字列を表示する。
			@param	label	ラベル名
			@param	selected	選択されているか?
		*/
		bool Selectable(const char16_t* label, bool selected);

		/**
			@brief	リストボックスを表示する。
			@param	label	ラベル名
			@param	current_item	選択されているアイテム
			@param	items	アイテム(;区切り)
			@note
			仕様が大きく変更される可能性がある。
		*/
		bool ListBox(const char16_t* label, int* current_item, const char16_t* items);

		// MenuBar

		/**
		@brief	画面上のメニューバーの表示を開始する。
		*/
		bool BeginMainMenuBar();

		/**
		@brief	画面上のメニューバーの表示を終了する。
		@note
		BeginMainMenuBarがtrueを返した場合のみ、実行する。
		*/
		void EndMainMenuBar();

		/**
		@brief	メニューバー(ウインドウ等)の表示を開始する。
		*/
		bool BeginMenuBar();

		/**
		@brief	メニューバーの表示を終了する。
		@note
		BeginMenuBarがtrueを返した場合のみ、実行する。
		*/
		void EndMenuBar();

		/**
		@brief	メニューの表示を開始する。
		@param	label	ラベル名
		*/
		bool BeginMenu(const char16_t* label);

		/**
		@brief	メニューの表示を終了する。
		@note
		BeginMenuがtrueを返した場合のみ、実行する。
		*/
		void EndMenu();

		/**
		@brief	メニューアイテムを表示する。
		@param	label		ラベル名
		@param	shortcut	ショートカット
		@param	p_selected	選択されているか?
		*/
		bool MenuItem(const char16_t* label, const char16_t* shortcut, bool* p_selected);

		/**
		@brief	行数を設定する。
		@param	count	行数
		*/
		void Columns(int count);

		/**
		@brief	次の行に移動する。
		*/
		void NextColumn();
		
		/**
		@brief	現在の行のインデックスを取得する。
		*/
		int GetColumnIndex();
		
		/**
			@brief	行の幅を取得する。
			@param	column_index	インデックス
		*/
		float GetColumnWidth(int column_index);

		/**
			@brief	行の幅を設定する。
			@param	column_index	インデックス
			@param	width	幅
		*/
		void SetColumnWidth(int column_index, float width);

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

		/**
		@brief	フォルダを選択するダイアログを開く。
		@param	defaultPath	最初に表示するディレクトリのパス
		@return	フォルダのパス
		*/
		const char16_t* PickFolder(const char16_t* defaultPath);

		/**
		@brief	フォントを追加する。
		@param	filename	フォントへのパス
		@param	size_pixels	フォントサイズ
		@note
		現在、パッケージからは読み込めない。実行ファイルからの相対パスを指定する。
		*/
		void AddFontFromFileTTF(const char16_t* filename, float size_pixels);

		/**
			@brief	ダイアログを表示する。
			@param	message	メッセージ
			@param	title	タイトル
			@param	style	ダイアログの種類
			@param	buttons	ボタンの種類
		*/
		ToolDialogSelection ShowDialog(const char16_t* message, const char16_t* title, ToolDialogStyle style, ToolDialogButtons buttons);
	};
}

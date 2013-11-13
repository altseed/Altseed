#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "../ace.Core.Base.h"
#include "../ace.ReferenceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Core
		: public ReferenceObject
	{
	private:

	public:

		/**
		@brief	初期化を行う。
		@param	title	タイトル
		@param	width	横幅
		@param	height	縦幅
		@param	isFullScreen	フルスクリーンで起動するかどうか?
		@param	isOpenGLMode	OpenGLで起動するかどうか?
		@return	成否
		*/
		virtual bool Initialize(const achar* title, int32_t width, int32_t height, bool isFullScreen, bool isOpenGLMode) = 0;

		/**
		@brief	初期化を行う。
		@param	handle1	ハンドル
		@param	handle2	ハンドル
		@param	width	横幅
		@param	height	縦幅
		@return	成否
		*/
		virtual bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height) = 0;

		/**
		@brief	イベントを実行し、進行可否を判断する。
		@return	進行可能か?
		*/
		virtual bool DoEvents() = 0;

		/**
		@brief	更新処理を行う。
		*/
		virtual void Update() = 0;

		/**
		@brief	終了処理を行う。
		*/
		virtual void Terminate() = 0;

		/**
		@brief	更新されたリソースのリロードを行う。
		*/
		virtual void Reload() = 0;

		/**
			@brief	描画機能の準備をする。
		*/
		virtual void BeginDrawing() = 0;

		/**
			@brief	描画機能の使用を完了する。
		*/
		virtual void EndDrawing() = 0;

		/**
			@brief	現在のシーンを描画します。
		*/
		virtual void Draw() = 0;

		/**
			@brief	描画対象となるシーンを変更します。
		*/
		virtual void ChangeScene(CoreScene* scene) = 0;

		/**
			@brief	スクリーンショットをpngとして保存する。
			@param	path	出力先
		*/
		virtual void TakeScreenshot(const achar* path) = 0;

		/**
		@brief キーボードの入力を扱うKeyboardクラスを取得する。
		*/
		virtual Keyboard* GetKeyboard() = 0;

		/**
		@brief マウスの入力を扱うMouseクラスを取得する。
		*/
		virtual Mouse* GetMouse() = 0;

		/**
		@brief 接続されているジョイスティックの入力を扱うJoystickContainerクラスを取得する。
		*/
		virtual JoystickContainer* GetJoystickContainer() = 0;

		/**
			@brief	ログ出力を扱うLogクラスを取得する。
		*/
		virtual Log* GetLogger() = 0;

		/**
			@brief	プロファイリング機能を提供するProfilerクラスを取得する。
		*/
		virtual Profiler* GetProfiler() = 0;

		/**
			@brief	描画を扱うグラフィックスクラスを取得する。
		*/
		virtual Graphics* GetGraphics() = 0;


		virtual ObjectSystemFactory* GetObjectSystemFactory() = 0;
	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};

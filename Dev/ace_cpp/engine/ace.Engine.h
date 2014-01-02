
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.CoreToEngine.h"
#include "ObjectSystem/ace.Scene.h"
#include "ace.Engine.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
	@brief	初期化時に設定するオプションのクラス
	*/
	struct EngineOption
	{
		/**
		@brief	フルスクリーンで起動するか?
		*/
		bool			IsFullScreen = false;

		/**
		@brief	描画に使用するデバイス
		*/
		eGraphicsType	GraphicsType = eGraphicsType::GRAPHICS_TYPE_DX11;
	};


	class Engine
	{
		friend Engine* GetEngine();

	public:
		typedef std::shared_ptr<Scene> ScenePtr;

	private:
		Core*	m_core;
		ScenePtr m_currentScene;
		ScenePtr m_nextScene;
		
		static bool HasDLL(const char* path);
		static bool CheckDLL();

		Engine(Core* core);
		~Engine();

	public:
		/**
			@brief	初期化を行う。
			@param	title	タイトル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		bool Initialize(const achar* title, int32_t width, int32_t height, EngineOption option);

		/**
			@brief	初期化を行う。
			@param	handle1	ハンドル
			@param	handle2	ハンドル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, EngineOption option);

		/**
			@brief	イベントを実行し、進行可否を判断する。
			@return	進行可能か?
		*/
		bool DoEvents();

		/**
			@brief	更新処理を行う。
		*/
		void Update();

		/**
			@brief	終了処理を行う。
		*/
		void Terminate();

		/**
			@brief	描画する対象となるシーンを変更する。
		*/
		void ChangeScene(std::shared_ptr<Scene>& scene);

		/**
		@brief	スクリーンショットをpngとして保存する。
		@param	path	出力先
		*/
		void TakeScreenshot(const achar* path);

#if _WIN32

#else
		/**
			@brief	初期化を行う。
			@param	title	タイトル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		bool Initialize(const wchar_t* title, int32_t width, int32_t height, EngineOption option)
		{
			return Initialize( ToAString(title).c_str(), width, height, option );
		}
#endif
	};

	/**
		@brief	エンジンを取得する。
		@return	エンジン
	*/
	Engine* GetEngine();

	/**
		@brief キーボードクラスを取得する。
		@return キーボード
	*/
	Keyboard* GetKeyboard();

	/**
		@brief マウスクラスを取得する。
		@return マウス
	*/
	Mouse* GetMouse();

	/**
		@brief	ログクラスを取得する。
		@return	ログクラス
	*/
	Log* GetLogger();

	/**
	@brief	プロファイラクラスを取得する。
	@return	プロファイラクラス
	*/
	Profiler* GetProfiler();

	/**
	@brief	ジョイスティックコンテナクラスを取得する。
	@return	ジョイスティックコンテナクラス
	*/
	JoystickContainer* GetJoystickContainer();

	/**
		@brief	Graphicsクラスを取得する。
	*/
	Graphics* GetGraphics();

	/**
	@brief	AnimationSyatemクラスを取得する。
	preturn	AnimationSyatemクラス
	*/
	AnimationSystem* GetAnimationSyatem();

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

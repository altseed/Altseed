
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.CoreToEngine.h"
#include "ObjectSystem/ace.Scene.h"
#include "ObjectSystem/Transition/ace.Transition.h"
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
		GraphicsDeviceType	GraphicsDevice = GraphicsDeviceType::Default;

		/**
		@brief	マルチスレッドモードを使用するか?
		@note
		現在、マルチスレッドモードは開発中であり、実行速度は上昇するが、バグが多発する可能性がある。
		*/
		bool			IsMultithreadingMode = false;
	};


	class Engine
	{
	public:
		typedef std::shared_ptr<Scene> ScenePtr;

		static Core*					m_core;

		static Keyboard* m_keyboard ;
		static Mouse* m_mouse ;
		static JoystickContainer* m_joystickContainer ;
		static Log* m_logger ;
		static Profiler* m_profiler ;
		static Sound*	m_sound;
		static Graphics* m_graphics ;
		static ObjectSystemFactory* m_objectSystemFactory ;
		static AnimationSystem* m_animationSyatem ;


		static std::shared_ptr<Scene>	m_currentScene;
		static std::shared_ptr<Scene>	m_nextScene;
		static std::shared_ptr<Scene>	m_previousScene;

		static std::shared_ptr<Transition>	transition;

	private:
		static bool HasDLL(const char* path);
		static bool CheckDLL();
		static bool GenerateCore();

		Engine();
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
		static bool Initialize(const achar* title, int32_t width, int32_t height, EngineOption option);

		/**
			@brief	初期化を行う。
			@param	handle1	ハンドル
			@param	handle2	ハンドル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		static bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, EngineOption option);

		/**
			@brief	イベントを実行し、進行可否を判断する。
			@return	進行可能か?
		*/
		static bool DoEvents();

		/**
			@brief	更新処理を行う。
		*/
		static void Update();

		/**
			@brief	終了処理を行う。
		*/
		static void Terminate();

		/**
			@brief	描画する対象となるシーンを変更する。
			@param	scene	次のシーン
		*/
		static void ChangeScene(std::shared_ptr<Scene>& scene);

		/**
		@brief	描画する対象となるシーンを画面遷移効果ありで変更する。
		@param	scene	次のシーン
		@param	transition	画面遷移効果
		*/
		static void ChangeSceneWithTransition(std::shared_ptr<Scene>& scene, std::shared_ptr<Transition>& transition);

		/**
		@brief	スクリーンショットをpngとして保存する。
		@param	path	出力先
		*/
		static void TakeScreenshot(const achar* path);

		/**
		@brief	現在のFPSを取得する。
		@return FPS
		*/
		static float GetCurrentFPS();

		/**
		@brief	目標FPSを取得する。
		@return	FPS
		*/
		static int32_t GetTargetFPS();

		/**
		@brief	目標FPSを設定する。
		@param	fps	FPS
		*/
		static void SetTargetFPS(int32_t fps);

		/**
			@brief	ウインドウを閉じる。
		*/
		static void Close();

		/**
		@brief	Windowsの場合、ウインドウのハンドルを取得する。
		@return	ウインドウハンドル
		*/
		static void* GetWindowHandle();

		/**
		@brief キーボードクラスを取得する。
		@return キーボード
		*/
		static Keyboard* GetKeyboard();

		/**
		@brief マウスクラスを取得する。
		@return マウス
		*/
		static Mouse* GetMouse();

		/**
		@brief	ログクラスを取得する。
		@return	ログクラス
		*/
		static Log* GetLogger();

		/**
		@brief	プロファイラクラスを取得する。
		@return	プロファイラクラス
		*/
		static Profiler* GetProfiler();

		/**
		@brief	ジョイスティックコンテナクラスを取得する。
		@return	ジョイスティックコンテナクラス
		*/
		static JoystickContainer* GetJoystickContainer();

		/**
		@brief	Graphicsクラスを取得する。
		*/
		static Graphics* GetGraphics();

		/**
		@brief	Soundクラスを取得する。
		*/
		static Sound* GetSound();

		/**
		@brief	AnimationSyatemクラスを取得する。
		preturn	AnimationSyatemクラス
		*/
		static AnimationSystem* GetAnimationSyatem();

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
		static bool Initialize(const wchar_t* title, int32_t width, int32_t height, EngineOption option)
		{
			return Initialize( ToAString(title).c_str(), width, height, option );
		}
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

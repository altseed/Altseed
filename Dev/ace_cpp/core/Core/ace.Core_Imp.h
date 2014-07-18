#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Core.h"
#include "../ace.Core.Base_Imp.h"
#include "../ObjectSystem/ace.CoreScene.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Core_Imp
		: public Core
	{
#ifndef SWIG
		friend Core* CreateCore__();
#endif

	private:

		CoreFuncPtr		m_removedFuncPtr;
		bool	m_isInitializedByExternal;

		Window_Imp*		m_window;
		Keyboard_Imp*	m_keyboard;
		Mouse_Imp*		m_mouse;
		Log_Imp*		m_logger;
		Profiler_Imp*	m_profiler;
		ProfilerViewer_Imp* m_profilerViewer;
		Graphics_Imp*	m_graphics;
		Sound_Imp*		m_sound;

		ObjectSystemFactory_Imp* m_objectSystemFactory;
		AnimationSystem_Imp*	m_animationSyatem;

		JoystickContainer_Imp* m_joystickContainer;

		File*			m_file;

		CoreScene* m_currentScene;

		std::vector<astring>		m_screenShots;

		int32_t				m_targetFPS;
		float				m_currentFPS;
		float				m_nextFPS;
		int64_t				m_previousTime;
		int64_t				m_startFrameTime;

		Core_Imp();
		virtual ~Core_Imp();

		void ComputeFPS();

		void ControlFPS();

	public:
		static Core_Imp* CreateCore();

#if !SWIG
		void SetRemovedFunctionPpointer(CoreFuncPtr func);
#endif

		bool Initialize(const achar* title, int32_t width, int32_t height, bool isFullScreen, bool isOpenGLMode, bool isMultithreadingMode);

		bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, bool isOpenGLMode, bool isMultithreadingMode);

		bool DoEvents();

		void Update();

		void Terminate();

		void Reload();

		void BeginDrawing();

		void EndDrawing();

		void Draw();

		void ChangeScene(CoreScene* scene);

		void Close() override;

		void TakeScreenshot(const achar* path);

		float GetCurrentFPS();

		int32_t GetTargetFPS();

		void SetTargetFPS(int32_t fps);

		void* GetWindowHandle() const;

		Window_Imp* GetWindow() { return m_window; }

		Keyboard* GetKeyboard();

		Mouse* GetMouse();

		JoystickContainer* GetJoystickContainer();

		File* GetFile();

		Log* GetLogger();

		Profiler* GetProfiler();

		Graphics* GetGraphics();

		Graphics_Imp* GetGraphics_Imp();

		Sound* GetSound();

		ObjectSystemFactory* GetObjectSystemFactory();

		AnimationSystem* GetAnimationSyatem();
	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};

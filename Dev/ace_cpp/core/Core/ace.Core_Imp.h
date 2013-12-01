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

		bool	m_isInitializedByExternal;

		Window_Imp*		m_window;
		Keyboard_Imp*	m_keyboard;
		Mouse_Imp*		m_mouse;
		Log_Imp*		m_logger;
		Profiler_Imp*	m_profiler;
		ProfilerViewer_Imp* m_profilerViewer;
		Graphics_Imp*	m_graphics;
		ObjectSystemFactory_Imp* m_objectSystemFactory;

		JoystickContainer_Imp* m_joystickContainer;

		CoreScene* m_currentScene;

		std::vector<astring>		m_screenShots;

		Core_Imp();
		virtual ~Core_Imp();
	public:
		static Core_Imp* CreateCore();

		bool Initialize(const achar* title, int32_t width, int32_t height, bool isFullScreen, bool isOpenGLMode);

		bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height);

		bool DoEvents();

		void Update();

		void Terminate();

		void Reload();

		void BeginDrawing();

		void EndDrawing();

		void Draw();

		void ChangeScene(CoreScene* scene);

		void TakeScreenshot(const achar* path);

		Window_Imp* GetWindow() { return m_window; }

		Keyboard* GetKeyboard();

		Mouse* GetMouse();

		JoystickContainer* GetJoystickContainer();

		Log* GetLogger();

		Profiler* GetProfiler();

		Graphics* GetGraphics();

		Graphics_Imp* GetGraphics_Imp();


		ObjectSystemFactory* GetObjectSystemFactory();
	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};

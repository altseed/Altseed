
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Core_Imp.h"
#include "../Window/ace.Window_Imp.h"
#include "../Input/ace.Keyboard_Imp.h"
#include "../Input/ace.Mouse_Imp.h"
#include "../Input/ace.JoystickContainer_Imp.h"
#include "../IO/ace.File_Imp.h"
#include "../Log/ace.Log_Imp.h"
#include "../Profiler/ace.Profiler_Imp.h"
#include "../Profiler/ace.ProfilerViewer_Imp.h"
#include "../ObjectSystem/ace.ObjectSystemFactory_Imp.h"

#include "../Graphics/ace.Graphics_Imp.h"
#include "../Sound/ace.Sound_Imp.h"
#include "../Graphics/Resource/ace.RenderState_Imp.h"

#include "../Graphics/Animation/ace.AnimationSystem_Imp.h"

#include <Utility/ace.Timer.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Core_Imp::Core_Imp()
		: m_window(nullptr)
		, m_keyboard(nullptr)
		, m_mouse(nullptr)
		, m_graphics(nullptr)
		, m_sound(nullptr)
		, m_joystickContainer(nullptr)
		, m_file(nullptr)
		, m_logger(nullptr)
		, m_profiler(nullptr)
		, m_profilerViewer(nullptr)
		, m_currentScene(nullptr)
		, m_removedFuncPtr(nullptr)
		, m_isInitializedByExternal(false)
		, m_objectSystemFactory(nullptr)
		, m_animationSyatem(nullptr)
		, m_targetFPS(60)
		, m_currentFPS(60)
		, m_nextFPS(60)
		, m_previousTime(0)
		, m_startFrameTime(0)
	{
		m_previousTime = GetTime();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Core_Imp::~Core_Imp()
	{
		Terminate();

		auto removingPtr = m_removedFuncPtr;
		if (removingPtr != nullptr)
		{
			removingPtr(this);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Core_Imp* Core_Imp::CreateCore()
	{
		return new Core_Imp();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::ComputeFPS()
	{
		auto time = GetTime();
		auto div = (time - m_previousTime) / 1000;
		if (div > 1000)
		{
			m_currentFPS = m_nextFPS * 1000.0f / (float)div;
			m_nextFPS = 0;
			m_previousTime = GetTime();
		}

		m_nextFPS++;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::ControlFPS()
	{
		int64_t ns = 1000000;

		int64_t frameTime = ns / m_targetFPS;
		auto d = GetTime() - m_startFrameTime;

		if (d > frameTime)
		{
			m_startFrameTime = GetTime();
		}
		else
		{
			auto sleepTime = ((frameTime + m_startFrameTime) - GetTime()) / 1000;

			if (sleepTime > 1)
			{
				Sleep(sleepTime - 1);
			}

			do
			{
				d = GetTime() - m_startFrameTime;
			} while (frameTime > d);

			m_startFrameTime += frameTime;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::SetRemovedFunctionPpointer(CoreFuncPtr func)
	{
		m_removedFuncPtr = func;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Core_Imp::Initialize(const achar* title, int32_t width, int32_t height, bool isFullScreen, bool isOpenGLMode, bool isMultithreadingMode)
	{
		if (m_window != nullptr) return false;
		if (m_keyboard != nullptr) return false;
		if (m_mouse != nullptr) return false;
		if (m_joystickContainer != nullptr) return false;
		if (m_file != nullptr) return false;
		if (m_logger != nullptr) return false;
		if (m_profiler != nullptr) return false;

		m_isInitializedByExternal = false;

#if _WIN32
		if (isOpenGLMode)
		{
			glfwMakeOpenGLEnabled();
		}
		else
		{
			glfwMakeOpenGLDisabled();
		}
#else
		if (!isOpenGLMode)
		{
			return false;
		}
#endif

		m_logger = Log_Imp::Create(ToAString("Log.html").c_str(), title);

		m_window = Window_Imp::Create(width, height, title, m_logger);
		if (m_window == nullptr) return false;

		m_keyboard = Keyboard_Imp::Create(m_window);
		m_mouse = Mouse_Imp::Create(m_window);
		m_joystickContainer = JoystickContainer_Imp::Create();

		m_file = File_Imp::Create();

		m_graphics = Graphics_Imp::Create(m_window, isOpenGLMode, m_logger, isMultithreadingMode);
		if (m_graphics == nullptr) return false;

		m_sound = new Sound_Imp();

		m_objectSystemFactory = new ObjectSystemFactory_Imp(m_graphics, m_logger, m_window->GetSize());
		m_profiler = Profiler_Imp::Create();
		m_profilerViewer = ProfilerViewer_Imp::Create(m_profiler, m_graphics, m_logger, m_window->GetSize());

		m_animationSyatem = new AnimationSystem_Imp();

		m_logger->WriteLineStrongly(L"コア初期化成功");

		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Core_Imp::InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, bool isOpenGLMode, bool isMultithreadingMode)
	{
		if (m_window != nullptr) return false;
		if (m_keyboard != nullptr) return false;
		if (m_mouse != nullptr) return false;
		if (m_joystickContainer != nullptr) return false;
		if (m_file != nullptr) return false;
		if (m_logger != nullptr) return false;

		m_isInitializedByExternal = true;

#if _WIN32
		if (isOpenGLMode)
		{
			glfwMakeOpenGLEnabled();
		}
		else
		{
			glfwMakeOpenGLDisabled();
		}
#else
		if (!isOpenGLMode)
		{
			return false;
		}
#endif

		m_logger = Log_Imp::Create(ToAString("Log.html").c_str(), ToAString(L"").c_str());

		m_graphics = Graphics_Imp::Create(handle1, handle2, width, height, false, m_logger, isMultithreadingMode);
		if (m_graphics == nullptr) return false;

		m_sound = new Sound_Imp();

		m_objectSystemFactory = new ObjectSystemFactory_Imp(m_graphics, m_logger, Vector2DI(width,height));

		m_file = File_Imp::Create();
		m_profiler = Profiler_Imp::Create();
		m_profilerViewer = ProfilerViewer_Imp::Create(m_profiler, m_graphics, m_logger, Vector2DI(width, height));

		m_animationSyatem = new AnimationSystem_Imp();

		m_logger->WriteLineStrongly(L"コア初期化成功");

		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Core_Imp::DoEvents()
	{
		if (m_isInitializedByExternal) return true;

		assert(m_window != nullptr);
		assert(m_keyboard != nullptr);
		assert(m_mouse != nullptr);
		assert(m_logger != nullptr);
		assert(m_joystickContainer != nullptr);

		ControlFPS();
		ComputeFPS();

		m_keyboard->RefreshInputState();
		m_mouse->RefreshInputState();
		m_joystickContainer->RefreshJoysticks();

		return m_window->DoEvent();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::Update()
	{
		assert(0);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::Terminate()
	{
		SafeRelease(m_currentScene);
		SafeDelete(m_objectSystemFactory);

		SafeRelease(m_profiler);
		SafeDelete(m_profilerViewer);

		SafeRelease(m_sound);
		SafeRelease(m_graphics);
		SafeRelease(m_window);
		SafeDelete(m_keyboard);
		SafeDelete(m_mouse);
		SafeDelete(m_logger);
		SafeDelete(m_joystickContainer);
		SafeDelete(m_file);

		SafeDelete(m_animationSyatem);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::Reload()
	{
		GetGraphics_Imp()->GetResourceContainer()->Reload();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::BeginDrawing()
	{
		m_graphics->Begin();
		m_graphics->Clear(true, false, Color(0, 0, 0, 255));
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::EndDrawing()
	{
		m_graphics->Present();
		m_graphics->End();

		// スクリーンショット撮影
		for (auto& ss : m_screenShots)
		{
			m_graphics->SaveScreenshot(ss.c_str());
		}
		m_screenShots.clear();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::Draw()
	{
		m_profilerViewer->Draw();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::ChangeScene(CoreScene* scene)
	{
		SafeSubstitute(m_currentScene, scene);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::TakeScreenshot(const achar* path)
	{
		m_screenShots.push_back(path);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	float Core_Imp::GetCurrentFPS()
	{
		return m_currentFPS;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	int32_t Core_Imp::GetTargetFPS()
	{
		return m_targetFPS;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::SetTargetFPS(int32_t fps)
	{
		m_targetFPS = fps;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void* Core_Imp::GetWindowHandle() const
	{
		return m_window->GetWindowHandle();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Keyboard* Core_Imp::GetKeyboard()
	{
		return m_keyboard;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Mouse* Core_Imp::GetMouse()
	{
		return m_mouse;
	}

	File* Core_Imp::GetFile()
	{
		return m_file;
	}
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Log* Core_Imp::GetLogger()
	{
		return m_logger;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Profiler* Core_Imp::GetProfiler()
	{
		return m_profiler;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Graphics* Core_Imp::GetGraphics()
	{
		return m_graphics;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Graphics_Imp* Core_Imp::GetGraphics_Imp()
	{
		return m_graphics;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Sound* Core_Imp::GetSound()
	{
		return m_sound;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	JoystickContainer* Core_Imp::GetJoystickContainer()
	{
		return m_joystickContainer;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ObjectSystemFactory* Core_Imp::GetObjectSystemFactory()
	{
		return m_objectSystemFactory;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	AnimationSystem* Core_Imp::GetAnimationSyatem()
	{
		return m_animationSyatem;
	}
};

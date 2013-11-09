
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Core_Imp.h"
#include "../Window/ace.Window_Imp.h"
#include "../Input/ace.Keyboard_Imp.h"
#include "../Input/ace.Mouse_Imp.h"
#include "../Input/ace.JoystickContainer_Imp.h"
#include "../Log/ace.Log_Imp.h"
#include "../Profiler/ace.Profiler_Imp.h"
#include "../Profiler/ace.ProfilerViewer_Imp.h"
#include "../ObjectSystem/ace.ObjectSystemFactory_Imp.h"

#include "../Graphics/Common/ace.Graphics_Imp.h"
#include "../Graphics/Common/Resource/ace.RenderState_Imp.h"

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
		, m_joystickContainer(nullptr)
		, m_logger(nullptr)
		, m_profiler(nullptr)
		, m_currentScene(nullptr)
		, m_isInitializedByExternal(false)
		, m_objectSystemFactory(nullptr)
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Core_Imp::~Core_Imp()
	{
		Terminate();
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
	bool Core_Imp::Initialize(const achar* title, int32_t width, int32_t height, bool isFullScreen, bool isOpenGLMode)
	{
		if (m_window != nullptr) return false;
		if (m_keyboard != nullptr) return false;
		if (m_mouse != nullptr) return false;
		if (m_joystickContainer != nullptr) return false;
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

		m_window = Window_Imp::Create(width, height, title);
		m_keyboard = Keyboard_Imp::Create(m_window);
		m_mouse = Mouse_Imp::Create(m_window);
		m_logger = Log_Imp::Create(ToAString("Log.html").c_str(), title);
		m_joystickContainer = JoystickContainer_Imp::Create();
		m_graphics = Graphics_Imp::Create(m_window, isOpenGLMode, m_logger);
		m_objectSystemFactory = new ObjectSystemFactory_Imp(m_graphics, m_logger, m_window->GetSize());

		m_profiler = Profiler_Imp::Create();
		m_profilerViewer = ProfilerViewer_Imp::Create(m_profiler, m_graphics, m_logger, m_window->GetSize());
		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Core_Imp::InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height)
	{
		if (m_window != nullptr) return false;
		if (m_keyboard != nullptr) return false;
		if (m_mouse != nullptr) return false;
		if (m_joystickContainer != nullptr) return false;
		if (m_logger != nullptr) return false;

		m_isInitializedByExternal = true;

		m_logger = Log_Imp::Create(ToAString("Log.html").c_str(), ToAString(L"").c_str());

		m_graphics = Graphics_Imp::Create(handle1, handle2, width, height, false, m_logger);

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
		m_graphics->Begin();

		m_graphics->Clear(true, false, Color(0, 0, 0, 255));

		//m_profilerViewer->Draw();

		m_graphics->Present();

		m_graphics->End();
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

		SafeRelease(m_graphics);
		SafeDelete(m_window);
		SafeDelete(m_keyboard);
		SafeDelete(m_mouse);
		SafeDelete(m_logger);
		SafeDelete(m_joystickContainer);
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
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::Draw()
	{
		if (m_currentScene != nullptr)
		{
			m_currentScene->Draw();
		}
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
	JoystickContainer* Core_Imp::GetJoystickContainer()
	{
		return m_joystickContainer;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ObjectSystemFactory* Core_Imp::GetObjectSystemFactory()
	{
		return GetObjectSystemFactoryImp();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ObjectSystemFactory_Imp* Core_Imp::GetObjectSystemFactoryImp()
	{
		return m_objectSystemFactory;
	}
};

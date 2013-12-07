
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "ace.Engine.h"

#if _WIN32

#else
#include <dlfcn.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	DLL情報読み取りクラス
	*/
	class DynamicLinkLibrary
	{
	private:
#if _WIN32
		HMODULE	m_dll;
#else
		void*	m_dll;
#endif

	public:
		DynamicLinkLibrary()
		{
			m_dll = nullptr;
		}

		~DynamicLinkLibrary()
		{
			Reset();
		}

		void Reset()
		{
			if (m_dll != NULL)
			{
#if _WIN32
				::FreeLibrary(m_dll);
#else
				dlclose(m_dll);
#endif
				m_dll = NULL;
			}
		}

		/**
		@brief	DLLファイルを読み取る。
		@param	path	パス
		@return	成否
		*/
		bool Load(const char* path)
		{
			Reset();

#if _WIN32
			m_dll = ::LoadLibraryA(path);
#else
			m_dll = dlopen(path, RTLD_LAZY);
#endif
			if (m_dll == nullptr) return false;

			return true;
		}

		/**
		@brief	DLLから関数を読み取る。
		@param	name	関数名
		@return	関数ポインタ
		*/
		template<typename T>
		T GetProc(const char* name)
		{
#if _WIN32
			void* pProc = ::GetProcAddress(m_dll, name);
#else
			void* pProc = dlsym(m_dll, name);
#endif
			if (pProc == NULL)
			{
				return nullptr;
			}
			return (T)(pProc);
		}
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	typedef int32_t(ACE_STDCALL *GetIntFunc)();

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	static Engine*	g_engine = nullptr;
	static Keyboard* g_keyboard = nullptr;
	static Mouse* g_mouse = nullptr;
	static JoystickContainer* g_joystickContainer = nullptr;
	static Log* g_logger = nullptr;
	static Profiler* g_profiler = nullptr;
	static std::shared_ptr <DynamicLinkLibrary>	g_dll = nullptr;
	static Graphics* g_graphics = nullptr;
	ObjectSystemFactory* g_objectSystemFactory = nullptr;

	static GetIntFunc g_GetGlobalRef = nullptr;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	static void ReleaseDLL()
	{
		if (g_dll != NULL)
		{
			g_GetGlobalRef = nullptr;

			g_dll->Reset();
			g_dll.reset();
		}
	}

	bool Engine::HasDLL(const char* path)
	{
#if _WIN32
		auto dll = ::LoadLibraryA(path);
		if (dll != nullptr)
		{
			FreeLibrary(dll);
		}
		else
		{
			return false;
		}
#endif
		return true;
	}

	bool Engine::CheckDLL()
	{
		return true;
#if _WIN32
		if (!HasDLL("D3DCOMPILER_47.dll"))
		{
			MessageBoxA(nullptr, "最新のDirectXEndUserRuntime？をインストールしてください。", "Error", MB_OK);
			return false;
		}
#endif
		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Engine::Engine(Core* core)
		: m_core(core)
		, m_currentScene(nullptr)
	{

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Engine::~Engine()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Engine::Initialize(const achar* title, int32_t width, int32_t height, EngineOption option)
	{
		if (!CheckDLL()) return false;

		if (this == nullptr) return false;

		bool init = m_core->Initialize(title, width, height, option.IsFullScreen, option.GraphicsType != eGraphicsType::GRAPHICS_TYPE_DX11);
		if (init)
		{
			g_keyboard = m_core->GetKeyboard();
			g_mouse = m_core->GetMouse();
			g_logger = m_core->GetLogger();
			g_profiler = m_core->GetProfiler();
			g_joystickContainer = m_core->GetJoystickContainer();
			g_objectSystemFactory = m_core->GetObjectSystemFactory();
			g_graphics = m_core->GetGraphics();
		}

		return init;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Engine::InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, EngineOption option)
	{
		if (!CheckDLL()) return false;

		if (this == nullptr) return false;

		bool init = m_core->InitializeByExternalWindow(handle1, handle2, width, height);
		if (init)
		{
			g_logger = m_core->GetLogger();
			g_profiler = m_core->GetProfiler();
			g_objectSystemFactory = m_core->GetObjectSystemFactory();
			g_graphics = m_core->GetGraphics();
		}
		return init;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Engine::DoEvents()
	{
		if (this == nullptr) return false;

		return m_core->DoEvents();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Engine::Update()
	{
		if (this == nullptr) return;

		if (m_currentScene != nullptr)
		{
			m_currentScene->Update();
		}

		m_core->BeginDrawing();

		if (m_currentScene != nullptr)
		{
			m_currentScene->BeginDrawing();
		}

		m_core->Draw();

		if (m_currentScene != nullptr)
		{
			m_currentScene->DrawAdditionally();
		}

		if (m_currentScene != nullptr)
		{
			m_currentScene->EndDrawing();
		}

		m_core->EndDrawing();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Engine::Terminate()
	{
		if (this == nullptr) return;

		m_currentScene.reset();

		m_core->Terminate();

		SafeRelease(m_core);

		delete this;
		g_engine = nullptr;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Engine::ChangeScene(std::shared_ptr<Scene>& scene)
	{
		m_currentScene = scene;
		m_core->ChangeScene(scene->m_coreScene.get());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Engine::TakeScreenshot(const achar* path)
	{
		m_core->TakeScreenshot(path);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Engine* GetEngine()
	{
		// DLLからエンジンを生成する。
		if (g_engine == nullptr)
		{
			g_dll = std::shared_ptr<DynamicLinkLibrary>(new DynamicLinkLibrary());

#if _WIN32
#if _DEBUG
			const char* path = "ace_core.dll";
#else
			const char* path = "ace_core.dll";
#endif
#else
#if _DEBUG
			const char* path = "libace_core.so";
#else
			const char* path = "libace_core.so";
#endif
#endif
			if (!g_dll->Load(path))
			{
				g_dll.reset();
				return nullptr;
			}

			{
#if _WIN32
				const char* name = "_CreateCore@0";
#else
				const char* name = "CreateCore";
#endif
				auto pProc = g_dll->GetProc<Core*(*)()>(name);
				if (pProc == NULL)
				{
					return nullptr;
				}
	
				auto core = pProc();
				g_engine = new Engine(core);
			}

			{
#if _WIN32
				const char* name = "_GetGlobalReferenceCount__@0";
#else
				const char* name = "GetGlobalReferenceCount__";
#endif
				auto pProc = g_dll->GetProc<GetIntFunc>(name);
				if (pProc == NULL)
				{
					return nullptr;
				}

				g_GetGlobalRef = pProc;
			}
		}

		return g_engine;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Keyboard* GetKeyboard()
	{
		return g_keyboard;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Mouse* GetMouse()
	{
		return g_mouse;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Log* GetLogger()
	{
		return g_logger;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Profiler* GetProfiler()
	{
		return g_profiler;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	JoystickContainer* GetJoystickContainer()
	{
		return g_joystickContainer;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Graphics* GetGraphics()
	{
		return g_graphics;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	int32_t GetGlobalReferenceCount()
	{
		if (g_GetGlobalRef == nullptr) return 0;
		auto ref = g_GetGlobalRef();
		return ref;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CheckDLL()
	{
		if (GetGlobalReferenceCount() == 0)
		{
			ReleaseDLL();
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

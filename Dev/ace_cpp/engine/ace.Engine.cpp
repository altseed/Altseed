
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
		mutable std::atomic<int32_t> m_reference;

#if _WIN32
		HMODULE	m_dll;
#else
		void*	m_dll;
#endif

	public:
		DynamicLinkLibrary()
			: m_reference(1)
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

		int AddRef()
		{
			std::atomic_fetch_add_explicit(&m_reference, 1, std::memory_order_consume);
			return m_reference;
		}

		int GetRef()
		{
			return m_reference;
		}

		int Release()
		{
			assert(m_reference > 0);
			bool destroy = std::atomic_fetch_sub_explicit(&m_reference, 1, std::memory_order_consume) == 1;
			if (destroy)
			{
				delete this;
				return 0;
			}

			return m_reference;
		}
	};

	struct DynamicLinkLibraryDeleter
	{
		void operator ()(DynamicLinkLibrary* p)
		{
			SafeRelease(p);
		}
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	typedef int32_t(ACE_STDCALL *GetIntFunc)();

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	
	static std::shared_ptr <DynamicLinkLibrary>	g_dll = nullptr;
	static GetIntFunc g_GetGlobalRef = nullptr;
	ObjectSystemFactory* g_objectSystemFactory = nullptr;
	
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	static void ReleaseDLL()
	{
		if (g_dll != NULL)
		{
			if (g_dll->GetRef() == 1)
			{
				g_GetGlobalRef = nullptr;
				g_dll->Reset();
				g_dll.reset();
			}
			else
			{
				g_dll->Release();
			}
		}
	}

	static void ACE_STDCALL RemovedCore(Core* core)
	{
		ReleaseDLL();
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
	bool Engine::GenerateCore()
	{
		// DLLからコアを生成する。
		if (m_core == nullptr)
		{
			g_dll = std::shared_ptr<DynamicLinkLibrary>(new DynamicLinkLibrary(), DynamicLinkLibraryDeleter());

#if _WIN32
#if _DEBUG
			const char* path = "ace_core.Debug.dll";
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
				return false;
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
					return false;
				}

				m_core = pProc();
				m_core->SetRemovedFunctionPpointer(RemovedCore);
				SafeAddRef(g_dll);
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
					return false;
				}

				g_GetGlobalRef = pProc;
			}
		}

		return true;
	}


	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Core* Engine::m_core = nullptr;
	Keyboard* Engine::m_keyboard = nullptr;
	Mouse* Engine::m_mouse = nullptr;
	JoystickContainer* Engine::m_joystickContainer = nullptr;
	Log* Engine::m_logger = nullptr;
	Profiler* Engine::m_profiler = nullptr;
	Sound* Engine::m_sound = nullptr;
	Graphics* Engine::m_graphics = nullptr;
	ObjectSystemFactory* Engine::m_objectSystemFactory = nullptr;
	AnimationSystem* Engine::m_animationSyatem = nullptr;

	std::shared_ptr<Scene>	Engine::m_currentScene;
	std::shared_ptr<Scene>	Engine::m_nextScene;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Engine::Engine()
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

		if (!GenerateCore()) return false;

		bool init = m_core->Initialize(title, width, height, option.IsFullScreen, option.GraphicsType != eGraphicsType::GRAPHICS_TYPE_DX11, option.IsMultithreadingMode);
		if (init)
		{
			m_logger = m_core->GetLogger();
			m_profiler = m_core->GetProfiler();
			m_objectSystemFactory = m_core->GetObjectSystemFactory();
			m_sound = m_core->GetSound();
			m_graphics = m_core->GetGraphics();
			m_animationSyatem = m_core->GetAnimationSyatem();

			m_keyboard = m_core->GetKeyboard();
			m_mouse = m_core->GetMouse();
			m_joystickContainer = m_core->GetJoystickContainer();

			g_objectSystemFactory = m_objectSystemFactory;
		}

		return init;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Engine::InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, EngineOption option)
	{
		if (!CheckDLL()) return false;

		if (!GenerateCore()) return false;

		bool init = m_core->InitializeByExternalWindow(handle1, handle2, width, height, option.GraphicsType != eGraphicsType::GRAPHICS_TYPE_DX11, option.IsMultithreadingMode);
		if (init)
		{
			m_logger = m_core->GetLogger();
			m_profiler = m_core->GetProfiler();
			m_objectSystemFactory = m_core->GetObjectSystemFactory();
			m_sound = m_core->GetSound();
			m_graphics = m_core->GetGraphics();
			m_animationSyatem = m_core->GetAnimationSyatem();

			g_objectSystemFactory = m_objectSystemFactory;
		}
		return init;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Engine::DoEvents()
	{
		if (m_core == nullptr) return false;

		if (m_nextScene != nullptr)
		{
			m_currentScene = m_nextScene;
			m_core->ChangeScene(m_nextScene->m_coreScene.get());
		}

		return m_core->DoEvents();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Engine::Update()
	{
		if (m_core == nullptr) return;

		m_core->BeginDrawing();

		if (m_currentScene != nullptr)
		{
			m_currentScene->Update();
		}
		
		if (m_currentScene != nullptr)
		{
			m_currentScene->Draw();
		}

		m_core->Draw();

		m_core->EndDrawing();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Engine::Terminate()
	{
		if (m_core == nullptr) return;

		m_currentScene.reset();
		m_nextScene.reset();

		m_core->Terminate();

		SafeRelease(m_core);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Engine::ChangeScene(ScenePtr& scene)
	{
		m_nextScene = scene;
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
	float Engine::GetCurrentFPS()
	{
		return m_core->GetCurrentFPS();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	int32_t Engine::GetTargetFPS()
	{
		return m_core->GetTargetFPS();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Engine::SetTargetFPS(int32_t fps)
	{
		m_core->SetTargetFPS(fps);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Keyboard* Engine::GetKeyboard()
	{
		return m_keyboard;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Mouse* Engine::GetMouse()
	{
		return m_mouse;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Log* Engine::GetLogger()
	{
		return m_logger;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Profiler* Engine::GetProfiler()
	{
		return m_profiler;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	JoystickContainer* Engine::GetJoystickContainer()
	{
		return m_joystickContainer;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Graphics* Engine::GetGraphics()
	{
		return m_graphics;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Sound* Engine::GetSound()
	{
		return m_sound;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	AnimationSystem* Engine::GetAnimationSyatem()
	{
		return m_animationSyatem;
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

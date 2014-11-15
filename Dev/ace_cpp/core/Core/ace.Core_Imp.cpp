
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
#include "../ObjectSystem/Transition/ace.CoreTransition_Imp.h"

#include "../Sound/ace.Sound_Imp.h"

#include "../Graphics/ace.Graphics_Imp.h"

#include "../Graphics/3D/Resource/Animation/ace.AnimationSystem_Imp.h"
#include "../Graphics/2D/ace.LayerRenderer.h"

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
		, m_windowSize(Vector2DI(0,0))
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
	bool Core_Imp::Initialize(const achar* title, int32_t width, int32_t height, CoreOption option)
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
		if (option.GraphicsDevice == GraphicsDeviceType::OpenGL)
		{
			glfwMakeOpenGLEnabled();
		}
		else
		{
			glfwMakeOpenGLDisabled();
		}
#else
		if (option.GraphicsDevice != GraphicsDeviceType::OpenGL)
		{
			return false;
		}
#endif

		m_logger = Log_Imp::Create(ToAString("Log.html").c_str(), title);

		m_window = Window_Imp::Create(width, height, title, m_logger, option.IsFullScreen);
		if (m_window == nullptr) return false;

		m_keyboard = Keyboard_Imp::Create(m_window);
		m_mouse = Mouse_Imp::Create(m_window);
		m_joystickContainer = JoystickContainer_Imp::Create();

		m_file = File_Imp::Create();

		m_graphics = Graphics_Imp::Create(m_window, option.GraphicsDevice, m_logger, option.IsReloadingEnabled, option.IsFullScreen);
		if (m_graphics == nullptr) return false;

		m_sound = new Sound_Imp(option.IsReloadingEnabled);

		m_objectSystemFactory = new ObjectSystemFactory_Imp(this, m_graphics, m_logger, m_window->GetSize());
		m_profiler = Profiler_Imp::Create();
		m_profilerViewer = ProfilerViewer_Imp::Create(m_profiler, m_graphics, m_logger, m_window->GetSize());

		m_animationSyatem = new AnimationSystem_Imp();

		m_windowSize = Vector2DI(width, height);
		layerRenderer = new LayerRenderer(m_graphics);
		layerRenderer->SetWindowSize(m_windowSize);

		{
			ace::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = m_windowSize.X;
			lpos[1].Y = 0;
			lpos[2].X = m_windowSize.X;
			lpos[2].Y = m_windowSize.Y;
			lpos[3].X = 0;
			lpos[3].Y = m_windowSize.Y;
			layerRenderer->SetLayerPosition(lpos);
		}

		m_logger->WriteLineStrongly(L"コア初期化成功");

		isReloadingEnabeld = option.IsReloadingEnabled;
		m_window->OnFocused = [this]() ->void
		{
			if (this->isReloadingEnabeld)
			{
				this->Reload();
			}
		};

		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Core_Imp::InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, CoreOption option)
	{
		if (m_window != nullptr) return false;
		if (m_keyboard != nullptr) return false;
		if (m_mouse != nullptr) return false;
		if (m_joystickContainer != nullptr) return false;
		if (m_file != nullptr) return false;
		if (m_logger != nullptr) return false;

		m_isInitializedByExternal = true;

#if _WIN32
		if (option.GraphicsDevice == GraphicsDeviceType::OpenGL)
		{
			glfwMakeOpenGLEnabled();
		}
		else
		{
			glfwMakeOpenGLDisabled();
		}
#else
		if (option.GraphicsDevice != GraphicsDeviceType::OpenGL)
		{
			return false;
		}
#endif

		m_logger = Log_Imp::Create(ToAString("Log.html").c_str(), ToAString(L"").c_str());

		m_graphics = Graphics_Imp::Create(handle1, handle2, width, height, option.GraphicsDevice, m_logger, option.IsReloadingEnabled, option.IsFullScreen);
		if (m_graphics == nullptr) return false;

		m_sound = new Sound_Imp(option.IsReloadingEnabled);

		m_objectSystemFactory = new ObjectSystemFactory_Imp(this, m_graphics, m_logger, Vector2DI(width, height));

		m_file = File_Imp::Create();
		m_profiler = Profiler_Imp::Create();
		m_profilerViewer = ProfilerViewer_Imp::Create(m_profiler, m_graphics, m_logger, Vector2DI(width, height));

		m_animationSyatem = new AnimationSystem_Imp();

		m_windowSize = Vector2DI(width, height);
		layerRenderer = new LayerRenderer(m_graphics);
		layerRenderer->SetWindowSize(m_windowSize);

		{
			ace::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = m_windowSize.X;
			lpos[1].Y = 0;
			lpos[2].X = m_windowSize.X;
			lpos[2].Y = m_windowSize.Y;
			lpos[3].X = 0;
			lpos[3].Y = m_windowSize.Y;
			layerRenderer->SetLayerPosition(lpos);
		}

		m_logger->WriteLineStrongly(L"コア初期化成功");

		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Core_Imp::DoEvents()
	{
		if (!m_isInitializedByExternal)
		{
			assert(m_window != nullptr);
			assert(m_keyboard != nullptr);
			assert(m_mouse != nullptr);
			assert(m_logger != nullptr);
			assert(m_joystickContainer != nullptr);
		}

		ControlFPS();
		ComputeFPS();

		if (m_isInitializedByExternal)
		{
		}
		else
		{
			m_keyboard->RefreshInputState();
			m_mouse->RefreshInputState();
			m_joystickContainer->RefreshJoysticks();
		}

		// 経過時間計算
		{
			if (deltaTimePreviousTime == 0)
			{
				deltaTimePreviousTime = GetTime();
			}

			auto delta = GetTime() - deltaTimePreviousTime;
			deltaTimePreviousTime = GetTime();
			
			if (framerateMode == FramerateMode::Constant)
			{
				deltaTime = (1.0f / (float) m_targetFPS) * timeSpan;
			}
			else if (framerateMode == FramerateMode::Variable)
			{
				deltaTime = delta / (1000.0f) * timeSpan;
			}
		}

		if (m_isInitializedByExternal)
		{
			return true;
		}

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

		SafeRelease(layerRenderer);

		SafeRelease(m_profiler);
		SafeDelete(m_profilerViewer);

		SafeRelease(m_sound);
		SafeRelease(m_graphics);
		SafeRelease(m_window);
		SafeDelete(m_keyboard);
		SafeDelete(m_mouse);
		SafeDelete(m_logger);
		SafeDelete(m_joystickContainer);
		SafeRelease(m_file);

		SafeDelete(m_animationSyatem);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::Reload()
	{
		if (isReloadingEnabeld)
		{
			GetGraphics_Imp()->Reload();
			m_sound->Reload();
		}
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
	
	void Core_Imp::DrawSceneToWindow(CoreScene* scene)
	{
		m_graphics->SetRenderTarget(nullptr, nullptr);

		layerRenderer->SetTexture(scene->GetBaseTarget());

		{
			{
				ace::Vector2DF positions[4];
				ace::Color colors[4];
				ace::Vector2DF uvs[4];

				colors[0] = ace::Color(255, 255, 255, 255);
				colors[1] = ace::Color(255, 255, 255, 255);
				colors[2] = ace::Color(255, 255, 255, 255);

				positions[0].X = 0.0f;
				positions[0].Y = 0.0f;
				positions[1].X = 1.0f;
				positions[1].Y = 0.0f;
				positions[2].X = 1.0f;
				positions[2].Y = 1.0f;


				uvs[0].X = 0;
				uvs[0].Y = 0;
				uvs[1].X = 1;
				uvs[1].Y = 0;
				uvs[2].X = 1;
				uvs[2].Y = 1;

				layerRenderer->AddTriangle(positions, colors, uvs);
			}

				{
					ace::Vector2DF positions[4];
					ace::Color colors[4];
					ace::Vector2DF uvs[4];

					colors[0] = ace::Color(255, 255, 255, 255);
					colors[1] = ace::Color(255, 255, 255, 255);
					colors[2] = ace::Color(255, 255, 255, 255);

					positions[0].X = 0.0f;
					positions[0].Y = 1.0f;
					positions[1].X = 1.0f;
					positions[1].Y = 1.0f;
					positions[2].X = 0.0f;
					positions[2].Y = 0.0f;

					uvs[0].X = 0;
					uvs[0].Y = 1;
					uvs[1].X = 1;
					uvs[1].Y = 1;
					uvs[2].X = 0;
					uvs[2].Y = 0;

					layerRenderer->AddTriangle(positions, colors, uvs);
				}
		}

		layerRenderer->DrawCache();
		layerRenderer->ClearCache();
	}

	void Core_Imp::DrawSceneToWindowWithTransition(CoreScene* nextScene, CoreScene* previousScene, CoreTransition* transition)
	{
		auto t = (CoreTransition_Imp*) transition;

		m_graphics->SetRenderTarget(nullptr, nullptr);

		t->DrawCache(layerRenderer, nextScene, previousScene);
		t->ClearCache();
	}

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

	void Core_Imp::Close()
	{
		m_window->Close();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::TakeScreenshot(const achar* path)
	{
		m_screenShots.push_back(path);
	}

	float Core_Imp::GetDeltaTime() const
	{
		return deltaTime;
	}

	void Core_Imp::SetDeltaTime(float deltaTime)
	{
		this->deltaTime = deltaTime;
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

	float Core_Imp::GetTimeSpan() const
	{
		return timeSpan;
	}

	void Core_Imp::SetTimeSpan(float timeSpan)
	{
		this->timeSpan = timeSpan;
	}


	FramerateMode Core_Imp::GetFramerateMode() const
	{
		return framerateMode;
	}

	void Core_Imp::SetFramerateMode(FramerateMode framerateMode)
	{
		this->framerateMode = framerateMode;
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

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector2DI Core_Imp::GetWindowSize()
	{
		return m_windowSize;
	}
};

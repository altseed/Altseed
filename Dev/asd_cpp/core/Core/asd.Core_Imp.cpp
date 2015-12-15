
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Core_Imp.h"
#include "../Window/asd.Window_Imp.h"
#include "../Window/asd.Cursor_Imp.h"
#include "../Input/asd.Keyboard_Imp.h"
#include "../Input/asd.Mouse_Imp.h"
#include "../Input/asd.JoystickContainer_Imp.h"
#include "../IO/asd.File_Imp.h"
#include "../Log/asd.Log_Imp.h"
#include "../Log/asd.GetSpec.h"

#include "../Profiler/asd.Profiler_Imp.h"
#include "../Profiler/asd.LayerProfiler_Imp.h"
#include "../Profiler/asd.ProfilerViewer_Imp.h"

#include "../ObjectSystem/asd.ObjectSystemFactory_Imp.h"
#include "../ObjectSystem/Transition/asd.CoreTransition_Imp.h"

#include "../Sound/asd.Sound_Imp.h"

#include "../Graphics/3D/Resource/Animation/asd.AnimationSystem_Imp.h"
#include "../Graphics/2D/asd.LayerRenderer.h"

#include <Utility/asd.Timer.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd
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
		, m_isProfilerVisible(false)
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
	void WriteSystemSpecToLog(Log* log) {
		log->WriteLineStrongly("システム情報");

		log->BeginTable();

		log->Write("CPU名");
		log->ChangeColumn();
		log->Write(GetCPUName().c_str());
		log->ChangeRow();
		
		log->Write("OS情報");
		log->ChangeColumn();
#if defined(_WIN32)
		log->Write("OS: Windows\n");
		log->Write(GetWindowsVersion().c_str());
#else
		log->Write("Unavailable");
#endif
		log->ChangeRow();
		
		log->Write("メモリ情報");
		log->ChangeColumn();
		log->Write(GetMemoryInfo().c_str());

		log->EndTable();
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

		if (!option.IsFullScreen && option.WindowPosition == WindowPositionType::Centering)
		{
			auto monitorSize = m_window->GetPrimaryMonitorSize();
			
			if (monitorSize.X > 0)
			{
				auto offset = (monitorSize - Vector2DI(width, height)) / 2;
				offset += m_window->GetPrimaryMonitorPosition();

				m_window->SetWindowPosition(offset);
			}
		}
		m_window->ShowWindow();

		m_keyboard = Keyboard_Imp::Create(m_window);
		m_mouse = Mouse_Imp::Create(m_window);
		m_joystickContainer = JoystickContainer_Imp::Create();

		m_file = File_Imp::Create();

		m_graphics = Graphics_Imp::Create(m_window, option.GraphicsDevice, m_logger,m_file, option.IsReloadingEnabled, option.IsFullScreen);
		if (m_graphics == nullptr) return false;

		m_sound = new Sound_Imp(m_file, m_logger, option.IsReloadingEnabled);

		m_profiler = Profiler_Imp::Create();

		m_layerProfiler = LayerProfiler_Imp::Create();

		m_profilerViewer = ProfilerViewer_Imp::Create(this, m_profiler, m_layerProfiler, m_graphics, m_logger, m_window->GetSize());

		m_objectSystemFactory = new ObjectSystemFactory_Imp(this, m_graphics, m_logger, m_window->GetSize());

		m_animationSyatem = new AnimationSystem_Imp();

		m_windowSize = Vector2DI(width, height);

		layerRenderer = new LayerRenderer(m_graphics);
		layerRenderer->SetWindowSize(m_windowSize);

		{
			asd::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = (float)m_windowSize.X;
			lpos[1].Y = 0;
			lpos[2].X = (float)m_windowSize.X;
			lpos[2].Y = (float)m_windowSize.Y;
			lpos[3].X = 0;
			lpos[3].Y = (float)m_windowSize.Y;
			layerRenderer->SetLayerPosition(lpos);
		}
		
		m_logger->WriteHeading(L"システム");

		WriteSystemSpecToLog(m_logger);
		
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

		m_file = File_Imp::Create();
		m_logger = Log_Imp::Create(ToAString("Log.html").c_str(), ToAString(L"").c_str());

		m_graphics = Graphics_Imp::Create(handle1, handle2, width, height, option.GraphicsDevice, m_logger,m_file, option.IsReloadingEnabled, option.IsFullScreen);
		if (m_graphics == nullptr) return false;

		m_sound = new Sound_Imp(m_file, m_logger, option.IsReloadingEnabled);

		m_objectSystemFactory = new ObjectSystemFactory_Imp(this, m_graphics, m_logger, Vector2DI(width, height));

		m_profiler = Profiler_Imp::Create();
		m_layerProfiler = LayerProfiler_Imp::Create();
		m_profilerViewer = ProfilerViewer_Imp::Create(this, m_profiler, m_layerProfiler, m_graphics, m_logger, Vector2DI(width, height));

		m_animationSyatem = new AnimationSystem_Imp();

		m_windowSize = Vector2DI(width, height);
		layerRenderer = new LayerRenderer(m_graphics);
		layerRenderer->SetWindowSize(m_windowSize);

		{
			asd::Vector2DF lpos[4];
			lpos[0].X = 0;
			lpos[0].Y = 0;
			lpos[1].X = (float)m_windowSize.X;
			lpos[1].Y = 0;
			lpos[2].X = (float)m_windowSize.X;
			lpos[2].Y = (float)m_windowSize.Y;
			lpos[3].X = 0;
			lpos[3].Y = (float)m_windowSize.Y;
			layerRenderer->SetLayerPosition(lpos);
		}
		
		m_logger->WriteHeading(L"システム");

		WriteSystemSpecToLog(m_logger);
		m_logger->WriteLineStrongly(L"コア初期化成功");

		return true;
	}

	void Core_Imp::SetTitle(const achar* title)
	{
		if (m_window == nullptr) return;
		m_window->SetTitle(title);
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
		for (auto& gifAnim : gifAnimations)
		{
			gifAnim->Helper->Finalize();
			gifAnim->Helper = nullptr;
		}
		gifAnimations.clear();

		SafeRelease(m_currentScene);
		SafeDelete(m_objectSystemFactory);

		SafeRelease(layerRenderer);

		SafeRelease(m_profiler);
		SafeRelease(m_layerProfiler);
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

		for (auto& gifAnim : gifAnimations)
		{
			if (gifAnim->Frequency == 1 || gifAnim->Time % (gifAnim->Frequency - 1) == 0)
			{
				std::vector<Color> bufs;
				Vector2DI size;
				m_graphics->SaveScreenshot(bufs, size);

				gifAnim->Helper->AddImage(bufs.data(), size.X, size.Y);
			}

			gifAnim->Time++;
		}

		for (auto& gifAnim : gifAnimations)
		{
			if (gifAnim->FrameCount * gifAnim->Frequency == gifAnim->Time)
			{
				gifAnim->Helper->Finalize();
				gifAnim->Helper = nullptr;
			}
		}

		auto it = std::remove_if(
			gifAnimations.begin(), gifAnimations.end(), 
			[](std::shared_ptr<GifAnimation> g)->bool { return g->Helper == nullptr; });
		gifAnimations.erase(it, gifAnimations.end());
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
				asd::Vector2DF positions[4];
				asd::Color colors[4];
				asd::Vector2DF uvs[4];

				colors[0] = asd::Color(255, 255, 255, 255);
				colors[1] = asd::Color(255, 255, 255, 255);
				colors[2] = asd::Color(255, 255, 255, 255);

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
					asd::Vector2DF positions[4];
					asd::Color colors[4];
					asd::Vector2DF uvs[4];

					colors[0] = asd::Color(255, 255, 255, 255);
					colors[1] = asd::Color(255, 255, 255, 255);
					colors[2] = asd::Color(255, 255, 255, 255);

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
		if (m_isProfilerVisible)
		{
			m_profilerViewer->Draw();
		}
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

	Cursor* Core_Imp::CreateCursor(const achar* path, Vector2DI hot)
	{
		return Cursor_Imp::Create(GetFile(), path, hot);
	}

	void Core_Imp::SetCursor(Cursor* cursor)
	{
		m_window->SetCursor(cursor);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Core_Imp::TakeScreenshot(const achar* path)
	{
		m_screenShots.push_back(path);
	}

	void Core_Imp::CaptureScreenAsGifAnimation(const achar* path, int32_t frame, float frequency_rate, float scale)
	{
		frequency_rate = Clamp(frequency_rate, 1.0f, 1.0f / GetTargetFPS());

		std::shared_ptr<GifAnimation> anim = std::make_shared<GifAnimation>();

		anim->Path = path;
		anim->Time = 0;
		anim->Frequency = (int32_t)(1.0f / frequency_rate);
		anim->FrameCount = frame;
		anim->Helper = std::make_shared<GifAnimationHelper>();

		anim->Helper->Initialize(path, m_windowSize.X, m_windowSize.Y, (int32_t)(GetTargetFPS() * frequency_rate), scale);

		gifAnimations.push_back(anim);
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


	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
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

	LayerProfiler* Core_Imp::GetLayerProfiler()
	{
		return m_layerProfiler;
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

	bool Core_Imp::GetProfilerVisibility() const
	{
		return m_isProfilerVisible;
	}

	void Core_Imp::SetProfilerVisibility(bool visible)
	{
		m_isProfilerVisible = visible;
	}
};

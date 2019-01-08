
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Window_Imp.h"
#include "asd.Cursor_Imp.h"

#include "../Log/asd.Log_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

Window_Imp::Window_Imp(Log* logger)
	: m_logger(logger)
{

}

Window_Imp::~Window_Imp()
{
	SafeRelease(currentCursor);
	SafeDelete(window);
}

Window_Imp* Window_Imp::Create(
	int32_t width,
	int32_t height,
	const achar* title,
	Log* logger,
	bool isResizable,
	WindowPositionType windowPositionType,
	GraphicsDeviceType graphicsDeviceType,
	bool isFullScreen)
{
	if (logger != nullptr) logger->WriteHeading("ウインドウ");

	auto ret = new Window_Imp(logger);
	auto window = ap::Window::Create();

	ap::WindowInitializationParameter initParam;
	initParam.WindowWidth = width;
	initParam.WindowHeight = height;
	initParam.Title = std::u16string(title);
	initParam.IsResizable = isResizable;
	initParam.WindowPosition = (ap::WindowPositionType)windowPositionType;
	initParam.ColorSpace = ap::ColorSpaceType::GammaSpace;
	initParam.GraphicsDevice = (ap::GraphicsDeviceType)graphicsDeviceType;
	initParam.IsFullscreenMode = isFullScreen;

	if (ret != nullptr && window != nullptr)
	{
		auto errorCode = window->Initialize(initParam);
		if (errorCode == ap::ErrorCode::OK)
		{
			ret->window = window;
			ret->m_size.X = width;
			ret->m_size.Y = height;

#if ( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )
#else
			auto glfwWindow = (GLFWwindow*)window->GetNativeWindow();

			window->Resized = ([ret](int x, int y) -> void
			{
				auto w = (Window_Imp*)ret;
				if (w->OnChangedSize != nullptr)
				{
					w->OnChangedSize(x, y);
				}
			});

			window->Focused = [ret](bool b) -> void
			{
				auto w = (Window_Imp*)ret;
				if (w->OnFocused && b)
				{
					w->OnFocused();
				}
			};
#endif

			if (logger != nullptr) logger->WriteLine("ウインドウ作成成功");
		}
		else
		{
			if (logger != nullptr) logger->WriteLine("ウインドウ作成失敗");
			goto End;
		}
	}
	else
	{
		if (logger != nullptr) logger->WriteLine("ウインドウ作成失敗");
		goto End;
	}

	return ret;

End:;
	SafeRelease(ret);
	SafeDelete(window);
	return nullptr;
}

bool Window_Imp::DoEvent()
{
	return window->DoEvent();
}

void Window_Imp::SetTitle(const achar* title)
{
	window->SetTitle(title);
}

void Window_Imp::Close()
{
	window->Close();
}

void* Window_Imp::GetWindowHandle() const
{
	return window->GetHandle();
}

void Window_Imp::SetSize(Vector2DI size)
{
	window->SetSize(size.X, size.Y);
}

void Window_Imp::SetCursor(Cursor* cursor)
{
#if ( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )
#else
	auto glfwWindow = (GLFWwindow*)window->GetNativeWindow();

	if (cursor == nullptr)
	{
		glfwSetCursor(glfwWindow, nullptr);
		SafeRelease(currentCursor);
		return;
	}

	auto c = (Cursor_Imp*) cursor;
	glfwSetCursor(glfwWindow, c->GetNative());

	SafeSubstitute(currentCursor, cursor);
#endif
}

const achar* Window_Imp::GetClipboardString()
{
#if ( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )
	return nullptr;
#else
	auto glfwWindow = (GLFWwindow*)window->GetNativeWindow();

	auto s = glfwGetClipboardString(glfwWindow);
	static achar temp[260];

	if (s == nullptr)
	{
		temp[0] = 0;
		return temp;
	}

	std::vector<int16_t> dst;
	int32_t len = Utf8ToUtf16(dst, (const int8_t*)s);

	int32_t ind = 0;
	for (ind = 0; ind < Min(len, 260); ind++)
	{
		temp[ind] = dst[ind];
	}

	if (ind < 260)
	{
		temp[ind] = 0;
	}

	temp[259] = 0;

	return temp;
#endif
}

void Window_Imp::SetClipboardString(const achar* s)
{
#if ( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )
#else
	auto glfwWindow = (GLFWwindow*)window->GetNativeWindow();

	std::vector<int8_t> dst;

	Utf16ToUtf8(dst, (int16_t*)s);

	glfwSetClipboardString(glfwWindow, (const char*)dst.data());
#endif
}

ap::Window* Window_Imp::GetWindow()
{
	return window;
}

}
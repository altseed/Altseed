
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Window_Imp.h"
#include "asd.Cursor_Imp.h"

#include "../Log/asd.Log_Imp.h"

#ifdef _WIN32
#include "asd.Window_Imp_Win.h"
#else
#include "asd.Window_Imp_X11.h"
#endif

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
}

Window_Imp* Window_Imp::Create(int32_t width, int32_t height, const achar* title, Log* logger, ColorSpaceType colorSpaceType, bool isFullScreen)
{
	Window_Imp* ret = nullptr;

	if (logger != nullptr) logger->WriteHeading("ウインドウ");

#ifdef _WIN32
	ret = Window_Imp_Win::Create(width, height, title, logger, colorSpaceType, isFullScreen);
#else
	ret = Window_Imp_X11::Create( width, height, title, logger, colorSpaceType, isFullScreen);
#endif
	if (ret != nullptr)
	{
		if (logger != nullptr) logger->WriteLine("ウインドウ作成成功");
	}
	return ret;
}

void Window_Imp::SetSize(Vector2DI size)
{
	glfwSetWindowSize(m_window, size.X, size.Y);
}

void Window_Imp::SetCursor(Cursor* cursor)
{
	if (cursor == nullptr)
	{
		glfwSetCursor(m_window,nullptr);
		SafeRelease(currentCursor);
		return;
	}

	auto c = (Cursor_Imp*) cursor;
	glfwSetCursor(m_window, c->GetNative());

	SafeSubstitute(currentCursor, cursor);
}

const achar* Window_Imp::GetClipboardString()
{
	auto s = glfwGetClipboardString(m_window);
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
}

void Window_Imp::SetClipboardString(const achar* s)
{
	std::vector<int8_t> dst;

	Utf16ToUtf8(dst, (int16_t*)s);

	glfwSetClipboardString(m_window, (const char*)dst.data());
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
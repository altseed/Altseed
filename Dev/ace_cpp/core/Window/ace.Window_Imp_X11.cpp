
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Window_Imp_X11.h"
#include "../Log/ace.Log_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Window_Imp_X11::Unregist()
{
	if (!m_closed)
	{
		glfwDestroyWindow(m_window);
		m_window = nullptr;
		m_closed = true;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Window_Imp* Window_Imp_X11::Create(int32_t width, int32_t height, const achar* title, Log* logger)
{
	if (!glfwInit())
	{
		if (logger != nullptr) logger->WriteLine("ウインドウシステムの初期化に失敗");
		return nullptr;
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	auto titleUTF8 = ToUtf8String(title);

	auto window = glfwCreateWindow(width, height, titleUTF8.c_str(), NULL, NULL);
	if (window == nullptr)
	{
		if (logger != nullptr) logger->WriteLine("ウインドウの作成に失敗");
		glfwTerminate();
		return nullptr;
	}

	glfwSwapInterval(1);

	return new Window_Imp_X11(window, width, height, logger);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Window_Imp_X11::Window_Imp_X11(GLFWwindow* window, int32_t width, int32_t height, Log* logger)
	: Window_Imp(logger)
	, m_window(window)
	, m_closed(false)
{
	m_size.X = width;
	m_size.Y = height;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Window_Imp_X11::~Window_Imp_X11()
{
	Unregist();
	glfwTerminate();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool Window_Imp_X11::DoEvent()
{
	if (m_closed) return false;

	if (glfwWindowShouldClose(m_window))
	{
		Unregist();
		return false;
	}

	glfwPollEvents();
	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Window_Imp_X11::SetTitle( const achar* title )
{
	auto titleUTF8 = ToUtf8String(title);
	glfwSetWindowTitle(m_window, titleUTF8.c_str());
}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Window_Imp_X11::Close()
{
	glfwSetWindowShouldClose(m_window, 1);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void* Window_Imp_Win::GetWindowHandle() const
{
	return nullptr;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}

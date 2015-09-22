
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Window_Imp.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Window_Imp_X11 : public Window_Imp
{
private:
	bool		m_closed;

	GLFWwindow*	m_window;

	static void CallbackOnFocus(GLFWwindow* window, int b);
	void Unregist();
public:

	static Window_Imp* Create(int32_t width, int32_t height, const achar* title, Log* logger, bool isFullScreen);

	Window_Imp_X11(GLFWwindow* window, int32_t width, int32_t height, Log* logger, bool isFullScreen);
	virtual ~Window_Imp_X11();

	bool DoEvent();

	void SetTitle( const achar* title );

	Vector2DI GetPrimaryMonitorPosition() override;

	Vector2DI GetPrimaryMonitorSize() override;

	Vector2DI GetWindowPosition() override;

	void SetWindowPosition(Vector2DI position) override;

	void Close();

	GLFWwindow*	GetWindow() { return m_window; }

	void* GetWindowHandle() const override;

#if !SWIG
public:
	virtual int GetRef() { return ReferenceObject::GetRef(); }
	virtual int AddRef() { return ReferenceObject::AddRef(); }
	virtual int Release() { return ReferenceObject::Release(); }
#endif
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
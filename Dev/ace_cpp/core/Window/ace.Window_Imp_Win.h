
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Window_Imp.h"

#include <Windows.h>

#ifdef _DEBUG
#pragma comment(lib,"Debug/glfw3.lib")
#else
#pragma comment(lib,"Release/glfw3.lib")
#endif
#pragma comment(lib,"opengl32.lib")

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Window_Imp_Win : public Window_Imp
{
private:
	bool		m_closed;

	GLFWwindow*	m_window;

	void Unregist();

public:

	static Window_Imp* Create(int32_t width, int32_t height, const achar* title);

	Window_Imp_Win( GLFWwindow* window, int32_t width, int32_t height);
	virtual ~Window_Imp_Win();

	bool DoEvent();

	void SetTitle( const achar* title );

	void Close();

	GLFWwindow*	GetWindow() { return m_window; }

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
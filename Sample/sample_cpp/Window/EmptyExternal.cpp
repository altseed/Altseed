
#include <Base.h>

#if _WIN32
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <Windows.h>

static HWND g_handle = NULL;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static void InitWindow()
{
	WNDCLASSW wndClass;
	wchar_t szClassNme [] = L"EmptyExternal";
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = GetModuleHandle(0);
	wndClass.hIcon = NULL;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szClassNme;
	RegisterClassW(&wndClass);
	g_handle = CreateWindowW(
		szClassNme,
		L"EmptyExternal",
		WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		NULL,
		NULL,
		GetModuleHandle(0),
		NULL);
	ShowWindow(g_handle, true);
	UpdateWindow(g_handle);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static bool DoEvent()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		GetMessage(&msg, NULL, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static void ExitWindow()
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#else
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <X11/Xlib.h>
#include <X11/Xutil.h>

static Display*	g_display;
static Window	g_window;
static GC	g_gc;
static bool	g_closed = false;
static Atom	g_wm_delete_window;

static void InitWindow()
{
	g_display = XOpenDisplay(NULL);
	if( g_display == nullptr ) return;

	auto background = WhitePixel(g_display, 0);
	auto foreground = BlackPixel(g_display, 0);

	g_window = XCreateSimpleWindow(
		g_display,
		DefaultRootWindow(g_display),
		0, 0, 640, 480,
		0, 0, background);

	/* タイトルの設定を行う。 */

	char* title_ptr = "WindowExternal";

	XTextProperty title_prop;
	setlocale(LC_ALL,"");
	XmbTextListToTextProperty( g_display, &(title_ptr), 1, XCompoundTextStyle, &title_prop );
	XSetWMName(g_display,g_window,&title_prop);

	XmbTextListToTextProperty(g_display, &(title_ptr), 1, XCompoundTextStyle, &title_prop );
	XSetWMIconName(g_display,g_window,&title_prop);

	g_gc = XCreateGC(g_display, g_window, 0, 0);

	XSetBackground(g_display, g_gc, background);
	XSetForeground(g_display, g_gc, foreground);

	XMapRaised(g_display, g_window);

	XSelectInput( g_display, g_window, StructureNotifyMask );

	g_wm_delete_window = XInternAtom( g_display, "WM_DELETE_WINDOW", False );
	XSetWMProtocols( g_display, g_window, &g_wm_delete_window, 1 );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static bool DoEvent()
{
	if( g_closed ) return false;

	while ( XPending( g_display ) )
	{
		XEvent event;
		XNextEvent( g_display, &event );

		switch ( event.type )
		{
			case ClientMessage :
			{
				if ( (Atom)event.xclient.data.l[0] == g_wm_delete_window )
				return false;
			}
			break;

			case (DestroyNotify) :
			{
				g_closed = true;
				return false;
			}
			break;
		}
	}

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static void ExitWindow()
{
	if( !g_closed )
	{
		XDestroyWindow( g_display, g_window );
		g_closed = true;
	}

	XCloseDisplay( g_display );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	外部のウインドウにaceの機能で描画を行う。
*/
void EmptyExternal()
{
	InitWindow();

	// aceを初期化する。
#if _WIN32
	asd::Engine::InitializeByExternalWindow(g_handle, nullptr, 640, 480, asd::EngineOption());
#else
	asd::Engine::InitializeByExternalWindow((void*)(g_display), (void*)(&g_window), 640, 480, asd::EngineOption());
#endif

	// aceが進行可能かチェックする。 
	while (asd::Engine::DoEvents())
	{
		if (!DoEvent()) break;

		// aceを更新する。
		asd::Engine::Update();
	}

	// aceを終了する。
	asd::Engine::Terminate();

	ExitWindow();
}

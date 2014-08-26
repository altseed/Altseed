
#include "ace.common.Base.h"

#if !_WIN32
namespace ace_x11
{
	// http://davescode.ohost.de/code/MessageBoxX11.c

	//
	/*
	I, David Oberhollenzer, author of this file hereby place the contents of
	this file into the public domain. Please feel free to use this file in any
	way you wish.
	I want to do this, because a lot of people are in the need of a simple X11
	message box function.
	The original version was written in C++ and has been ported to C. This
	version is entirely leak proof! (According to valgrind)
	*/

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <string.h>
#include <stdlib.h>




	/**************************************************************************
	* A "small" and "simple" function that creates a message box with an OK  *
	* button, using ONLY Xlib.                                               *
	* The function does not return until the user closes the message box,    *
	* using the OK button, the escape key, or the close button what means    *
	* that you can't do anything in the mean time(in the same thread).       *
	* The code may look very ugly, because I pieced it together from         *
	* tutorials and manuals and I use an awfull lot of magic values and      *
	* unexplained calculations.                                              *
	*                                                                        *
	* title: The title of the message box.                                   *
	* text:  The contents of the message box. Use '\n' as a line terminator. *
	**************************************************************************/
	void MessageBoxX11(const char* title, const char* text)
	{
		const char* wmDeleteWindow = "WM_DELETE_WINDOW";
		int black, white, length, height, direction, ascent, descent, X, Y, W, H,
			okX1, okY1, okX2, okY2, okBaseX, okBaseY, okWidth, okHeight, run,
			buttonFocus, offset;
		size_t i, strvec_size;
		char **strvec, *atom, *pch, *temp;
		Display* dpy;
		Window w;
		Atom wmDelete;
		GC gc;
		XFontStruct* font;
		XCharStruct overall;
		XSizeHints* hints;
		XEvent e;

		/* Open a display */
		if (!(dpy = XOpenDisplay(0)))
			return;

		/* Get us a white and black color */
		black = BlackPixel(dpy, DefaultScreen(dpy));
		white = WhitePixel(dpy, DefaultScreen(dpy));

		/* Create a window with the specified title */
		w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 100, 100,
			0, black, black);

		XSelectInput(dpy, w, ExposureMask | StructureNotifyMask |
			KeyReleaseMask | PointerMotionMask |
			ButtonPressMask | ButtonReleaseMask);

		XMapWindow(dpy, w);
		XStoreName(dpy, w, title);

		wmDelete = XInternAtom(dpy, wmDeleteWindow, True);
		XSetWMProtocols(dpy, w, &wmDelete, 1);

		/* Create a graphics context for the window */
		gc = XCreateGC(dpy, w, 0, 0);

		XSetForeground(dpy, gc, white);
		XSetBackground(dpy, gc, black);

		/* Split the text down into a list of lines */
		strvec = NULL;
		strvec_size = 0;

		temp = (char*)malloc(strlen(text) + 1);
		strcpy(temp, text);

		pch = strtok(temp, "\n");

		while (pch != NULL)
		{
			strvec = (char**)realloc(strvec, (strvec_size + 1)*sizeof(char**));

			strvec[strvec_size] = (char*)malloc(strlen(pch) + 1);
			strcpy(strvec[strvec_size], pch);

			++strvec_size;

			pch = strtok(NULL, "\n");
		}

		free(temp);

		/* Compute the printed length and height of the longest and the tallest line */
		if (!(font = XQueryFont(dpy, XGContextFromGC(gc))))
			return;

		for (i = 0, length = 0, height = 0; i<strvec_size; ++i)
		{
			XTextExtents(font, strvec[i], strlen(strvec[i]), &direction, &ascent, &descent, &overall);

			length = overall.width  >length ? overall.width : length;
			height = (ascent + descent)>height ? (ascent + descent) : height;
		}

		/* Compute the shape of the window, needed to display the text and adjust the window accordingly */
		X = DisplayWidth(dpy, DefaultScreen(dpy)) / 2 - length / 2 - 10;
		Y = DisplayHeight(dpy, DefaultScreen(dpy)) / 2 - strvec_size*height / 2 - height - 10;
		W = length + 20;
		H = strvec_size*height + height + 40;

		XMoveResizeWindow(dpy, w, X, Y, W, H);

		/* Compute the shape of the OK button */
		XTextExtents(font, "OK", 2, &direction, &ascent, &descent, &overall);

		okWidth = overall.width;
		okHeight = ascent + descent;

		okX1 = W / 2 - okWidth / 2 - 15;
		okY1 = (strvec_size*height + 20) + 5;
		okX2 = W / 2 + okWidth / 2 + 15;
		okY2 = okY1 + 4 + okHeight;
		okBaseX = okX1 + 15;
		okBaseY = okY1 + 2 + okHeight;

		XFreeFontInfo(NULL, font, 1); /* We don't need that anymore */

		/* Make the window non resizeable */
		XUnmapWindow(dpy, w);
		hints = XAllocSizeHints();

		hints->flags = PSize | PMinSize | PMaxSize;
		hints->min_width = hints->max_width = hints->base_width = W;
		hints->min_height = hints->max_height = hints->base_height = H;

		XSetWMNormalHints(dpy, w, hints);
		XFree(hints);

		XMapRaised(dpy, w);
		XFlush(dpy);

		/* Event loop */
		run = 1;
		buttonFocus = 0;

		do
		{
			XNextEvent(dpy, &e);
			offset = 0;

			if (e.type == MotionNotify)
			{
				if (e.xmotion.x >= okX1 && e.xmotion.x <= okX2 && e.xmotion.y >= okY1 && e.xmotion.y <= okY2)
				{
					if (!buttonFocus)
						e.type = Expose;

					buttonFocus = 1;
				}
				else
				{
					if (buttonFocus)
						e.type = Expose;

					buttonFocus = 0;
					offset = 0;
				}
			}

			switch (e.type)
			{
			case ButtonPress:
			case ButtonRelease:
				if (e.xbutton.button != Button1)
					break;

				if (buttonFocus)
				{
					offset = e.type == ButtonPress ? 1 : 0;

					if (!offset)
						run = 0;
				}
				else
				{
					offset = 0;
				}

			case Expose:
			case MapNotify:
				XClearWindow(dpy, w);

				/* Draw text lines */
				for (i = 0; i<strvec_size; ++i)
					XDrawString(dpy, w, gc, 10, 10 + height + height*i, strvec[i], strlen(strvec[i]));

				/* Draw OK button */
				if (buttonFocus)
				{
					XFillRectangle(dpy, w, gc, offset + okX1, offset + okY1, okX2 - okX1, okY2 - okY1);
					XSetForeground(dpy, gc, black);
				}
				else
				{
					XDrawLine(dpy, w, gc, okX1, okY1, okX2, okY1);
					XDrawLine(dpy, w, gc, okX1, okY2, okX2, okY2);
					XDrawLine(dpy, w, gc, okX1, okY1, okX1, okY2);
					XDrawLine(dpy, w, gc, okX2, okY1, okX2, okY2);
				}

				XDrawString(dpy, w, gc, offset + okBaseX, offset + okBaseY, "OK", 2);

				if (buttonFocus)
				{
					XSetForeground(dpy, gc, white);
				}

				XFlush(dpy);
				break;

			case KeyRelease:
				if (XLookupKeysym(&e.xkey, 0) == XK_Escape)
					run = 0;
				break;

			case ClientMessage:
				atom = XGetAtomName(dpy, e.xclient.message_type);

				if (*atom == *wmDeleteWindow)
					run = 0;

				XFree(atom);

				break;
			};
		} while (run);

		/* Clean up */
		for (i = 0; i<strvec_size; ++i)
			free(strvec[i]);

		free(strvec);

		XFreeGC(dpy, gc);
		XDestroyWindow(dpy, w);
		XCloseDisplay(dpy);
	}
}
#endif

namespace ace
{
	int32_t Utf16ToUtf8(std::vector<int8_t> &dst, const int16_t* src)
	{
		const int16_t* wp = src;
		dst.resize(0);

		for (;;)
		{
			int16_t wc = *wp++;
			if (wc == 0)
			{
				break;
			}
			if ((wc & ~0x7f) == 0)
			{
				dst.push_back(wc & 0x7f);
			}
			else if ((wc & ~0x7ff) == 0)
			{
				dst.push_back(((wc >> 6) & 0x1f) | 0xc0);
				dst.push_back(((wc) & 0x3f) | 0x80);
			}
			else
			{
				dst.push_back(((wc >> 12) & 0xf) | 0xe0);
				dst.push_back(((wc >> 6) & 0x3f) | 0x80);
				dst.push_back(((wc) & 0x3f) | 0x80);
			}
		}
		dst.push_back('\0');
		return static_cast<int32_t>(dst.size()) - 1;
	}

	int32_t Utf8ToUtf16(std::vector<int16_t> &dst, const int8_t* src)
	{
		int32_t i, code;
		int8_t c0, c1, c2;
		dst.resize(0);

		for (;;)
		{
			int16_t wc;

			c0 = *src++;
			if (c0 == '\0')
			{
				break;
			}
			// UTF8からUTF16に変換
			code = (uint8_t) c0 >> 4;
			if (code <= 7)
			{
				// 8bit文字
				wc = c0;
			}
			else if (code >= 12 && code <= 13)
			{
				// 16bit文字
				c1 = *src++;
				wc = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
			}
			else if (code == 14)
			{
				// 24bit文字
				c1 = *src++;
				c2 = *src++;
				wc = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
			}
			else
			{
				continue;
			}
			dst.push_back(wc);
		}
		dst.push_back(0);
		return static_cast<int32_t>(dst.size()) - 1;
	}

	std::wstring ToWide(const char* pText)
	{
#if _WIN32
		int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

		wchar_t* pOut = new wchar_t[Len + 1];
		::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
		std::wstring Out(pOut);
		delete[] pOut;
		return Out;
#else
		std::vector<int16_t> result;
		result.reserve(4096);
		std::wstring temp;

		Utf8ToUtf16(result, (int8_t*) pText);

		if (sizeof(wchar_t) == 2)
		{
			temp = std::wstring((const wchar_t*) result.data());
		}
		else if (sizeof(wchar_t) == 4)
		{
			std::vector<wchar_t> buf(result.size());
			std::copy(result.begin(), result.end(), buf.begin());
			temp = std::wstring(buf.data());
		}

		return temp;
#endif
	}

	astring ToAString(const wchar_t* src)
	{
		if (sizeof(wchar_t) == 2)
		{
#ifdef _WIN32
			return astring(src);
#else
			return astring(reinterpret_cast<const achar*>(src));
#endif
		}
		if (sizeof(wchar_t) == 4)
		{
#ifndef _WIN32
			std::vector<achar> temp;
			temp.reserve(2048);
			int32_t length = 0;
			while (src[length] != 0)
			{
				temp.push_back(static_cast<achar>(src[length]));
				length++;
			}
			temp.push_back(0);
			return astring(temp.data());
#endif
		}
		return astring();
	}

	astring ToAString(const char* src)
	{
		return ToAString(ToWide(src).c_str());
	}

	std::string ToUtf8String(const achar* src)
	{
		std::vector<int8_t> result;
		result.reserve(4096);
		Utf16ToUtf8(result, (int16_t*) src);

		std::string temp((const char*) result.data());
		return temp;
	}

	astring ReplaceAll(const astring text, const achar* from, const achar* to)
	{
		astring result = text;
		astring endStr = astring(to);
		astring::size_type pos = 0;
		while (true)
		{
			pos = result.find(from, pos + 1);
			if (pos != astring::npos)
			{
				result = result.replace(pos, endStr.length(), endStr);
			}
			else
			{
				break;
			}
		}

		return result;
	}

	astring CombinePath(const achar* rootPath, const achar* path)
	{
		const int32_t dstLength = 260;
		achar dst[dstLength];

		int rootPathLength = 0;
		while (rootPath[rootPathLength] != 0)
		{
			rootPathLength++;
		}

		int pathLength = 0;
		while (path[pathLength] != 0)
		{
			pathLength++;
		}

		// 両方ともなし
		if (rootPathLength == 0 && pathLength == 0)
		{
			return astring();
		}

		// 片方なし
		if (rootPathLength == 0)
		{
			if (pathLength < dstLength)
			{
				memcpy(dst, path, sizeof(achar) * (pathLength + 1));
				return astring(dst);
			}
			else
			{
				return astring();
			}
		}

		if (pathLength == 0)
		{
			if (rootPathLength < dstLength)
			{
				memcpy(dst, rootPath, sizeof(wchar_t) * (rootPathLength + 1));
				return astring(dst);
			}
			else
			{
				return astring();
			}
		}

		// 両方あり

		// ディレクトリパスまで戻す。
		int PathPosition = rootPathLength;
		while (PathPosition > 0)
		{
			if (rootPath[PathPosition - 1] == L'/' || rootPath[PathPosition - 1] == L'\\')
			{
				break;
			}
			PathPosition--;
		}

		// コピーする
		memcpy(dst, rootPath, sizeof(achar) * PathPosition);
		dst[PathPosition] = 0;

		// 無理やり繋げる
		if (PathPosition + pathLength > dstLength)
		{
			return astring();
		}

		memcpy(&(dst[PathPosition]), path, sizeof(achar) * pathLength);
		PathPosition = PathPosition + pathLength;
		dst[PathPosition] = 0;

		// ../ ..\ の処理
		for (int i = 0; i < PathPosition - 2; i++)
		{
			if (dst[i] == L'.' && dst[i + 1] == L'.' && (dst[i + 2] == L'/' || dst[i + 2] == L'\\'))
			{
				int pos = 0;

				if (i > 1 && dst[i - 2] == L'.')
				{

				}
				else
				{
					for (pos = i - 2; pos > 0; pos--)
					{
						if (dst[pos - 1] == L'/' || dst[pos - 1] == L'\\')
						{
							break;
						}
					}

					for (int k = pos; k < PathPosition; k++)
					{
						dst[k] = dst[k + (i + 3) - pos];
					}
					PathPosition = PathPosition - (i + 3 - pos);
					i = pos - 1;
				}
			}
		}
		dst[PathPosition] = 0;
		return astring(dst);
	}

	void ShowMessageBox(const achar* title, const achar* text)
	{
#if _WIN32
		::MessageBoxW(NULL, text, title, MB_OK);
#else
		auto title_ = ToUtf8String(title);
		auto text_ = ToUtf8String(text);

		ace_x11::MessageBoxX11(title_.c_str(), text_.c_str());
#endif
	}
}

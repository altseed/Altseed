
#pragma once

#include <asd.common.Base.h>

#include <Math/asd.Vector2DF.h>
#include <Math/asd.Vector3DF.h>
#include <Math/asd.Vector4DF.h>
#include <Graphics/asd.Color.h>

// GLEW
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#if _WIN32

#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/libglew32d.lib")
#else
#pragma comment(lib,"x64/Release/libglew32.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/libglew32d.lib")
#else
#pragma comment(lib,"x86/Release/libglew32.lib")
#endif

#endif

#endif


// GLU
#pragma comment(lib, "glu32.lib")

// GLFW
#if _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#else
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include <X11/extensions/Xrandr.h>
#endif

#include <GLFW/glfw3.h>

#if _WIN32
#include <GLFW/glfw3native.h>
#endif

// DirectX
#if _WIN32

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib" )
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#include <D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib" )

#endif

// Effekseer
#include <Effekseer.h>

#if _WIN32
#include <EffekseerRendererDX11.h>

#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/Effekseer.lib")
#pragma comment(lib,"x64/Debug/EffekseerRendererDX11.lib")
#pragma comment(lib,"x64/Debug/EffekseerRendererGL.lib")
#else
#pragma comment(lib,"x64/Release/Effekseer.lib")
#pragma comment(lib,"x64/Release/EffekseerRendererDX11.lib")
#pragma comment(lib,"x64/Release/EffekseerRendererGL.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/Effekseer.lib")
#pragma comment(lib,"x86/Debug/EffekseerRendererDX11.lib")
#pragma comment(lib,"x86/Debug/EffekseerRendererGL.lib")
#else
#pragma comment(lib,"x86/Release/Effekseer.lib")
#pragma comment(lib,"x86/Release/EffekseerRendererDX11.lib")
#pragma comment(lib,"x86/Release/EffekseerRendererGL.lib")
#endif

#endif

#endif
#include <EffekseerRendererGL.h>


#if _WIN32

#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/freetype.lib")
#else
#pragma comment(lib,"x64/Release/freetype.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/freetype.lib")
#else
#pragma comment(lib,"x86/Release/freetype.lib")
#endif

#endif

#endif

// Altseed
#if _WIN32

#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/asd_common.lib")
#pragma comment(lib,"x64/Debug/asd_tool.lib")
#else
#pragma comment(lib,"x64/Release/asd_common.lib")
#pragma comment(lib,"x64/Release/asd_tool.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/asd_common.lib")
#pragma comment(lib,"x86/Debug/asd_tool.lib")
#else
#pragma comment(lib,"x86/Release/asd_common.lib")
#pragma comment(lib,"x86/Release/asd_tool.lib")
#endif

#endif

#endif


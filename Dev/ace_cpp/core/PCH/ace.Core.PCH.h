
#pragma once

#include <ace.common.Base.h>

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Graphics/ace.Color.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#if _WIN32
#if _DEBUG
#pragma comment(lib,"glew32sd.lib")
#else
#pragma comment(lib,"glew32s.lib")
#endif
#endif

// GLU
#pragma comment(lib, "glu32.lib")

// GLFW
#if _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#else
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif

#include <GLFW/glfw3.h>

#if _WIN32
#include <GLFW/glfw3native.h>
#endif

// DirectX
#if _WIN32

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib" )

#include <D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib" )

#endif

// Effekseer
#include <Effekseer.h>

#if _WIN32
#include <EffekseerRendererDX11.h>

#if _DEBUG
#pragma comment(lib,"Debug/Effekseer.lib")
#pragma comment(lib,"Debug/EffekseerRendererDX11.lib")
#pragma comment(lib,"Debug/EffekseerRendererGL.lib")
#else
#pragma comment(lib,"Release/Effekseer.lib")
#pragma comment(lib,"Release/EffekseerRendererDX11.lib")
#pragma comment(lib,"Release/EffekseerRendererGL.lib")
#endif

#endif
#include <EffekseerRendererGL.h>

// ACE
#if _WIN32
#if _DEBUG
#pragma comment(lib,"Debug/ace_common.lib")
#pragma comment(lib,"Debug/ace_tool.lib")
#else
#pragma comment(lib,"Release/ace_common.lib")
#pragma comment(lib,"Release/ace_tool.lib")
#endif
#endif


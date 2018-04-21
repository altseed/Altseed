#include "asd.Tool.h"

#include "../3rdParty/imgui/imgui.h"
#include "../3rdParty/imgui/imgui_impl_glfw.h"

#if _WIN32
#include "../3rdParty/imgui/imgui_impl_dx11.h"
#endif

#include "../3rdParty/imgui/imgui_impl_opengl3.h"

#include "../Graphics/asd.Graphics_Imp.h"
#include "../Window/asd.Window_Imp.h"

#include <codecvt>
#include <locale>
#include <functional>

namespace asd
{
	// http://hasenpfote36.blogspot.jp/2016/09/stdcodecvt.html
	static constexpr std::codecvt_mode mode = std::codecvt_mode::little_endian;

	static std::string utf16_to_utf8(const std::u16string& s)
	{
#if defined(_MSC_VER)
		std::wstring_convert<std::codecvt_utf8_utf16<std::uint16_t, 0x10ffff, mode>, std::uint16_t> conv;
		auto p = reinterpret_cast<const std::uint16_t*>(s.c_str());
		return conv.to_bytes(p, p + s.length());
#else
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, mode>, char16_t> conv;
		return conv.to_bytes(s);
#endif
	}

	Tool::Tool(Window* window, Graphics* graphics)
		: window(window)
		, graphics(graphics)
	{
	}

	Tool::~Tool()
	{

	}

	void Tool::Initialize()
	{
		auto w = (Window_Imp*)window;
		auto g = (Graphics_Imp*)graphics;

		auto gw = (GLFWwindow*)w->GetWindow()->GetNativeWindow();
		
		ImGui::CreateContext();

		ImGui_ImplGlfw_Init(gw, false);

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
#if _WIN32
			auto rhi = g->GetRHI();
			auto internalObjs = rhi->GetInternalObjects();
			ImGui_ImplDX11_Init(w->GetWindowHandle(), (ID3D11Device*)internalObjs[0], (ID3D11DeviceContext*)internalObjs[1]);
#endif
		}
		else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			ImGui_ImplGlfwGL3_Init(gw, false);
		}
	}

	void Tool::Terminate()
	{
		auto g = (Graphics_Imp*)graphics;
		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
#if _WIN32
			ImGui_ImplDX11_Shutdown();
#endif
		}
		else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			ImGui_ImplGlfwGL3_Shutdown();
		}

		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Tool::NewFrame()
	{
		auto g = (Graphics_Imp*)graphics;
		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
#if _WIN32
			ImGui_ImplDX11_NewFrame();
#endif
		}
		else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			ImGui_ImplGlfwGL3_NewFrame();
		}

		ImGui_ImplGlfw_NewFrame();
	}

	void Tool::Render()
	{
		ImGui::Render();

		auto g = (Graphics_Imp*)graphics;
		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
#if _WIN32
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
		}
		else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	bool Tool::Begin(const char16_t* name)
	{
		return ImGui::Begin(utf16_to_utf8(name).c_str());
	}

	void Tool::End()
	{
		ImGui::End();
	}

	void Tool::Text(const char16_t* text)
	{
		ImGui::Text(utf16_to_utf8(text).c_str());
	}

	bool Tool::Button(const char16_t* label)
	{
		return ImGui::Button(utf16_to_utf8(label).c_str());
	}
}
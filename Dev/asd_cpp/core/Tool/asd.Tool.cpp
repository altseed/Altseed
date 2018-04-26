#include "asd.Tool.h"

#include "../3rdParty/imgui/imgui.h"
#include "../3rdParty/imgui/imgui_impl_glfw.h"

#if _WIN32
#include "../3rdParty/imgui/imgui_impl_dx11.h"
#endif

#include "../3rdParty/imgui/imgui_impl_opengl3.h"

#include "../Graphics/asd.Graphics_Imp.h"
#include "../Graphics/Platform/DX11/Resource/asd.Texture2D_Imp_DX11.h"

#include "../Window/asd.Window_Imp.h"

#include "../3rdParty/nfd/nfd.h"

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

	static std::u16string utf8_to_utf16(const std::string& s)
	{

#if defined(_MSC_VER)
		std::wstring_convert<std::codecvt_utf8_utf16<std::uint16_t, 0x10ffff, mode>, std::uint16_t> conv;
		auto p = reinterpret_cast<const std::uint16_t*>(s.c_str());
		return std::u16string((const char16_t*)conv.from_bytes(s).c_str());
#else
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, mode>, char16_t> conv;
		return conv.from_bytes(s);
#endif
	}

	/**
	@brief	文字列分割
	@note
	http://shnya.jp/blog/?p=195 のコードを改造
	*/
	static std::vector<astring> split(const astring &str, const astring &delim)
	{
		std::vector<astring> res;
		size_t current = 0, found, delimlen = delim.size();
		while ((found = str.find(delim, current)) != astring::npos)
		{
			res.push_back(astring(str, current, found - current));
			current = found + delimlen;
		}
		res.push_back(astring(str, current, str.size() - current));
		return res;
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

		ImGui_ImplGlfw_Init(gw, true);

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
		auto w = (Window_Imp*)window;
		auto g = (Graphics_Imp*)graphics;

		auto gw = (GLFWwindow*)w->GetWindow()->GetNativeWindow();

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

		ImGui_ImplGlfw_Shutdown(gw);
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

	void Tool::Image(Texture2D* user_texture, const Vector2DF& size)
	{
		if (user_texture == nullptr) return;
		auto texture = (Texture2D_Imp_DX11*)user_texture;

		auto g = (Graphics_Imp*)graphics;
		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
			auto o = texture->GetRHI()->GetInternalObjects();
			ImGui::Image((ImTextureID)o[1], ImVec2(size.X, size.Y));
		}
		else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			auto o = texture->GetRHI()->GetInternalObjects();
			ImGui::Image((ImTextureID)o[0], ImVec2(size.X, size.Y), ImVec2(0, 1), ImVec2(0, 0));
		}
	}

	bool Tool::BeginCombo(const char16_t* label, const char16_t* preview_value)
	{
		return ImGui::BeginCombo(utf16_to_utf8(label).c_str(), utf16_to_utf8(preview_value).c_str());
	}

	void Tool::EndCombo()
	{
		ImGui::EndCombo();
	}

	bool Tool::InputText(const char16_t* label, int8_t* buf, int32_t buf_size)
	{
		return ImGui::InputText(utf16_to_utf8(label).c_str(), (char*)buf, buf_size);
	}

	bool Tool::InputInt(const char16_t* label, int* v)
	{
		return ImGui::InputInt(utf16_to_utf8(label).c_str(), v);
	}

	bool Tool::ColorEdit4(const char16_t* label, float* vs)
	{
		return ImGui::ColorEdit4(utf16_to_utf8(label).c_str(), vs);
	}

	bool Tool::Selectable(const char16_t* label, bool selected)
	{
		return ImGui::Selectable(utf16_to_utf8(label).c_str(), selected);
	}

	bool Tool::ListBox(const char16_t* label, int* current_item, const char16_t* items)
	{
		auto items_ = split(items, astring(u";"));
		std::vector<std::string> strs;
		strs.resize(items_.size());

		std::vector<char*> cs;

		for (int i = 0; i < items_.size(); i++)
		{
			strs[i] = utf16_to_utf8(items_[i]);
			cs.push_back((char*)strs[i].c_str());
		}

		return ImGui::ListBox(utf16_to_utf8(label).c_str(), current_item, cs.data(), cs.size());
	}

	void Tool::SetItemDefaultFocus()
	{
		ImGui::SetItemDefaultFocus();
	}

	static std::u16string temp;

	const char16_t* Tool::OpenDialog(const char16_t* filterList, const char16_t* defaultPath)
	{
		auto filterList_ = utf16_to_utf8(filterList);
		auto defaultPath_ = utf16_to_utf8(defaultPath);

		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog(filterList_.c_str(), defaultPath_.c_str(), &outPath);

		if (result == NFD_OKAY)
		{
			temp = utf8_to_utf16(outPath);
			free(outPath);
			return temp.c_str();
		}
		else if (result == NFD_CANCEL)
		{
			temp = u"";
			return temp.c_str();
		}

		temp = u"";
		return temp.c_str();
	}

	const char16_t* Tool::SaveDialog(const char16_t* filterList, const char16_t* defaultPath)
	{
		auto filterList_ = utf16_to_utf8(filterList);
		auto defaultPath_ = utf16_to_utf8(defaultPath);

		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_SaveDialog(filterList_.c_str(), defaultPath_.c_str(), &outPath);

		if (result == NFD_OKAY)
		{
			temp = utf8_to_utf16(outPath);
			free(outPath);
			return temp.c_str();
		}
		else if (result == NFD_CANCEL)
		{
			temp = u"";
			return temp.c_str();
		}

		temp = u"";
		return temp.c_str();
	}
}
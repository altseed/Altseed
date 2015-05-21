
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Texture2D_Imp.h"
#include "../ace.Graphics_Imp.h"

#define Z_SOLO
#include <png.h>
#include <pngstruct.h>
#include <pnginfo.h>

#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/libpng16.lib")
#pragma comment(lib,"x64/Debug/zlib.lib")
#else
#pragma comment(lib,"x64/Release/libpng16.lib")
#pragma comment(lib,"x64/Release/zlib.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/libpng16.lib")
#pragma comment(lib,"x86/Debug/zlib.lib")
#else
#pragma comment(lib,"x86/Release/libpng16.lib")
#pragma comment(lib,"x86/Release/zlib.lib")
#endif

#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	TextureLockInfomation::TextureLockInfomation()
	{
	}

	void* TextureLockInfomation::GetPixels() const
	{
		return pixels;
	}

	int32_t TextureLockInfomation::GetPitch() const
	{
		return pitch;
	}

	Vector2DI TextureLockInfomation::GetSize() const
	{
		return size;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Texture2D_Imp::InternalLoad(void* data, int32_t size, bool rev)
	{
		InternalUnload();

		void* imagedata = nullptr;
		auto g = (Graphics_Imp*) GetGraphics();
		if (ImageHelper::LoadPNGImage(data, size, rev, m_internalTextureWidth, m_internalTextureHeight, m_internalTextureData, g->GetLog()))
		{
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Texture2D_Imp::InternalUnload()
	{
		m_internalTextureWidth = 0;
		m_internalTextureHeight = 0;
		std::vector<uint8_t>().swap(m_internalTextureData);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp::Texture2D_Imp(Graphics* graphics)
		: DeviceObject(graphics)
		, m_internalTextureWidth(0)
		, m_internalTextureHeight(0)
	{
		m_type = TextureClassType::Texture2D;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp::~Texture2D_Imp()
	{
		auto g = (Graphics_Imp*)GetGraphics();
		g->Texture2DContainer->Unregister(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
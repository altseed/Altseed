
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Texture2D_Imp.h"
#include "../asd.Graphics_Imp.h"

#define Z_SOLO
#include <png.h>
#include <pngstruct.h>
#include <pnginfo.h>

/*
#ifdef _v120
#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/VS2013/libpng16.lib")
#pragma comment(lib,"x64/Debug/VS2013/zlib.lib")
#else
#pragma comment(lib,"x64/Release/VS2013/libpng16.lib")
#pragma comment(lib,"x64/Release/VS2013/zlib.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/VS2013/libpng16.lib")
#pragma comment(lib,"x86/Debug/VS2013/zlib.lib")
#else
#pragma comment(lib,"x86/Release/VS2013/libpng16.lib")
#pragma comment(lib,"x86/Release/VS2013/zlib.lib")
#endif

#endif
#endif
*/

//#ifdef _v140
#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/VS2015/libpng16.lib")
#pragma comment(lib,"x64/Debug/VS2015/zlib.lib")
#else
#pragma comment(lib,"x64/Release/VS2015/libpng16.lib")
#pragma comment(lib,"x64/Release/VS2015/zlib.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/VS2015/libpng16.lib")
#pragma comment(lib,"x86/Debug/VS2015/zlib.lib")
#else
#pragma comment(lib,"x86/Release/VS2015/libpng16.lib")
#pragma comment(lib,"x86/Release/VS2015/zlib.lib")
#endif

#endif
//#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
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
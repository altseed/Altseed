
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Texture2D_Imp.h"
#include "../ace.Graphics_Imp.h"
#include "../ace.GraphicsResourceContainer.h"

#define Z_SOLO
#include <png.h>
#include <pngstruct.h>
#include <pnginfo.h>

#if _WIN32
#if _DEBUG
#pragma comment(lib,"libpng16.Debug.lib")
#pragma comment(lib,"zlib.Debug.lib")
#else
#pragma comment(lib,"libpng16.Release.lib")
#pragma comment(lib,"zlib.Release.lib")
#endif
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Texture2D_Imp::InternalLoad(void* data, int32_t size, bool rev)
	{
		InternalUnload();

		void* imagedata = nullptr;
		if (ImageHelper::LoadPNGImage(data, size, rev, m_internalTextureWidth, m_internalTextureHeight, m_internalTextureData))
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
		m_type = TEXTURE_CLASS_TEXTURE2D;
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
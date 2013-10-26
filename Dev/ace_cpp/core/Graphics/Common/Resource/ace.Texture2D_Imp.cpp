
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
	static void PngReadData(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		auto d = (uint8_t**) png_get_io_ptr(png_ptr);

		memcpy(data, *d, length);
		(*d) += length;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Texture2D_Imp::InternalLoad(void* data, int32_t size, bool rev)
	{
		InternalUnload();

		uint8_t* data_ = (uint8_t*) data;

		/* pngアクセス構造体を作成 */
		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		/* リードコールバック関数指定 */
		png_set_read_fn(png, &data_, &PngReadData);

		/* png画像情報構造体を作成 */
		png_infop png_info = png_create_info_struct(png);

		/* IHDRチャンク情報を取得 */
		png_read_info(png, png_info);

		/* RGBA8888フォーマットに変換する */
		if (png_info->bit_depth < 8)
		{
			png_set_packing(png);
		}
		else if (png_info->bit_depth == 16)
		{
			png_set_strip_16(png);
		}


		uint32_t pixelBytes = 4;
		switch (png_info->color_type)
		{
		case PNG_COLOR_TYPE_PALETTE:
			png_set_palette_to_rgb(png);
			pixelBytes = 3;
			break;
		case PNG_COLOR_TYPE_GRAY:
			png_set_expand_gray_1_2_4_to_8(png);
			pixelBytes = 3;
			break;
		case PNG_COLOR_TYPE_RGB:
			pixelBytes = 3;
			break;
		case PNG_COLOR_TYPE_RGBA:
			break;
		}

		uint8_t* image = new uint8_t[png_info->width * png_info->height * pixelBytes];
		uint32_t pitch = png_info->width * pixelBytes;

		// 読み込み
		if (rev)
		{
			for (uint32_t i = 0; i < png_info->height; i++)
			{
				png_read_row(png, &image[(png_info->height - 1 - i) * pitch], NULL);
			}
		}
		else
		{
			for (uint32_t i = 0; i < png_info->height; i++)
			{
				png_read_row(png, &image[i * pitch], NULL);
			}
		}

		m_internalTextureWidth = png_info->width;
		m_internalTextureHeight = png_info->height;
		m_internalTextureData = new uint8_t[m_internalTextureWidth * m_internalTextureHeight * 4];

		if (pixelBytes == 4)
		{
			memcpy(m_internalTextureData, image, m_internalTextureWidth * m_internalTextureHeight * 4);
		}
		else
		{
			for (int32_t y = 0; y < m_internalTextureHeight; y++)
			{
				for (int32_t x = 0; x < m_internalTextureWidth; x++)
				{
					auto src = (x + y * m_internalTextureHeight) * 3;
					auto dst = (x + y * m_internalTextureHeight) * 4;
					m_internalTextureData[dst + 0] = image[src + 0];
					m_internalTextureData[dst + 1] = image[src + 1];
					m_internalTextureData[dst + 2] = image[src + 2];
					m_internalTextureData[dst + 3] = 255;
				}
			}
		}

		delete [] image;
		png_destroy_read_struct(&png, &png_info, NULL);

		return true;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Texture2D_Imp::InternalUnload()
	{
		m_internalTextureWidth = 0;
		m_internalTextureHeight = 0;
		SafeDeleteArray(m_internalTextureData);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D_Imp::Texture2D_Imp(Graphics* graphics)
		: DeviceObject(graphics)
		, m_internalTextureData(nullptr)
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
		g->GetResourceContainer()->UnregistTexture2D(this);

		SafeDeleteArray(m_internalTextureData);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
#pragma once
#include <array>
#include <vector>
#include <png.h>
#include <cstdint>
#include <iostream>

#pragma warning(disable:4996)

namespace FontGenerator
{
	template<typename T>
	std::vector<char> GetBytesOf(T& data)
	{
		std::vector<char> result;
		char* buf = reinterpret_cast<char*>(&data);
		int size = sizeof(T);
		for (int i = 0; i < size; ++i)
		{
			result.push_back(buf[i]);
		}
		return result;
	}

	static void AddRange(std::vector<char>& dst, std::vector<char>& src)
	{
		dst.insert(dst.end(), src.begin(), src.end());
	}

	/**
	@brief	PNGファイルを保存する。
	@param	filepath	保存先
	@param	width	横幅
	@param	height	縦幅
	@param	data	隙間なく敷き詰められた画素データ(1画素4byte固定)
	@param	rev		上下反転で保存する。
	*/
	static void SavePNGImage(const asd::achar* filepath, int32_t width, int32_t height, void* data, bool rev)
	{
		png_bytep raw1D;
		png_bytepp raw2D;

		/* 構造体確保 */
#if _WIN32
		FILE *fp = _wfopen((const wchar_t*)filepath, L"wb");
#else
		FILE *fp = fopen(asd::ToUtf8String(filepath).c_str(), "wb");
#endif

		if (fp == nullptr) return;

		png_structp pp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		png_infop ip = png_create_info_struct(pp);

		/* 書き込み準備 */
		png_init_io(pp, fp);
		png_set_IHDR(pp, ip, width, height,
			8, /* 8bit以外にするなら変える */
			PNG_COLOR_TYPE_RGBA, /* RGBA以外にするなら変える */
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

		/* ピクセル領域確保 */
		raw1D = (png_bytep)malloc(height * png_get_rowbytes(pp, ip));
		raw2D = (png_bytepp)malloc(height * sizeof(png_bytep));
		for (int32_t i = 0; i < height; i++)
		{
			raw2D[i] = &raw1D[i*png_get_rowbytes(pp, ip)];
		}

		memcpy((void*)raw1D, data, width * height * 4);

		/* 上下反転 */
		if (rev)
		{
			for (int32_t i = 0; i < height / 2; i++)
			{
				png_bytep swp = raw2D[i];
				raw2D[i] = raw2D[height - i - 1];
				raw2D[height - i - 1] = swp;
			}
		}

		/* 書き込み */
		png_write_info(pp, ip);
		png_write_image(pp, raw2D);
		png_write_end(pp, ip);

		/* 解放 */
		png_destroy_write_struct(&pp, &ip);
		fclose(fp);
		free(raw1D);
		free(raw2D);
	}

}
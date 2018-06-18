
//-----------------------------------------------------------------------------------
// Include
//-----------------------------------------------------------------------------------
#include "BitmapWriter.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool BitmapWriter::Write( const char* path, uint32_t* data, int32_t width, int32_t height )
{
	FILE* fp = nullptr;
	fopen_s(&fp, path, "wb");
	if( fp == NULL ) return false;

	auto output_datasize = width * height * 4;

	// 32bitbmp
	{
		// BM
		fwrite( "BM", sizeof(uint8_t), 2, fp );

		// ファイルサイズ
		uint32_t filesize = 14 + 40 + output_datasize;
		fwrite( &filesize, sizeof(uint32_t), 1, fp );

		// 空
		uint32_t empty = 0;
		fwrite( &empty, sizeof(uint32_t), 1, fp );

		// オフセット
		uint32_t offset = 14 + 40;
		fwrite( &offset, sizeof(uint32_t), 1, fp );

		//
		uint32_t infosize = 40;
		fwrite( &infosize, sizeof(uint32_t), 1, fp );

		//
		uint32_t width_ = (uint32_t)width;
		fwrite( &width_, sizeof(uint32_t), 1, fp );

		//
		uint32_t height_ = (uint32_t)height;
		fwrite( &height_, sizeof(uint32_t), 1, fp );

		//
		uint16_t planes = 1;
		fwrite( &planes, sizeof(uint16_t), 1, fp );

		//
		uint16_t bitcount = 32;
		fwrite( &bitcount, sizeof(uint16_t), 1, fp );

		//
		uint32_t compre = 0;
		fwrite( &compre, sizeof(uint32_t), 1, fp );

		//
		uint32_t size_image = (uint32_t)output_datasize;
		fwrite( &size_image, sizeof(uint32_t), 1, fp );

		//
		uint32_t xpp = 0;
		fwrite( &xpp, sizeof(uint32_t), 1, fp );

		//
		uint32_t ypp = 0;
		fwrite( &ypp, sizeof(uint32_t), 1, fp );

		//
		uint32_t cu = 0;
		fwrite( &cu, sizeof(uint32_t), 1, fp );

		//
		uint32_t ci = 0;
		fwrite( &ci, sizeof(uint32_t), 1, fp );

		//
		fwrite( data, sizeof(uint8_t), output_datasize, fp );
	}
	
	fclose( fp );

	return true;
}

#pragma once
#include "../../../../PCH/asd.Core.PCH.h"
#include "../../../../asd.Core.Base.h"

#ifdef __APPLE__
#include <OpenGL/gl3ext.h>
#endif

namespace asd
{
	/**
		参考
		http://asura.iaigiri.com/OpenGL/gl7.html
	*/

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

	typedef struct _DDPixelFormat
	{
		GLuint size;
		GLuint flgas;
		GLuint fourCC;
		GLuint bpp;
		GLuint redMask;
		GLuint greenMask;
		GLuint blueMask;
		GLuint alphaMask;
	} DDPixelFormat;

	typedef struct _DDSCaps
	{
		GLuint caps;
		GLuint caps2;
		GLuint caps3;
		GLuint caps4;
	} DDSCaps;

	typedef struct _DDColorKey
	{
		GLuint lowVal;
		GLuint highVal;
	} DDColorKey;

	typedef struct _DDSurfaceDesc
	{
		GLuint size;
		GLuint flags;
		GLuint height;
		GLuint width;
		GLuint pitch;
		GLuint depth;
		GLuint mipMapLevels;
		GLuint alphaBitDepth;
		GLuint reserved;
		GLuint surface;

		DDColorKey ckDestOverlay;
		DDColorKey ckDestBlt;
		DDColorKey ckSrcOverlay;
		DDColorKey ckSrcBlt;

		DDPixelFormat format;
		DDSCaps caps;

		GLuint textureStage;
	} DDSurfaceDesc;

	
	class OpenGLDDSReader
	{
	public:
		bool Read(void* data, int32_t size, int32_t& width, int32_t& height, int32_t& numMipmaps, GLuint& texture, TextureFormat& fmt)
		{
			char magic[4];
			int mipmapFactor;
			long curr, end;
			DDSurfaceDesc ddsd;
			GLenum format;
			GLuint internalFormat;

			auto p = (uint8_t*) data;
			
			//　マジックを読み取り
			memcpy(magic, p, 4);
			p += 4;

			//　マジックをチェック
			if (strncmp(magic, "DDS ", 4) != 0)
			{
				return false;
			}

			//　ヘッダーを読み取り
			memcpy(&ddsd, p, sizeof(ddsd));
			p += sizeof(ddsd);

			//　幅・高さを格納
			height = ddsd.height;
			width = ddsd.width;
			numMipmaps = ddsd.mipMapLevels;

			//　フォーマット判別
			switch (ddsd.format.fourCC)
			{
			case FOURCC_DXT1:
				//　DXT1
				format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				internalFormat = 3;
				mipmapFactor = 2;
				fmt = TextureFormat::BC1;
				break;

			case FOURCC_DXT3:
				//　DXT3
				format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				internalFormat = 4;
				mipmapFactor = 4;
				fmt = TextureFormat::BC2;
				break;

			case FOURCC_DXT5:
				//　DXT5
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				internalFormat = 4;
				mipmapFactor = 4;
				fmt = TextureFormat::BC3;
				break;

			default:
				return false;
			}

			texture = 0;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			int blockSize;
			int offset = 0;
			GLsizei mWidth = width, mHeight = height, mSize = 0;

			//　DXT1
			if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
				blockSize = 8;
			//　DXT3, DXT5
			else
				blockSize = 16;

			//　解凍
			for (int i = 0; i<(int) numMipmaps; i++)
			{
				mSize = ((mWidth + 3) / 4) * ((mHeight + 3) / 4) * blockSize;
				glCompressedTexImage2D(GL_TEXTURE_2D, i, format, mWidth, mHeight, 0, mSize, p);

				if (mWidth >> 1)  mWidth = (mWidth >> 1);
				else    mWidth = 1;

				if (mHeight >> 1) mHeight = (mHeight >> 1);
				else    mHeight = 1;

				offset += mSize;
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};

}
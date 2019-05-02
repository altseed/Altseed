
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Graphics_Imp.h"
#include "asd.RenderingThread.h"
#include <vector>

#include "../IO/asd.File_Imp.h"
#include "../Log/asd.Log.h"

#include "Resource/asd.CubemapTexture_Imp.h"
#include "Resource/asd.DepthBuffer_Imp.h"
#include "Resource/asd.Effect_Imp.h"
#include "Resource/asd.IndexBuffer_Imp.h"
#include "Resource/asd.Material2D_Imp.h"
#include "Resource/asd.Material3D_Imp.h"
#include "Resource/asd.NativeShader_Imp.h"
#include "Resource/asd.Shader2D_Imp.h"
#include "Resource/asd.Shader3D_Imp.h"
#include "Resource/asd.VertexBuffer_Imp.h"

#include "Resource/asd.Effect_Imp.h"

#include "Resource/asd.MaterialPropertyBlock_Imp.h"
#include "Resource/asd.ShaderCache.h"

#include "Media/asd.MediaPlayer.h"
#include "Resource/asd.ImagePackage_Imp.h"

#if _WIN32
#include "Media/Platform/asd.MediaPlayerWMF.h"
#endif

#ifdef __APPLE__
#include "Media/Platform/asd.MediaPlayerAVF.h"
#endif

#include "3D/Resource/asd.Deformer_Imp.h"
#include "3D/Resource/asd.MassModel_Imp.h"
#include "3D/Resource/asd.Mesh_Imp.h"
#include "3D/Resource/asd.Model_Imp.h"

#include "Helper/asd.EffekseerHelper.h"

#if !defined(_CONSOLE_GAME)
#include "3D/Resource/asd.Terrain3D_Imp.h"
#endif

#include "3D/Resource/Animation/asd.AnimationClip.h"
#include "3D/Resource/Animation/asd.AnimationSource.h"

#include <Graphics/3D/asd.MassModel_IO.h>
#include <Graphics/3D/asd.Model_IO.h>

#include "../Log/asd.Log.h"
#include "../Window/asd.Window_Imp.h"

#define Z_SOLO
#include <png.h>
#include <pngstruct.h>
#include <pnginfo.h>

#if _WIN32
#include <GdiPlus.h>
#include <Gdiplusinit.h>
#include <Windows.h>
#pragma comment(lib, "gdiplus.lib")

#endif

#include <sstream>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#ifdef _WIN32
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <sys/stat.h>
#endif

static void CreateShaderCacheDirectory()
{
	const char* shaderCacheDirectory = "ShaderCache";
#if _WIN32
	if (!PathIsDirectoryA(shaderCacheDirectory))
	{
		CreateDirectoryA(shaderCacheDirectory, NULL);
	}
#else
	mkdir(shaderCacheDirectory, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH | S_IXOTH);
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd
{

class EffectLoader : public Effekseer::EffectLoader
{
private:
	File* file = nullptr;

public:
	EffectLoader(File* file) : file(file) {}

	bool Load(const EFK_CHAR* path, void*& data, int32_t& size)
	{
		auto sf = file->CreateStaticFile((const achar*)path);
		if (sf == nullptr)
			return false;

		size = sf->GetSize();
		data = new uint8_t[size];
		memcpy(data, sf->GetData(), size);

		SafeRelease(sf);

		return true;
	}

	void Unload(void* data, int32_t size)
	{
		auto d = (uint8_t*)data;
		SafeDeleteArray(d);
	}
};

static void GetParentDir(EFK_CHAR* dst, const EFK_CHAR* src)
{
	int i, last = -1;
	for (i = 0; src[i] != L'\0'; i++)
	{
		if (src[i] == L'/' || src[i] == L'\\')
			last = i;
	}
	if (last >= 0)
	{
		memcpy(dst, src, last * sizeof(EFK_CHAR));
		dst[last] = L'\0';
	}
	else
	{
		dst[0] = L'\0';
	}
}

static astring GetFileNameWithoutExtension(const achar* filepath)
{
	auto path = astring(filepath);
	size_t i = path.rfind('.', path.length());
	if (i != astring::npos)
	{
		return (path.substr(0, i));
	}
	return astring();
}

static astring GetFileExt(const achar* filepath)
{
	auto path = astring(filepath);
	size_t i = path.rfind('.', path.length());
	if (i != astring::npos)
	{
		return (path.substr(i + 1, path.length() - i));
	}
	return astring();
}

static achar tolower_(achar in)
{
	if (in <= 'Z' && in >= 'A')
	{
		return in - ('Z' - 'z');
	}
	return in;
}

void ImageHelper::SaveImage(const achar* filepath, int32_t width, int32_t height, void* data, bool rev)
{
#if (defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE))

#else
	auto ext_ = GetFileExt(filepath);
	astring ext(ext_);
	std::transform(ext_.begin(), ext_.end(), ext.begin(), tolower_);

	if (ext == ToAString("png"))
	{
		SavePNGImage(filepath, width, height, data, rev);
	}

	if (ext == ToAString("jpeg") || ext == ToAString("jpg"))
	{
		SaveJPGImage(filepath, width, height, data, rev);
	}
#endif
}

void ImageHelper::SaveJPGImage(const achar* filepath, int32_t width, int32_t height, void* data, bool rev)
{
#if _WIN32
	// GDI+を初期化する。
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	{
		// bmpを作成する。
		Gdiplus::Bitmap bmp(width, height);

		auto p = (Color*)data;
		for (int32_t y = 0; y < height; y++)
		{
			for (int32_t x = 0; x < width; x++)
			{
				if (rev)
				{
					auto src = p[x + width * (height - 1 - y)];
					Gdiplus::Color dst(src.R, src.G, src.B);
					bmp.SetPixel(x, y, dst);
				}
				else
				{
					auto src = p[x + width * y];
					Gdiplus::Color dst(src.R, src.G, src.B);
					bmp.SetPixel(x, y, dst);
				}
			}
		}

		// 保存
		CLSID id;
		UINT encoderNum = 0;
		UINT encoderSize = 0;
		Gdiplus::GetImageEncodersSize(&encoderNum, &encoderSize);
		if (encoderSize == 0)
		{
			Gdiplus::GdiplusShutdown(gdiplusToken);
			return;
		}

		auto imageCodecInfo = (Gdiplus::ImageCodecInfo*)malloc(encoderSize);
		Gdiplus::GetImageEncoders(encoderNum, encoderSize, imageCodecInfo);

		for (UINT i = 0; i < encoderNum; i++)
		{
			if (wcscmp(imageCodecInfo[i].MimeType, L"image/jpeg") == 0)
			{
				id = imageCodecInfo[i].Clsid;
				free(imageCodecInfo);
				imageCodecInfo = nullptr;
				break;
			}
		}

		if (imageCodecInfo != nullptr)
		{
			free(imageCodecInfo);
			return;
		}

		bmp.Save((const wchar_t*)filepath, &id);
	}

	// GDI+を終了する。
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return;
#else
	return;
#endif // _WIN32
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static void PngReadData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	auto d = (uint8_t**)png_get_io_ptr(png_ptr);

	memcpy(data, *d, length);
	(*d) += length;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ImageHelper::SavePNGImage(const achar* filepath, int32_t width, int32_t height, void* data, bool rev)
{
	/* 構造体確保 */
#if _WIN32
	FILE* fp = _wfopen((const wchar_t*)filepath, L"wb");
#else
	FILE* fp = fopen(ToUtf8String(filepath).c_str(), "wb");
#endif

	if (fp == nullptr)
		return;

	png_structp pp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop ip = png_create_info_struct(pp);

	/* 書き込み準備 */
	png_init_io(pp, fp);
	png_set_IHDR(pp,
				 ip,
				 width,
				 height,
				 8,					  /* 8bit以外にするなら変える */
				 PNG_COLOR_TYPE_RGBA, /* RGBA以外にするなら変える */
				 PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_DEFAULT,
				 PNG_FILTER_TYPE_DEFAULT);

	/* ピクセル領域確保 */
	std::vector<png_byte> raw1D(height * png_get_rowbytes(pp, ip));
	std::vector<png_bytep> raw2D(height * sizeof(png_bytep));
	for (int32_t i = 0; i < height; i++)
	{
		raw2D[i] = &raw1D[i * png_get_rowbytes(pp, ip)];
	}

	memcpy((void*)raw1D.data(), data, width * height * 4);

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
	png_write_image(pp, raw2D.data());
	png_write_end(pp, ip);

	/* 解放 */
	png_destroy_write_struct(&pp, &ip);
	fclose(fp);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

struct ReadPNGStruct
{
	int32_t Width = 0;
	int32_t Height = 0;
	std::vector<uint8_t>* DstPtr = nullptr;
	bool Rev = false;
};

void readPNGImage(const uint8_t* data, int32_t width, int32_t height, void* userData)
{
	auto s = (ReadPNGStruct*)userData;
	s->Width = width;
	s->Height = height;
	s->DstPtr->resize(width * height * 4);

	if (s->Rev)
	{
		for (auto y = 0; y < height; y++)
		{
			memcpy(s->DstPtr->data() + (y * 4 * width), data + ((height - 1 - y) * 4 * width), width * 4);
		}
	}
	else
	{
		memcpy(s->DstPtr->data(), data, width * height * 4);
	}
}

bool ImageHelper::LoadPNGImage(
	void* data, int32_t size, bool rev, int32_t& imagewidth, int32_t& imageheight, std::vector<uint8_t>& imagedst, Log* log)
{
	imagewidth = 0;
	imageheight = 0;
	imagedst.clear();

	uint8_t* data_ = (uint8_t*)data;

	/* pngアクセス構造体を作成 */
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	/* リードコールバック関数指定 */
	png_set_read_fn(png, &data_, &PngReadData);

	/* png画像情報構造体を作成 */
	png_infop png_info = png_create_info_struct(png);

	/* エラーハンドリング */
	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_read_struct(&png, &png_info, NULL);

		if (log != nullptr)
		{
			log->WriteLineStrongly(u"pngファイルのヘッダの読み込みに失敗しました", LogLevel::All);
		}

		return false;
	}

	/* IHDRチャンク情報を取得 */
	png_read_info(png, png_info);

	/* インターレース */
	auto number_of_passes = png_set_interlace_handling(png);
	if (number_of_passes == 0)
	{
		number_of_passes = 1;
	}

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
	{
		png_set_palette_to_rgb(png);

		png_bytep trans_alpha = NULL;
		int num_trans = 0;
		png_color_16p trans_color = NULL;

		png_get_tRNS(png, png_info, &trans_alpha, &num_trans, &trans_color);
		if (trans_alpha != NULL)
		{
			pixelBytes = 4;
		}
		else
		{
			pixelBytes = 3;
		}
	}
	break;
	case PNG_COLOR_TYPE_GRAY:
		png_set_expand_gray_1_2_4_to_8(png);
		png_set_gray_to_rgb(png);
		pixelBytes = 3;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		png_set_gray_to_rgb(png);
		pixelBytes = 4;
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
		png_bytepp rp;
		for (auto pass = 0; pass < number_of_passes; pass++)
		{
			for (uint32_t i = 0; i < png_info->height; i++)
			{
				png_read_row(png, &image[(png_info->height - 1 - i) * pitch], NULL);
			}
		}
	}
	else
	{
		for (auto pass = 0; pass < number_of_passes; pass++)
		{
			for (uint32_t i = 0; i < png_info->height; i++)
			{
				png_read_row(png, &image[i * pitch], NULL);
			}
		}
	}

	imagewidth = png_info->width;
	imageheight = png_info->height;
	imagedst.resize(imagewidth * imageheight * 4);
	auto imagedst_ = imagedst.data();

	if (pixelBytes == 4)
	{
		memcpy(imagedst_, image, imagewidth * imageheight * 4);
	}
	else
	{
		for (int32_t y = 0; y < imageheight; y++)
		{
			for (int32_t x = 0; x < imagewidth; x++)
			{
				auto src = (x + y * imagewidth) * 3;
				auto dst = (x + y * imagewidth) * 4;
				imagedst_[dst + 0] = image[src + 0];
				imagedst_[dst + 1] = image[src + 1];
				imagedst_[dst + 2] = image[src + 2];
				imagedst_[dst + 3] = 255;
			}
		}
	}

	SafeDeleteArray(image);
	png_destroy_read_struct(&png, &png_info, NULL);

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int32_t ImageHelper::GetPitch(TextureFormat format)
{
	if (format == TextureFormat::R8G8B8A8_UNORM)
		return 4;
	if (format == TextureFormat::R16G16B16A16_FLOAT)
		return 2 * 4;
	if (format == TextureFormat::R32G32B32A32_FLOAT)
		return 4 * 4;
	if (format == TextureFormat::R8G8B8A8_UNORM_SRGB)
		return 4;
	if (format == TextureFormat::R16G16_FLOAT)
		return 2 * 2;
	if (format == TextureFormat::R8_UNORM)
		return 1;

	// 1ピクセル単位で返せない
	if (format == TextureFormat::BC1)
		return 0;
	if (format == TextureFormat::BC1_SRGB)
		return 0;
	if (format == TextureFormat::BC2)
		return 0;
	if (format == TextureFormat::BC2_SRGB)
		return 0;
	if (format == TextureFormat::BC3)
		return 0;
	if (format == TextureFormat::BC3_SRGB)
		return 0;

	return 0;
}

int32_t ImageHelper::GetVRAMSize(TextureFormat format, int32_t width, int32_t height)
{
	auto pitch = GetPitch(format);

	if (pitch == 0)
	{
		if (format == TextureFormat::BC1 || format == TextureFormat::BC1_SRGB)
			return width * height * 4 / 6;

		if (format == TextureFormat::BC2 || format == TextureFormat::BC2_SRGB)
			return width * height * 4 / 4;

		if (format == TextureFormat::BC3 || format == TextureFormat::BC3_SRGB)
			return width * height * 4 / 4;
	}

	return pitch * width * height;
}

int32_t ImageHelper::GetMipmapCount(int32_t width, int32_t height)
{
	auto ret = 1;
	while (width != 1 || height != 1)
	{
		if (width > 1)
			width = width >> 1;
		if (height > 1)
			height = height >> 1;
		ret++;
	}

	return ret;
}

void ImageHelper::GetMipmapSize(int mipmap, int32_t& width, int32_t& height)
{
	for (auto i = 0; i < mipmap; i++)
	{
		if (width > 1)
			width = width >> 1;
		if (height > 1)
			height = height >> 1;
	}
}

bool ImageHelper::IsPNG(const void* data, int32_t size)
{
	if (size < 4)
		return false;

	auto d = (uint8_t*)data;

	if (d[0] != 0x89)
		return false;
	if (d[1] != 'P')
		return false;
	if (d[2] != 'N')
		return false;
	if (d[3] != 'G')
		return false;

	return true;
}

bool ImageHelper::IsDDS(const void* data, int32_t size)
{
	if (size < 4)
		return false;

	auto d = (uint8_t*)data;

	if (d[0] != 'D')
		return false;
	if (d[1] != 'D')
		return false;
	if (d[2] != 'S')
		return false;

	return true;
}

std::string GraphicsHelper::GetFormatName(Graphics_Imp* graphics, TextureFormat format)
{
	if (format == TextureFormat::R8_UNORM)
		return std::string("R8_UNORM");
	if (format == TextureFormat::R16G16B16A16_FLOAT)
		return std::string("R16G16B16A16_FLOAT");
	if (format == TextureFormat::R32G32B32A32_FLOAT)
		return std::string("R32G32B32A32_FLOAT");
	if (format == TextureFormat::R8G8B8A8_UNORM)
		return std::string("R8G8B8A8_UNORM");
	if (format == TextureFormat::R8G8B8A8_UNORM_SRGB)
		return std::string("R8G8B8A8_UNORM_SRGB");
	if (format == TextureFormat::R16G16_FLOAT)
		return std::string("R16G16_FLOAT");
	return std::string("Unknown");
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
WindowOpenGLX11::WindowOpenGLX11() {}

WindowOpenGLX11::~WindowOpenGLX11()
{
#if _WIN32
#elif __APPLE__
#else
	glXMakeContextCurrent(x11Display, 0, NULL);
	glXDestroyContext(x11Display, glx);
#endif
}

bool WindowOpenGLX11::Initialize(void* display, void* window)
{
#if _WIN32
	return false;
#elif __APPLE__
	return false;
#else
	Display* display_ = (Display*)display;
	::Window window_ = *((::Window*)window);

	GLint attribute[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
	XVisualInfo* vi = glXChooseVisual(display_, DefaultScreen(display_), attribute);

	if (vi == nullptr)
	{
		return false;
	}

	GLXContext context = glXCreateContext(display_, vi, 0, GL_TRUE);

	XFree(vi);

	glXMakeContextCurrent(display_, window_, context);

	glx = context;
	x11Display = display_;
	x11Window = window_;
	return true;
#endif
}

void WindowOpenGLX11::MakeContextCurrent()
{
#if _WIN32
#elif __APPLE__
#else
	glXMakeContextCurrent(x11Display, x11Window, glx);
#endif
}

void WindowOpenGLX11::SwapBuffers()
{
#if _WIN32
#elif __APPLE__
#else
	glXSwapBuffers(x11Display, x11Window);
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#if _WIN32
void WriteAdapterInformation(Log* log, IDXGIAdapter1* adapter, int32_t index)
{
	if (log == nullptr)
		return;

	auto write = [log](std::ostringstream& os) -> void { log->Write(ToAString(os.str().c_str()).c_str()); };

	auto writeTable = [log, write](const char* title, std::ostringstream& text, bool isLast) -> void {
		log->Write(ToAString(title).c_str());
		log->ChangeColumn();
		write(text);
		if (!isLast)
		{
			log->ChangeRow();
		}
	};

	{
		DXGI_ADAPTER_DESC1 adapterDesc;

		auto hr = adapter->GetDesc1(&adapterDesc);

		std::ostringstream title, card, vendor, device, subSys, revision, videoMemory, systemMemory, sharedSystemMemory;

		title << "デバイス情報 (" << (index + 1) << ")";

		if (SUCCEEDED(hr))
		{
			card << ToUtf8String((const achar*)adapterDesc.Description);
			vendor << adapterDesc.VendorId;
			device << adapterDesc.DeviceId;
			subSys << adapterDesc.SubSysId;
			revision << adapterDesc.Revision;
			videoMemory << (adapterDesc.DedicatedVideoMemory / 1024 / 1024) << "MB";
			systemMemory << (adapterDesc.DedicatedSystemMemory / 1024 / 1024) << "MB";
			sharedSystemMemory << (adapterDesc.SharedSystemMemory / 1024 / 1024) << "MB";
		}

		log->WriteLineStrongly(ToAString(title.str().c_str()).c_str());

		log->BeginTable();

		writeTable("GraphicCard", card, false);
		writeTable("VendorID", vendor, false);
		writeTable("DeviceID", device, false);
		writeTable("SubSysID", subSys, false);
		writeTable("Revision", revision, false);
		writeTable("VideoMemory", videoMemory, false);
		writeTable("SystemMemory", systemMemory, false);
		writeTable("SharedSystemMemory", sharedSystemMemory, true);

		log->EndTable();
	}

	for (int32_t i = 0;; i++)
	{
		IDXGIOutput* temp = nullptr;
		if (adapter->EnumOutputs(i, &temp) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_OUTPUT_DESC outputDesc;

			if (SUCCEEDED(temp->GetDesc(&outputDesc)))
			{
				std::ostringstream title, name, attach, pos;

				title << "アウトプット情報 (" << (i + 1) << ")";

				name << ToUtf8String((const achar*)outputDesc.DeviceName);
				attach << (outputDesc.AttachedToDesktop == TRUE ? "True" : "False");
				pos << "(" << outputDesc.DesktopCoordinates.left << "," << outputDesc.DesktopCoordinates.top << ","
					<< (outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left) << ","
					<< (outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top) << ")";

				log->WriteLineStrongly(ToAString(title.str().c_str()).c_str());
				log->BeginTable();
				writeTable("Name", name, false);
				writeTable("AttachedToDesktop", attach, false);
				writeTable("Coordinate", pos, true);
				log->EndTable();
			}

			SafeRelease(temp);
		}
		else
		{
			break;
		}
	}
}
#endif

void WriteDeviceInformation(ar::Manager* manager, Log* log)
{
	if (log == nullptr)
		return;

	log->WriteLineStrongly(ToAString("ビデオカード情報").c_str());

	log->BeginTable();

	log->Write(ToAString("バージョン").c_str());
	log->ChangeColumn();

#ifndef __APPLE__
	if (GLEW_VERSION_4_0)
	{
		log->Write(ToAString("4.0").c_str());
	}
	else if (GLEW_VERSION_3_3)
	{
		log->Write(ToAString("3.3").c_str());
	}
	else if (GLEW_VERSION_3_2)
	{
		log->Write(ToAString("3.2").c_str());
	}
	else if (GLEW_VERSION_3_1)
	{
		log->Write(ToAString("3.1").c_str());
	}
	else if (GLEW_VERSION_3_0)
	{
		log->Write(ToAString("3.0").c_str());
	}
	else if (GLEW_VERSION_2_1)
	{
		log->Write(ToAString("2.1").c_str());
	}
	else
	{
		log->Write(ToAString("Unknown").c_str());
	}
#endif
	log->EndTable();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::AddDeviceObject(DeviceObject* o)
{
	assert(m_deviceObjects.count(o) == 0);
	m_deviceObjects.insert(o);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::RemoveDeviceObject(DeviceObject* o)
{
	assert(m_deviceObjects.count(o) == 1);
	m_deviceObjects.erase(o);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::ResetDrawState()
{
	SafeRelease(m_vertexBufferPtr);
	SafeRelease(m_indexBufferPtr);
	SafeRelease(m_shaderPtr);
}

void Graphics_Imp::StartRenderingThreadFunc(void* self)
{
	auto self_ = (Graphics_Imp*)self;
	self_->StartRenderingThread();
}

void Graphics_Imp::StartRenderingThread() {}

void Graphics_Imp::EndRenderingThreadFunc(void* self)
{
	auto self_ = (Graphics_Imp*)self;
	self_->EndRenderingThread();
}

void Graphics_Imp::EndRenderingThread() {}

RenderTexture2D* Graphics_Imp::CreateRenderTexture2D_(int32_t width, int32_t height, TextureFormat format)
{
	return CreateRenderTexture2D_Imp(width, height, format);
}

Texture2D_Imp* Graphics_Imp::CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp::Create(this, data, size, false, this->GetOption().ColorSpace == ColorSpaceType::LinearSpace);
	return ret;
}

bool Graphics_Imp::CreateTexture2DAsync_Imp_Internal(Texture2D_Imp* texture, uint8_t* data, int32_t size)
{
	return texture->Load(data, size, false, this->GetOption().ColorSpace == ColorSpaceType::LinearSpace);
}

Texture2D_Imp* Graphics_Imp::CreateTexture2DAsRawData_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp::Create(this, data, size, false, false);
	return ret;
}

Texture2D_Imp*
Graphics_Imp::CreateEmptyTexture2D_Imp_Internal(Graphics* graphics, int32_t width, int32_t height, TextureFormat format, void* data)
{
	auto ret = Texture2D_Imp::Create(this, width, height, format, data);
	return ret;
}

Texture2D_Imp* Graphics_Imp::CreateEditableTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size)
{
	auto ret = Texture2D_Imp::Create(this, data, size, true, true);
	return ret;
}

Graphics_Imp* Graphics_Imp::Create(
	Window* window, void* handle1, void* handle2, int32_t width, int32_t height, Log* log, File* file, GraphicsOption option)
{
	auto writeLogHeading = [log](const astring s) -> void {
		if (log == nullptr)
			return;
		log->WriteHeading(s.c_str());
	};

	auto writeLog = [log](const astring s) -> void {
		if (log == nullptr)
			return;
		log->WriteLine(s.c_str());
	};

	WindowOpenGLX11* windowHelper = nullptr;

	if (option.GraphicsDevice == GraphicsDeviceType::DirectX11)
	{
		writeLogHeading(ToAString("描画(DirectX11)"));
	}
	else if (option.GraphicsDevice == GraphicsDeviceType::OpenGL)
	{
		writeLogHeading(ToAString("描画(OpenGL)"));
	}

	// ShowData
	if (option.GraphicsDevice == GraphicsDeviceType::DirectX11)
	{
#if _WIN32
		IDXGIFactory1* dxgiFactory = NULL;
		std::vector<IDXGIAdapter1*> adapters;

		HRESULT hr;

		hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&dxgiFactory);
		if (dxgiFactory == NULL)
		{
			writeLog(ToAString("ファクトリの作成に失敗"));
			goto End;
		}

		for (int32_t i = 0;; i++)
		{
			IDXGIAdapter1* temp = 0;
			if (dxgiFactory->EnumAdapters1(i, &temp) != DXGI_ERROR_NOT_FOUND)
			{
				WriteAdapterInformation(log, temp, i);
				temp->Release();
			}
			else
			{
				break;
			}
		}

		dxgiFactory->Release();
#endif
	}

	ar::Manager* manager = nullptr;

	auto window_ = (Window_Imp*)window;
	if (window_ != nullptr)
	{
		window_->GetWindow()->MakeContextCurrent();
	}
	else
	{
		bool succeed = true;
		auto msg = ToAString("外部ウインドウに対応していません。");
#if _WIN32
		if (option.GraphicsDevice == GraphicsDeviceType::OpenGL)
		{
			succeed = false;
		}
#elif __APPLE__
		succeed = false;
#else
		windowHelper = new WindowOpenGLX11();
		if (windowHelper->Initialize(handle1, handle2))
		{
			windowHelper->MakeContext();
		}
		else
		{
			succeed = false;
		}
#endif
		if (!succeed)
		{
			writeLog(msg);
			return nullptr;
		}
	}

	if (window_ != nullptr)
	{
		// For retina
		window_->GetWindow()->GetFrameBufferSize(width, height);
	}

	manager = ar::Manager::Create((ar::GraphicsDeviceType)option.GraphicsDevice);
	ar::ManagerInitializationParameter initParam;
	initParam.WindowWidth = width;
	initParam.WindowHeight = height;
	initParam.IsFullscreenMode = option.IsFullScreen;
	initParam.ColorSpace = (ar::ColorSpaceType)option.ColorSpace;
	initParam.Handles[0] = handle1;
	initParam.Handles[1] = handle2;

	auto errorCode = manager->Initialize(initParam);

	if (errorCode == ar::ErrorCode::OK)
	{
	}
	else if (errorCode == ar::ErrorCode::FailedToCreateFactory)
	{
		writeLog(ToAString("ファクトリの作成に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToGetAdapter)
	{
		writeLog(ToAString("アダプタの取得に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToCreateDevice)
	{
		writeLog(ToAString("デバイスの作成に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToCreateSwapChain)
	{
		writeLog(ToAString("スワップチェーンの作成に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToGetBackBuffer)
	{
		writeLog(ToAString("バックバッファの取得に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToCreateBackBufferTarget)
	{
		writeLog(ToAString("バックバッファのレンダーターゲットの取得に失敗"));
		goto End;
	}
	else if (errorCode == ar::ErrorCode::FailedToInitializeGlew)
	{
		writeLog(ToAString("Glewの初期化に失敗"));
		goto End;
	}

	if (option.GraphicsDevice == GraphicsDeviceType::OpenGL)
	{
		if (manager->GetVersion() < 33)
		{
			writeLog(ToAString("OpenGLのバージョンが3.3未満です。"));
			goto End;
		}
	}

	if (option.GraphicsDevice == GraphicsDeviceType::DirectX11)
	{
		writeLogHeading(ToAString("初期化成功(DirectX11)"));
	}
	else if (option.GraphicsDevice == GraphicsDeviceType::OpenGL)
	{
		writeLogHeading(ToAString("初期化成功(OpenGL)"));
	}

	return new Graphics_Imp(manager, window, windowHelper, Vector2DI(width, height), log, file, option);

End:

	asd::SafeDelete(windowHelper);
	asd::SafeDelete(manager);

	if (option.GraphicsDevice == GraphicsDeviceType::DirectX11)
	{
		writeLogHeading(ToAString("初期化失敗(DirectX11)"));
	}
	else if (option.GraphicsDevice == GraphicsDeviceType::OpenGL)
	{
		writeLogHeading(ToAString("初期化失敗(OpenGL)"));
	}

	writeLog(ToAString(""));

	return nullptr;
}

Graphics_Imp* Graphics_Imp::Create(Window* window, Log* log, File* file, GraphicsOption option)
{
	auto size = window->GetSize();
	auto handle = ((Window_Imp*)window)->GetWindowHandle();
	return Create(window, handle, nullptr, size.X, size.Y, log, file, option);
}

Graphics_Imp* Graphics_Imp::Create(void* handle1, void* handle2, int32_t width, int32_t height, Log* log, File* file, GraphicsOption option)
{
	return Create(nullptr, handle1, handle2, width, height, log, file, option);
}

Graphics_Imp* Graphics_Imp::Create(Window* window, GraphicsDeviceType graphicsDevice, Log* log, File* file, GraphicsOption option)
{
#if _WIN32
	if (graphicsDevice == GraphicsDeviceType::OpenGL)
	{
		return Create(window, log, file, option);
	}
	else
	{
		return Create(window, log, file, option);
	}
#else
	if (graphicsDevice == GraphicsDeviceType::OpenGL)
	{
		return Create(window, log, file, option);
	}
	else
	{
		return nullptr;
	}
#endif
	return nullptr;
}

Graphics_Imp* Graphics_Imp::Create(void* handle1,
								   void* handle2,
								   int32_t width,
								   int32_t height,
								   GraphicsDeviceType graphicsDevice,
								   Log* log,
								   File* file,
								   GraphicsOption option)
{
#if _WIN32
	if (graphicsDevice == GraphicsDeviceType::OpenGL)
	{
		return Create((HWND)handle1, handle2, width, height, log, file, option);
	}
	else
	{
		return Create((HWND)handle1, handle2, width, height, log, file, option);
	}
#elif __APPLE__
	return nullptr; // not supported
#else
	return Create(handle1, handle2, width, height, log, file, option);
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp::Graphics_Imp(ar::Manager* manager, Vector2DI size, Log* log, File* file, GraphicsOption option)
	: rhi(manager)
	, m_size(size)
	, m_vertexBufferPtr(nullptr)
	, m_indexBufferPtr(nullptr)
	, m_shaderPtr(nullptr)
	, m_log(log)
	, m_file(file)
	, option(option)
{
	CreateShaderCacheDirectory();

	Texture2DContainer = std::make_shared<ResourceContainer<Texture2D_Imp>>(file);
	EffectContainer = std::make_shared<ResourceContainer<Effect_Imp>>(file);
	FontContainer = std::make_shared<ResourceContainer<Font_Imp>>(file);
	ModelContainer = std::make_shared<ResourceContainer<Model_Imp>>(file);
	ImagePackageContainer = std::make_shared<ResourceContainer<ImagePackage_Imp>>(file);

#if !defined(_CONSOLE_GAME)
	m_renderingThread = std::make_shared<RenderingThread>();
#endif

	m_effectSetting = Effekseer::Setting::Create();
	m_effectSetting->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);
	m_effectSetting->SetEffectLoader(new EffectLoader(file));

	m_effectFileInterface = new EffekseerFile(this);

	m_shaderCache = new ShaderCache(this);

	for (auto i = 0; i < MaxTextureCount; i++)
	{
		currentState.textureFilterTypes[i] = TextureFilterType::Nearest;
		currentState.textureWrapTypes[i] = TextureWrapType::Clamp;
		nextState.textureFilterTypes[i] = TextureFilterType::Nearest;
		nextState.textureWrapTypes[i] = TextureWrapType::Clamp;

		currentState.textureFilterTypes_vs[i] = TextureFilterType::Nearest;
		currentState.textureWrapTypes_vs[i] = TextureWrapType::Clamp;
		nextState.textureFilterTypes_vs[i] = TextureFilterType::Nearest;
		nextState.textureWrapTypes_vs[i] = TextureWrapType::Clamp;
	}
}

Graphics_Imp::Graphics_Imp(
	ar::Manager* manager, Window* window, WindowOpenGLX11* windowHelper, Vector2DI size, Log* log, File* file, GraphicsOption option)
	: Graphics_Imp(manager, size, log, file, option)
{
	m_window = window;
	this->windowHelper = windowHelper;

	SafeAddRef(window);

#if !defined(_CONSOLE_GAME)
	m_renderingThread->Run(this, StartRenderingThreadFunc, EndRenderingThreadFunc);
#endif

	GetEffectSetting()->SetTextureLoader(EffekseerHelper::CreateTextureLoader(this, GetEffectFileInterface()));
	GetEffectSetting()->SetModelLoader(EffekseerHelper::CreateModelLoader(this, GetEffectFileInterface()));

#if _WIN32
	if (GetRHI()->GetDeviceType() == ar::GraphicsDeviceType::DirectX11)
	{
		auto device = (ID3D11Device*)GetRHI()->GetInternalObjects()[0];
		auto flevel = GetDevice()->GetFeatureLevel();

		if (flevel == D3D_FEATURE_LEVEL_9_3 || flevel == D3D_FEATURE_LEVEL_9_2 || flevel == D3D_FEATURE_LEVEL_9_1)
		{
			isInitializedAsDX9 = true;
		}
	}
#endif

	if (GetRHI()->GetDeviceType() == ar::GraphicsDeviceType::OpenGL)
	{
		WriteDeviceInformation(manager, log);
	}

	rhiContext = ar::Context::Create(GetRHI());
	rhiContext->Initialize(GetRHI());

	renderTargets.fill(0);
	currentRenderTargets.fill(0);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp::~Graphics_Imp()
{
#if !defined(_CONSOLE_GAME)
	m_renderingThread->AddEvent(nullptr);
	while (m_renderingThread->IsRunning())
	{
		Sleep(1);
	}
	m_renderingThread.reset();
#endif

	for (auto& r : currentRenderTargets)
	{
		SafeRelease(r);
	}
	SafeRelease(currentDepthTarget);

	for (auto& r : renderTargets)
	{
		SafeRelease(r);
	}
	SafeRelease(depthTarget);

	asd::SafeDelete(windowHelper);
	asd::SafeDelete(rhiContext);
	SafeRelease(m_window);

	SafeDelete(m_shaderCache);

	SafeRelease(m_vertexBufferPtr);
	SafeRelease(m_indexBufferPtr);
	SafeRelease(m_shaderPtr);

	// SafeDelete(m_resourceContainer);

	SafeRelease(m_effectSetting);
	SafeDelete(m_effectFileInterface);
	// SafeRelease(m_log);

	asd::SafeDelete(rhi);
}

void Graphics_Imp::ApplyRenderTargets()
{
	if (isRenderTargetDirty)
	{
		// reset all
		for (auto& r : renderTargets)
		{
			SafeAddRef(r);
		}
		SafeAddRef(depthTarget);

		for (auto& r : currentRenderTargets)
		{
			SafeRelease(r);
		}
		SafeRelease(currentDepthTarget);

		// end scene
		if (isSceneRunning)
		{
			rhiContext->End();
			GetRHI()->EndScene();
		}

		// set parameter
		currentRenderTargets = renderTargets;
		currentDepthTarget = depthTarget;

		ar::SceneParameter sceneParam;

		for (int32_t i = 0; i < 4; i++)
		{
			if (currentRenderTargets[i] == nullptr)
				continue;
			auto rt = (RenderTexture2D_Imp*)currentRenderTargets[i];

			sceneParam.RenderTargets[i] = rt->GetRHI();
		}

		if (currentDepthTarget != nullptr)
		{
			auto db = (DepthBuffer_Imp*)currentDepthTarget;
			sceneParam.DepthTarget = db->GetRHI();
		}

		GetRHI()->BeginScene(sceneParam);
		rhiContext->Begin();

		isRenderTargetDirty = false;
		isSceneRunning = true;
	}
}

void Graphics_Imp::UpdateDrawStates(VertexBuffer_Imp* vertexBuffer,
									IndexBuffer_Imp* indexBuffer,
									NativeShader_Imp* shaderPtr,
									int32_t& vertexBufferOffset)
{
	assert(vertexBuffer != nullptr);
	assert(indexBuffer != nullptr);
	assert(shaderPtr != nullptr);

	auto v_impl = (VertexBuffer_Imp*)vertexBuffer;
	auto i_impl = (IndexBuffer_Imp*)indexBuffer;
	auto s_impl = (NativeShader_Imp*)shaderPtr;

	drawParam.VertexBufferPtr = v_impl->GetRHI();
	drawParam.IndexBufferPtr = i_impl->GetRHI();
	drawParam.ShaderPtr = s_impl->GetRHI();
	drawParam.VertexConstantBufferPtr = s_impl->GetRHIVertexConstantBuffer();
	drawParam.PixelConstantBufferPtr = s_impl->GetRHIPixelConstantBuffer();
	drawParam.VertexShaderTextures.fill(nullptr);
	drawParam.PixelShaderTextures.fill(nullptr);

	vertexBufferOffset = v_impl->GetRHI()->GetVertexOffset();

	s_impl->AssignConstantBuffer();

	for (auto& bt : s_impl->GetBindingTextures())
	{
		if (bt.second.TexturePtr == nullptr)
			continue;
		auto tex = bt.second.TexturePtr.get();
		auto id = bt.first;

		ar::Texture* rhi = nullptr;

		if (bt.second.TexturePtr->GetType() == TextureClassType::Texture2D)
		{
			auto t = (Texture2D_Imp*)tex;
			rhi = t->GetRHI();
		}
		else if (tex->GetType() == TextureClassType::RenderTexture2D)
		{
			auto t = (RenderTexture2D_Imp*)tex;
			rhi = t->GetRHI();
		}
		else if (tex->GetType() == TextureClassType::CubemapTexture)
		{
			auto t = (CubemapTexture_Imp*)tex;
			rhi = t->GetRHI();
		}
		else if (tex->GetType() == TextureClassType::DepthBuffer)
		{
			auto t = (DepthBuffer_Imp*)tex;
			rhi = t->GetRHI();
		}

		if (id >= 0xff)
		{
			// 頂点シェーダーに設定
			if (id - 0xff < drawParam.VertexShaderTextures.size())
			{
				drawParam.VertexShaderTextures[id - 0xff] = rhi;
			}

			nextState.textureFilterTypes_vs[id - 0xff] = bt.second.FilterType;
			nextState.textureWrapTypes_vs[id - 0xff] = bt.second.WrapType;
		}
		else
		{
			// ピクセルシェーダーに設定
			if (id < drawParam.PixelShaderTextures.size())
			{
				drawParam.PixelShaderTextures[id] = rhi;
			}

			// ステート設定
			nextState.textureFilterTypes[id] = bt.second.FilterType;
			nextState.textureWrapTypes[id] = bt.second.WrapType;
		}
	}

	CommitRenderState(false);
}

void Graphics_Imp::DrawPolygonInternal(int32_t count,
									   VertexBuffer_Imp* vertexBuffer,
									   IndexBuffer_Imp* indexBuffer,
									   NativeShader_Imp* shaderPtr)
{
	int32_t vertexBufferOffset = 0;

	UpdateDrawStates(vertexBuffer, indexBuffer, shaderPtr, vertexBufferOffset);

	drawParam.InstanceCount = 1;
	drawParam.VertexOffset = vertexBufferOffset;
	drawParam.IndexCount = count * 3;
	drawParam.IndexOffset = 0;

	ApplyRenderTargets();

	rhiContext->Draw(drawParam);
}

void Graphics_Imp::DrawPolygonInternal(
	int32_t offset, int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr)
{
	int32_t vertexBufferOffset = 0;

	UpdateDrawStates(vertexBuffer, indexBuffer, shaderPtr, vertexBufferOffset);

	drawParam.InstanceCount = 1;
	drawParam.VertexOffset = vertexBufferOffset;
	drawParam.IndexCount = count * 3;
	drawParam.IndexOffset = offset * 3;

	ApplyRenderTargets();

	rhiContext->Draw(drawParam);
}

void Graphics_Imp::DrawPolygonInstancedInternal(
	int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t instanceCount)
{
	int32_t vertexBufferOffset = 0;

	UpdateDrawStates(vertexBuffer, indexBuffer, shaderPtr, vertexBufferOffset);

	drawParam.InstanceCount = instanceCount;
	drawParam.VertexOffset = 0;
	drawParam.IndexCount = count * 3;
	drawParam.IndexOffset = 0;

	ApplyRenderTargets();

	rhiContext->Draw(drawParam);
}

void Graphics_Imp::BeginInternal()
{
	// Reset targets
	for (auto& r : renderTargets)
	{
		SafeRelease(r);
	}
	SafeRelease(depthTarget);

	isRenderTargetDirty = true;

	GetRHI()->BeginRendering();
}

void Graphics_Imp::EndInternal()
{
	if (isSceneRunning)
	{
		rhiContext->End();
		GetRHI()->EndScene();
		isSceneRunning = false;
	}

	GetRHI()->EndRendering();
}

void Graphics_Imp::CommitRenderState(bool forced)
{
	currentState = nextState;

	drawParam.AlphaBlend = (ar::AlphaBlendMode)currentState.renderState.AlphaBlendState;
	drawParam.Culling = (ar::CullingType)currentState.renderState.Culling;
	drawParam.IsDepthTest = currentState.renderState.DepthTest;
	drawParam.IsDepthWrite = currentState.renderState.DepthWrite;

	for (int32_t i = 0; i < drawParam.VertexShaderTextures.size(); i++)
	{
		drawParam.VertexShaderTextureFilers[i] = (ar::TextureFilterType)currentState.textureFilterTypes_vs[i];
		drawParam.VertexShaderTextureWraps[i] = (ar::TextureWrapType)currentState.textureWrapTypes_vs[i];
	}

	for (int32_t i = 0; i < drawParam.PixelShaderTextures.size(); i++)
	{
		drawParam.PixelShaderTextureFilers[i] = (ar::TextureFilterType)currentState.textureFilterTypes[i];
		drawParam.PixelShaderTextureWraps[i] = (ar::TextureWrapType)currentState.textureWrapTypes[i];
	}
}

void Graphics_Imp::SetRenderTarget(RenderTexture2D_Imp* texture, DepthBuffer_Imp* depthBuffer)
{
	SafeAddRef(texture);
	SafeAddRef(depthBuffer);

	for (auto& r : renderTargets)
	{
		SafeRelease(r);
	}
	SafeRelease(depthTarget);

	if (texture == currentRenderTargets[0] && nullptr == currentRenderTargets[1] && nullptr == currentRenderTargets[2] &&
		nullptr == currentRenderTargets[3] && depthBuffer == currentDepthTarget)
	{
		isRenderTargetDirty = false;
	}
	else
	{
		isRenderTargetDirty = true;
	}

	renderTargets[0] = texture;
	depthTarget = depthBuffer;
}

void Graphics_Imp::SetRenderTarget(RenderTexture2D_Imp* texture1,
								   RenderTexture2D_Imp* texture2,
								   RenderTexture2D_Imp* texture3,
								   RenderTexture2D_Imp* texture4,
								   DepthBuffer_Imp* depthBuffer)
{
	SafeAddRef(texture1);
	SafeAddRef(texture2);
	SafeAddRef(texture3);
	SafeAddRef(texture4);
	SafeAddRef(depthBuffer);

	for (auto& r : renderTargets)
	{
		SafeRelease(r);
	}
	SafeRelease(depthTarget);

	isRenderTargetDirty = true;

	renderTargets[0] = texture1;
	renderTargets[1] = texture2;
	renderTargets[2] = texture3;
	renderTargets[3] = texture4;
	depthTarget = depthBuffer;
}

void Graphics_Imp::MakeContextCurrent()
{
	if (windowHelper != nullptr)
	{
		windowHelper->MakeContextCurrent();
	}
}

void Graphics_Imp::FlushCommand() {}

void Graphics_Imp::SetIsFullscreenMode(bool isFullscreenMode) { GetRHI()->SetIsFullscreenMode(isFullscreenMode); }

void Graphics_Imp::SetWindowSize(Vector2DI size) { GetRHI()->SetWindowSize(size.X, size.Y); }

void Graphics_Imp::Clear(bool isColorTarget, bool isDepthTarget, const Color& color)
{
	ApplyRenderTargets();

	ar::Color color_;
	color_.R = color.R;
	color_.G = color.G;
	color_.B = color.B;
	color_.A = color.A;

	GetRHI()->Clear(isColorTarget, isDepthTarget, color_);
}

void Graphics_Imp::Present()
{
	GetRHI()->Present();

	if (m_window != nullptr)
	{
		auto window_ = (Window_Imp*)m_window;
		window_->GetWindow()->Present();
	}

	if (windowHelper != nullptr)
	{
		windowHelper->SwapBuffers();
	}
}

void Graphics_Imp::SaveScreenshot(const achar* path)
{
	std::vector<Color> bufs;
	Vector2DI size;

	SaveScreenshot(bufs, size);

	ImageHelper::SaveImage(path, size.X, size.Y, bufs.data(), false);
}

void Graphics_Imp::SaveScreenshot(std::vector<Color>& bufs, Vector2DI& size)
{
	std::vector<ar::Color> dst;
	int32_t width;
	int32_t height;

	GetRHI()->SaveScreen(dst, width, height);

	bufs.resize(width * height);
	memcpy(bufs.data(), dst.data(), width * height * sizeof(Color));

	size.X = width;
	size.Y = height;
}

Texture2D_Imp* Graphics_Imp::CreateTexture2D_Imp(const achar* path)
{
	auto nameWE = GetFileNameWithoutExtension((const achar*)path);
	auto ddsPath = nameWE + ToAString("dds");

	Texture2D_Imp* ret = nullptr;

	// DDS優先読み込み
	ret = Texture2DContainer->TryLoad(
		ddsPath.c_str(), [this](uint8_t* data, int32_t size) -> Texture2D_Imp* { return CreateTexture2D_Imp_Internal(this, data, size); });
	if (ret != nullptr)
		return ret;

	ret = Texture2DContainer->TryLoad(
		path, [this](uint8_t* data, int32_t size) -> Texture2D_Imp* { return CreateTexture2D_Imp_Internal(this, data, size); });

	return ret;
}

Texture2D_Imp* asd::Graphics_Imp::CreateTexture2DAsync_Imp(const achar* path)
{
	auto nameWE = GetFileNameWithoutExtension((const achar*)path);
	auto ddsPath = nameWE + ToAString("dds");

	Texture2D_Imp* ret = nullptr;

	// DDS優先読み込み
	ret = Texture2DContainer->TryLoadAsync(ddsPath.c_str(),
										   [this]() -> Texture2D_Imp* { return Texture2D_Imp::Create(this); },
										   [this](Texture2D_Imp* texture, uint8_t* data, int32_t size) -> bool {
											   return CreateTexture2DAsync_Imp_Internal(texture, data, size);
										   });
	if (ret != nullptr)
		return ret;

	ret = Texture2DContainer->TryLoadAsync(path,
										   [this]() -> Texture2D_Imp* { return Texture2D_Imp::Create(this); },
										   [this](Texture2D_Imp* texture, uint8_t* data, int32_t size) -> bool {
											   return CreateTexture2DAsync_Imp_Internal(texture, data, size);
										   });

	return ret;
}


Texture2D_Imp* Graphics_Imp::CreateTexture2DAsRawData_Imp(const achar* path)
{
	auto nameWE = GetFileNameWithoutExtension((const achar*)path);
	auto ddsPath = nameWE + ToAString("dds");

	Texture2D_Imp* ret = nullptr;

	// DDS優先読み込み
	ret = Texture2DContainer->TryLoad(ddsPath.c_str(), [this](uint8_t* data, int32_t size) -> Texture2D_Imp* {
		return CreateTexture2DAsRawData_Imp_Internal(this, data, size);
	});
	if (ret != nullptr)
		return ret;

	ret = Texture2DContainer->TryLoad(
		path, [this](uint8_t* data, int32_t size) -> Texture2D_Imp* { return CreateTexture2DAsRawData_Imp_Internal(this, data, size); });

	return ret;
}

Texture2D_Imp* Graphics_Imp::CreateEmptyTexture2D_Imp(int32_t width, int32_t height, TextureFormat format)
{
	return CreateEmptyTexture2D_Imp_Internal(this, width, height, format, nullptr);
}

Texture2D_Imp* Graphics_Imp::CreateEditableTexture2D_Imp(const achar* path)
{
	auto staticFile = m_file->CreateStaticFile(path);
	if (staticFile == nullptr)
		return nullptr;

	auto ret = CreateEditableTexture2D_Imp_Internal(this, (uint8_t*)staticFile->GetData(), staticFile->GetSize());
	SafeRelease(staticFile);
	return ret;
}

Texture2D_Imp* Graphics_Imp::CreateTexture2DWithRawData(int32_t width, int32_t height, TextureFormat format, void* data)
{
	return CreateEmptyTexture2D_Imp_Internal(this, width, height, format, data);
}

RenderTexture2D_Imp* Graphics_Imp::CreateRenderTexture2D_Imp(int32_t width, int32_t height, TextureFormat format)
{
	return RenderTexture2D_Imp::Create(this, width, height, format);
}

CubemapTexture* Graphics_Imp::CreateCubemapTexture_(const achar* path) { return CubemapTexture_Imp::Create(this, path); }

DepthBuffer_Imp* Graphics_Imp::CreateDepthBuffer_Imp(int32_t width, int32_t height) { return DepthBuffer_Imp::Create(this, width, height); }

VertexBuffer_Imp* Graphics_Imp::CreateVertexBuffer_Imp_(int32_t size, int32_t count, bool isDynamic)
{
	return VertexBuffer_Imp::Create(this, size, count, isDynamic);
}

IndexBuffer_Imp* Graphics_Imp::CreateIndexBuffer_Imp_(int maxCount, bool isDynamic, bool is32bit)
{
	return IndexBuffer_Imp::Create(this, maxCount, isDynamic, is32bit);
}

NativeShader_Imp* Graphics_Imp::CreateShader_Imp_(const char* vertexShaderText,
												  const char* vertexShaderFileName,
												  const char* pixelShaderText,
												  const char* pixelShaderFileName,
												  std::vector<VertexLayout>& layout,
												  bool is32Bit,
												  std::vector<Macro>& macro)
{
	return NativeShader_Imp::Create(
		this, vertexShaderText, vertexShaderFileName, pixelShaderText, pixelShaderFileName, layout, is32Bit, macro, m_log);
}

NativeShader_Imp* Graphics_Imp::CreateShader_Imp_(const uint8_t* vertexShader,
												  int32_t vertexShaderSize,
												  const uint8_t* pixelShader,
												  int32_t pixelShaderSize,
												  std::vector<VertexLayout>& layout,
												  bool is32Bit)
{
	return NativeShader_Imp::Create(this, vertexShader, vertexShaderSize, pixelShader, pixelShaderSize, layout, is32Bit);
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Shader2D* Graphics_Imp::CreateShader2D_(const achar* shaderText)
{
	auto shader = Shader2D_Imp::Create(this, shaderText, ToAString("").c_str(), m_log);

	return shader;
}

Shader2D* Graphics_Imp::CreateShader2DFromBinary_(const achar* path)
{
	auto shader = Shader2D_Imp::Create(this, path, m_log);

	return shader;
}

Material2D* Graphics_Imp::CreateMaterial2D_(Shader2D* shader)
{
	auto material = Material2D_Imp::Create((Shader2D_Imp*)shader);
	auto material2d = (Material2D*)material;
	return material2d;
}

Shader3D* Graphics_Imp::CreateShader3D_(const achar* shaderText)
{
	auto shader = Shader3D_Imp::Create(this, shaderText, ToAString("").c_str(), m_log);
	return shader;
}

Material3D* Graphics_Imp::CreateMaterial3D_(Shader3D* shader)
{
	auto material = Material3D_Imp::Create((Shader3D_Imp*)shader);
	auto material3d = (Material3D*)material;
	return material3d;
}

MaterialPropertyBlock* Graphics_Imp::CreateMaterialPropertyBlock_()
{
	auto block = new MaterialPropertyBlock_Imp();
	return block;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Mesh* Graphics_Imp::CreateMesh_() { return Mesh_Imp::Create(this); }

Deformer* Graphics_Imp::CreateDeformer_() { return new Deformer_Imp(); }

Model* Graphics_Imp::CreateModel_(const achar* path)
{
	auto ret = ModelContainer->TryLoadWithVector(path, [this, &path](const std::vector<uint8_t>& data) -> Model_Imp* {
		auto model = new Model_Imp(this);
		model->Load(this, data, path);
		return model;
	});

	return ret;
}

MassModel* Graphics_Imp::CreateMassModelFromModelFile_(const achar* path)
{
	auto staticFile = GetFile()->CreateStaticFile(path);
	if (staticFile == nullptr)
		return nullptr;

	std::vector<uint8_t> data;
	data.resize(staticFile->GetSize());

	memcpy(data.data(), staticFile->GetData(), staticFile->GetSize());

	SafeRelease(staticFile);

	Model_IO model_io;
	if (!model_io.Load(data, path))
	{
		return nullptr;
	}

	MassModel_IO massmodel_io;
	if (!massmodel_io.Convert(model_io))
	{
		return nullptr;
	}

	auto massmodel = new MassModel_Imp();

	if (!massmodel->Load(this, massmodel_io))
	{
		SafeDelete(massmodel);
	}

	return massmodel;
}

MassModel* Graphics_Imp::CreateMassModel_(const achar* path)
{
	auto staticFile = GetFile()->CreateStaticFile(path);
	if (staticFile == nullptr)
		return nullptr;

	std::vector<uint8_t> data;
	data.resize(staticFile->GetSize());

	memcpy(data.data(), staticFile->GetData(), staticFile->GetSize());

	SafeRelease(staticFile);

	MassModel_IO massmodel_io;
	if (!massmodel_io.Load(data, path))
	{
		return nullptr;
	}

	auto massmodel = new MassModel_Imp();

	if (!massmodel->Load(this, massmodel_io))
	{
		SafeDelete(massmodel);
	}

	return massmodel;
}

Terrain3D* Graphics_Imp::CreateTerrain3D_()
{
#if !defined(_CONSOLE_GAME)
	return new Terrain3D_Imp(this);
#endif

	return nullptr;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* Graphics_Imp::CreateEffect_(const achar* path)
{
	auto ret = EffectContainer->TryLoad(path, [this, path](uint8_t* data, int32_t size) -> Effect_Imp* {
		EFK_CHAR parentDir[512];
		GetParentDir(parentDir, (const EFK_CHAR*)path);

		auto effect = Effekseer::Effect::Create(m_effectSetting, data, size, 1.0, parentDir);
		if (effect == nullptr)
			return nullptr;
		return Effect_Imp::CreateEffect(this, effect);
	});

	return ret;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Font* Graphics_Imp::CreateFont_(const achar* path)
{
	auto ret = FontContainer->TryLoadWithVector(path, [this, &path](const std::vector<uint8_t>& data) -> Font_Imp* {
		Font_Imp* font = new Font_Imp(this, path, data);
		return font;
	});

	return ret;
}

Font* Graphics_Imp::CreateDynamicFont_(const achar* font, int32_t fontSize, Color color, int32_t outlineSize, Color outlineColor)
{
	auto space = asd::astring(ToAString(" "));

	auto key = asd::astring(font) + space + ToAString(std::to_string(fontSize).c_str()) + space +
			   ToAString(std::to_string(color.R).c_str()) + space + ToAString(std::to_string(color.G).c_str()) + space +
			   ToAString(std::to_string(color.B).c_str()) + space + ToAString(std::to_string(color.A).c_str()) + space +
			   ToAString(std::to_string(outlineSize).c_str()) + space + ToAString(std::to_string(outlineColor.R).c_str()) + space +
			   ToAString(std::to_string(outlineColor.G).c_str()) + space + ToAString(std::to_string(outlineColor.B).c_str()) + space +
			   ToAString(std::to_string(outlineColor.A).c_str());

	auto extFont = FontContainer->Get(key.c_str());
	if (extFont != nullptr)
	{
		SafeAddRef(extFont);
		return extFont;
	}

	auto font_ = Font_Imp::Create(this, font, fontSize, color, outlineSize, outlineColor);

	FontContainer->Register(key.c_str(), font_);

	return font_;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

ImagePackage* Graphics_Imp::CreateImagePackage_(const achar* path)
{
	auto ret = ImagePackageContainer->TryLoadWithVector(path, [this, &path](const std::vector<uint8_t>& data) -> ImagePackage_Imp* {
		auto ip = ImagePackage_Imp::Create(this, data);
		return ip;
	});

	return ret;
}

MediaPlayer* Graphics_Imp::CreateMediaPlayer_()
{
#if _WIN32
	return new MediaPlayerWMF(this);
#endif

#ifdef __APPLE__
	return new MediaPlayerAVF(this);
#endif

	return nullptr;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::SetVertexBuffer(VertexBuffer_Imp* vertexBuffer)
{
	SafeAddRef(vertexBuffer);
	SafeRelease(m_vertexBufferPtr);
	m_vertexBufferPtr = vertexBuffer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::SetIndexBuffer(IndexBuffer_Imp* indexBuffer)
{
	SafeAddRef(indexBuffer);
	SafeRelease(m_indexBufferPtr);
	m_indexBufferPtr = indexBuffer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::SetShader(NativeShader_Imp* shader)
{
	SafeAddRef(shader);
	SafeRelease(m_shaderPtr);
	m_shaderPtr = shader;
}

void Graphics_Imp::SetRenderState(const RenderState& renderState) { nextState.renderState = renderState; }

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::DrawPolygon(int32_t count)
{
	assert(m_vertexBufferPtr != nullptr);
	assert(m_indexBufferPtr != nullptr);
	assert(m_shaderPtr != nullptr);

	CommitRenderState(false);

	DrawPolygonInternal(count, m_vertexBufferPtr, m_indexBufferPtr, m_shaderPtr);

	drawCallCountCurrent++;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::DrawPolygon(int32_t offset, int32_t count)
{
	assert(m_vertexBufferPtr != nullptr);
	assert(m_indexBufferPtr != nullptr);
	assert(m_shaderPtr != nullptr);

	CommitRenderState(false);

	DrawPolygonInternal(offset, count, m_vertexBufferPtr, m_indexBufferPtr, m_shaderPtr);

	drawCallCountCurrent++;
}

void Graphics_Imp::DrawPolygonInstanced(int32_t count, int32_t instanceCount)
{
	assert(m_vertexBufferPtr != nullptr);
	assert(m_indexBufferPtr != nullptr);
	assert(m_shaderPtr != nullptr);

	CommitRenderState(false);

	DrawPolygonInstancedInternal(count, m_vertexBufferPtr, m_indexBufferPtr, m_shaderPtr, instanceCount);

	drawCallCountCurrent++;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::Begin()
{
	drawCallCount = drawCallCountCurrent;
	drawCallCountCurrent = 0;

	CommitRenderState(true);

	ResetDrawState();

	BeginInternal();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::End()
{
	ResetDrawState();

	EndInternal();
}

void Graphics_Imp::Reload()
{
	Texture2DContainer->Reload([this](std::shared_ptr<ResourceContainer<Texture2D_Imp>::LoadingInformation> o,
									  uint8_t* data,
									  int32_t size) -> void { o->ResourcePtr->Reload(data, size); });

	EffectContainer->Reload([this](std::shared_ptr<ResourceContainer<Effect_Imp>::LoadingInformation> o,
								   uint8_t* data,
								   int32_t size) -> void { o->ResourcePtr->Reload(o->LoadedPath.c_str(), m_effectSetting, data, size); });

	ModelContainer->ReloadWithVector(
		[this](std::shared_ptr<ResourceContainer<Model_Imp>::LoadingInformation> o, const std::vector<uint8_t>& data) -> void {
			o->ResourcePtr->Reload(data, o->LoadedPath.c_str());
		});

	FontContainer->ReloadWithVector(
		[this](std::shared_ptr<ResourceContainer<Font_Imp>::LoadingInformation> o, const std::vector<uint8_t>& data) -> void {
			o->ResourcePtr->Reload(o->LoadedPath.c_str(), data);
		});

	for (auto& r : EffectContainer->GetAllResources())
	{
		auto e = r.second;
		e->ReloadResources(r.first.c_str());
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

} // namespace asd

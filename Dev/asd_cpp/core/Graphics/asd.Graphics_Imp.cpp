
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <vector>
#include "asd.Graphics_Imp.h"
#include "asd.RenderingThread.h"

#include "../Log/asd.Log.h"
#include "../IO/asd.File_Imp.h"

#include "Resource/asd.VertexBuffer_Imp.h"
#include "Resource/asd.IndexBuffer_Imp.h"
#include "Resource/asd.NativeShader_Imp.h"
#include "Resource/asd.Effect_Imp.h"

#include "Resource/asd.Shader2D_Imp.h"
#include "Resource/asd.Material2D_Imp.h"

#include "Resource/asd.Shader3D_Imp.h"
#include "Resource/asd.Material3D_Imp.h"

#include "Resource/asd.Effect_Imp.h"

#include "Resource/asd.ShaderCache.h"
#include "Resource/asd.MaterialPropertyBlock_Imp.h"

#include "Resource/asd.ImagePackage_Imp.h"

#include "3D/Resource/asd.Mesh_Imp.h"
#include "3D/Resource/asd.Deformer_Imp.h"
#include "3D/Resource/asd.Model_Imp.h"
#include "3D/Resource/asd.MassModel_Imp.h"
#include "3D/Resource/asd.Terrain3D_Imp.h"

#include "3D/Resource/Animation/asd.AnimationClip.h"
#include "3D/Resource/Animation/asd.AnimationSource.h"

#include <Graphics/3D/asd.Model_IO.h>
#include <Graphics/3D/asd.MassModel_IO.h>

#if _WIN32
#include "Platform/DX11/asd.Graphics_Imp_DX11.h"
#endif

#include "Platform/GL/asd.Graphics_Imp_GL.h"

#define Z_SOLO
#include <png.h>
#include <pngstruct.h>
#include <pnginfo.h>

#if _WIN32
#include <Windows.h>
#include <GdiPlus.h>
#include <Gdiplusinit.h>
#pragma comment( lib, "gdiplus.lib" )
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#if _WIN32
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
	mkdir(shaderCacheDirectory,
		S_IRUSR | S_IWUSR | S_IXUSR |
		S_IRGRP | S_IWGRP | S_IXGRP |
		S_IROTH | S_IXOTH | S_IXOTH);
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {

	class EffectLoader
		: public Effekseer::EffectLoader
	{
	private:
		File*	file = nullptr;
	public:

		EffectLoader(File* file)
			: file(file)
		{
		}

		bool Load(const EFK_CHAR* path, void*& data, int32_t& size)
		{
			auto sf = file->CreateStaticFile((const achar*) path);
			if (sf.get() == nullptr) return false;

			size = sf->GetSize();
			data = new uint8_t[size];
			memcpy(data, sf->GetData(), size);
			return true;
		}

		void Unload(void* data, int32_t size)
		{
			auto d = (uint8_t*) data;
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
	}

	void ImageHelper::SaveJPGImage(const achar* filepath, int32_t width, int32_t height, void* data, bool rev)
	{
#if _WIN32
		//GDI+を初期化する。
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		{
			// bmpを作成する。
			Gdiplus::Bitmap bmp(width, height);

			auto p = (Color*) data;
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

			auto imageCodecInfo = (Gdiplus::ImageCodecInfo*) malloc(encoderSize);
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

			bmp.Save(filepath, &id);
		}

		//GDI+を終了する。
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
	auto d = (uint8_t**) png_get_io_ptr(png_ptr);

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
	FILE *fp = _wfopen(filepath, L"wb");
#else
	FILE *fp = fopen(ToUtf8String(filepath).c_str(), "wb");
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
	std::vector<png_byte>  raw1D(height * png_get_rowbytes(pp, ip));
	std::vector<png_bytep> raw2D(height * sizeof(png_bytep));
	for (int32_t i = 0; i < height; i++)
	{
		raw2D[i] = &raw1D[i*png_get_rowbytes(pp, ip)];
	}

	memcpy((void*) raw1D.data(), data, width * height * 4);

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

	/* 開放 */
	png_destroy_write_struct(&pp, &ip);
	fclose(fp);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool ImageHelper::LoadPNGImage(void* data, int32_t size, bool rev, int32_t& imagewidth, int32_t& imageheight, std::vector<uint8_t>& imagedst, Log* log)
{
	imagewidth = 0;
	imageheight = 0;
	imagedst.clear();

	uint8_t* data_ = (uint8_t*) data;

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
			log->WriteLineStrongly("pngファイルのヘッダの読み込みに失敗しました。");
		}
		
		return false;
	}

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
	if (format == TextureFormat::R8G8B8A8_UNORM) return 4;
	if (format == TextureFormat::R16G16B16A16_FLOAT) return 2 * 4;
	if (format == TextureFormat::R32G32B32A32_FLOAT) return 4 * 4;
	if (format == TextureFormat::R8G8B8A8_UNORM_SRGB) return 4;
	if (format == TextureFormat::R16G16_FLOAT) return 2 * 2;
	if (format == TextureFormat::R8_UNORM) return 1;

	// 1ピクセル単位で返せない
	if (format == TextureFormat::BC1) return 0;
	if (format == TextureFormat::BC1_SRGB) return 0;
	if (format == TextureFormat::BC2) return 0;
	if (format == TextureFormat::BC2_SRGB) return 0;
	if (format == TextureFormat::BC3) return 0;
	if (format == TextureFormat::BC3_SRGB) return 0;

	return 0;
}

int32_t ImageHelper::GetVRAMSize(TextureFormat format, int32_t width, int32_t height)
{
	auto pitch = GetPitch(format);

	if (pitch == 0)
	{
		if (format == TextureFormat::BC1 ||
			format == TextureFormat::BC1_SRGB) return width * height * 4 / 6;
		
		if (format == TextureFormat::BC2 ||
			format == TextureFormat::BC2_SRGB) return width * height * 4 / 4;

		if (format == TextureFormat::BC3 ||
			format == TextureFormat::BC3_SRGB) return width * height * 4 / 4;
	}

	return pitch * width * height;
}

int32_t ImageHelper::GetMipmapCount(int32_t width, int32_t height)
{
	auto ret = 1;
	while (width != 1 || height != 1)
	{
		if (width > 1) width = width >> 1;
		if (height > 1) height = height >> 1;
		ret++;
	}

	return ret;
}

void ImageHelper::GetMipmapSize(int mipmap, int32_t& width, int32_t& height)
{
	for (auto i = 0; i < mipmap; i++)
	{
		if (width > 1) width = width >> 1;
		if (height > 1) height = height >> 1;
	}
}


bool ImageHelper::IsPNG(const void* data, int32_t size)
{
	if (size < 4) return false;

	auto d = (uint8_t*) data;

	if (d[0] != 0x89) return false;
	if (d[1] != 'P') return false;
	if (d[2] != 'N') return false;
	if (d[3] != 'G') return false;

	return true;
}

bool ImageHelper::IsDDS(const void* data, int32_t size)
{
	if (size < 4) return false;

	auto d = (uint8_t*) data;

	if (d[0] != 'D') return false;
	if (d[1] != 'D') return false;
	if (d[2] != 'S') return false;

	return true;
}

std::string GraphicsHelper::GetFormatName(Graphics_Imp* graphics, TextureFormat format)
{
	if (format == TextureFormat::R8_UNORM) return std::string("R8_UNORM");
	if (format == TextureFormat::R16G16B16A16_FLOAT) return std::string("R16G16B16A16_FLOAT");
	if (format == TextureFormat::R32G32B32A32_FLOAT) return std::string("R32G32B32A32_FLOAT");
	if (format == TextureFormat::R8G8B8A8_UNORM) return std::string("R8G8B8A8_UNORM");
	if (format == TextureFormat::R8G8B8A8_UNORM_SRGB) return std::string("R8G8B8A8_UNORM_SRGB");
	if (format == TextureFormat::R16G16_FLOAT) return std::string("R16G16_FLOAT");
	return std::string("Unknown");
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectTextureLoader::EffectTextureLoader(Graphics_Imp* graphics)
	: m_graphics(graphics)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
EffectTextureLoader::~EffectTextureLoader()
{
	assert(m_caches.size() == 0);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void* EffectTextureLoader::Load(const EFK_CHAR* path, Effekseer::TextureType textureType)
{
	auto key = astring((const achar*) path);
	auto cache = m_caches.find(key);
	if (cache != m_caches.end())
	{
		cache->second.Count++;
		return cache->second.Ptr;
	}

	auto staticFile = m_graphics->GetFile()->CreateStaticFile((const achar*) path);
	if (staticFile.get() == nullptr) return nullptr;

	int32_t imageWidth = 0;
	int32_t imageHeight = 0;
	std::vector<uint8_t> imageDst;
	if (!ImageHelper::LoadPNGImage(staticFile->GetData(), staticFile->GetSize(), IsReversed(), imageWidth, imageHeight, imageDst, m_graphics->GetLog()))
	{
		return nullptr;
	}

	void* img = InternalLoad(m_graphics, imageDst, imageWidth, imageHeight);

	Cache c;
	c.Ptr = img;
	c.Count = 1;
	c.Width = imageWidth;
	c.Height = imageHeight;
	m_caches[key] = c;
	dataToKey[img] = key;

	m_graphics->IncVRAM(ImageHelper::GetVRAMSize(TextureFormat::R8G8B8A8_UNORM, imageWidth, imageHeight));

	return img;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void EffectTextureLoader::Unload(void* data)
{
	if (data == nullptr) return;

	auto key = dataToKey[data];
	auto cache = m_caches.find(key);
	cache->second.Count--;

	if (cache->second.Count == 0)
	{
		InternalUnload(data);

		m_graphics->DecVRAM(ImageHelper::GetVRAMSize(TextureFormat::R8G8B8A8_UNORM, cache->second.Width, cache->second.Height));

		m_caches.erase(key);
		dataToKey.erase(data);
	}
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
	auto self_ = (Graphics_Imp*) self;
	self_->StartRenderingThread();
}

void Graphics_Imp::StartRenderingThread()
{

}

void Graphics_Imp::EndRenderingThreadFunc(void* self)
{
	auto self_ = (Graphics_Imp*) self;
	self_->EndRenderingThread();
}

void Graphics_Imp::EndRenderingThread()
{

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp* Graphics_Imp::Create(Window* window, GraphicsDeviceType graphicsDevice, Log* log,File* file, bool isReloadingEnabled, bool isFullScreen)
{
#if _WIN32
	if (graphicsDevice == GraphicsDeviceType::OpenGL)
	{
		return Graphics_Imp_GL::Create(window, log, file,isReloadingEnabled, isFullScreen);
	}
	else
	{
		return Graphics_Imp_DX11::Create(window, log, file,isReloadingEnabled, isFullScreen);
	}
#else
	if (graphicsDevice == GraphicsDeviceType::OpenGL)
	{
		return Graphics_Imp_GL::Create(window, log, file, isReloadingEnabled, isFullScreen);
	}
	else
	{
		return nullptr;
	}
#endif
	return nullptr;
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp* Graphics_Imp::Create(void* handle1, void* handle2, int32_t width, int32_t height, GraphicsDeviceType graphicsDevice, Log* log,File* file, bool isReloadingEnabled, bool isFullScreen)
{
#if _WIN32
	if (graphicsDevice == GraphicsDeviceType::OpenGL)
	{
		return Graphics_Imp_DX11::Create((HWND) handle1, width, height, log, file,isReloadingEnabled, isFullScreen);
	}
	else
	{
		return Graphics_Imp_DX11::Create((HWND) handle1, width, height, log, file,isReloadingEnabled, isFullScreen);
	}
#elif __APPLE__
	return nullptr; // not supported
#else
	return Graphics_Imp_GL::Create_X11(handle1, handle2, width, height, log, file,isReloadingEnabled, isFullScreen);
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp::Graphics_Imp(Vector2DI size, Log* log, File* file, bool isReloadingEnabled, bool isFullScreen)
	: m_size(size)
	, m_vertexBufferPtr(nullptr)
	, m_indexBufferPtr(nullptr)
	, m_shaderPtr(nullptr)
	, m_log(log)
	, m_file(file)
{
	CreateShaderCacheDirectory();

	Texture2DContainer = std::make_shared<ResourceContainer<Texture2D_Imp>>(file);
	EffectContainer = std::make_shared<ResourceContainer<Effect_Imp>>(file);
	FontContainer = std::make_shared<ResourceContainer<Font_Imp>>(file);
	ModelContainer = std::make_shared<ResourceContainer<Model_Imp>>(file);
	ImagePackageContainer = std::make_shared<ResourceContainer<ImagePackage_Imp>>(file);

	//SafeAddRef(m_log);
	//m_resourceContainer = new GraphicsResourceContainer(m_file);
	m_renderingThread = std::make_shared<RenderingThread>();

	m_effectSetting = Effekseer::Setting::Create();
	m_effectSetting->SetCoordinateSystem(Effekseer::CoordinateSystem::RH);
	m_effectSetting->SetEffectLoader(new EffectLoader(file));

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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp::~Graphics_Imp()
{
	SafeDelete(m_shaderCache);

	SafeRelease(m_vertexBufferPtr);
	SafeRelease(m_indexBufferPtr);
	SafeRelease(m_shaderPtr);

	//SafeDelete(m_resourceContainer);

	SafeRelease(m_effectSetting);
	//SafeRelease(m_log);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Texture2D_Imp* Graphics_Imp::CreateTexture2D_Imp(const achar* path)
{
	auto ret = Texture2DContainer->TryLoad(path, [this](uint8_t* data, int32_t size) -> Texture2D_Imp*
	{
		return CreateTexture2D_Imp_Internal(this, data, size);
	});

	return ret;
}

Texture2D_Imp* Graphics_Imp::CreateTexture2DAsRawData_Imp(const achar* path)
{
	auto ret = Texture2DContainer->TryLoad(path, [this](uint8_t* data, int32_t size) -> Texture2D_Imp*
	{
		return CreateTexture2DAsRawData_Imp_Internal(this, data, size);
	});

	return ret;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Texture2D_Imp* Graphics_Imp::CreateEmptyTexture2D_Imp(int32_t width, int32_t height, TextureFormat format)
{
	return CreateEmptyTexture2D_Imp_Internal(this, width, height, format, nullptr);
}


Texture2D_Imp* Graphics_Imp::CreateTexture2DWithRawData(int32_t width, int32_t height, TextureFormat format, void* data)
{
	return CreateEmptyTexture2D_Imp_Internal(this, width, height, format, data);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Shader2D* Graphics_Imp::CreateShader2D_(const achar* shaderText)
{
	auto shader = Shader2D_Imp::Create(this,shaderText,ToAString("").c_str(),m_log);

	return shader;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Material2D* Graphics_Imp::CreateMaterial2D_(Shader2D* shader)
{
	auto material = Material2D_Imp::Create((Shader2D_Imp*) shader);
	auto material2d = (Material2D*) material;
	return material2d;
}

Shader3D* Graphics_Imp::CreateShader3D_(const achar* shaderText)
{
	auto shader = Shader3D_Imp::Create(this, shaderText, ToAString("").c_str(), m_log);
	return shader;
}

Material3D* Graphics_Imp::CreateMaterial3D_(Shader3D* shader)
{
	auto material = Material3D_Imp::Create((Shader3D_Imp*) shader);
	auto material3d = (Material3D*) material;
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
Mesh* Graphics_Imp::CreateMesh_()
{
	return Mesh_Imp::Create(this);
}

Deformer* Graphics_Imp::CreateDeformer_()
{
	return new Deformer_Imp();
}

Model* Graphics_Imp::CreateModel_(const achar* path)
{
	auto ret = ModelContainer->TryLoadWithVector(path, [this, &path](const std::vector<uint8_t>& data) -> Model_Imp*
	{
		auto model = new Model_Imp(this);
		model->Load(this, data, path);
		return model;
	});
	
	return ret;
}

MassModel* Graphics_Imp::CreateMassModelFromModelFile_(const achar* path)
{
	auto staticFile = GetFile()->CreateStaticFile(path);
	if (staticFile.get() == nullptr) return nullptr;

	std::vector<uint8_t> data;
	data.resize(staticFile->GetSize());

	memcpy(data.data(), staticFile->GetData(), staticFile->GetSize());

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
	if (staticFile.get() == nullptr) return nullptr;

	std::vector<uint8_t> data;
	data.resize(staticFile->GetSize());

	memcpy(data.data(), staticFile->GetData(), staticFile->GetSize());

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
	return new Terrain3D_Imp(this);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* Graphics_Imp::CreateEffect_(const achar* path)
{
	auto ret = EffectContainer->TryLoad(path, [this,path](uint8_t* data, int32_t size) -> Effect_Imp*
	{
		EFK_CHAR parentDir[512];	
		GetParentDir(parentDir, (const EFK_CHAR*) path);
	
		auto effect = Effekseer::Effect::Create(m_effectSetting, data, size, 1.0, parentDir);
		if (effect == nullptr) return nullptr;
		return Effect_Imp::CreateEffect(this, effect);
	});

	return ret;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Font* Graphics_Imp::CreateFont_(const achar* path)
{
	auto ret = FontContainer->TryLoadWithVector(path, [this, &path](const std::vector<uint8_t>& data) -> Font_Imp*
	{
		Font_Imp* font = new Font_Imp(this, path, data);
		return font;
	});
	
	return ret;
}

Font* Graphics_Imp::CreateDynamicFont_(const achar* font, int32_t fontSize, Color color, int32_t outlineSize, Color outlineColor)
{
	auto space = asd::astring(ToAString(" "));

	auto key = asd::astring(font) + space +
		ToAString(std::to_string(fontSize).c_str()) + space +
		ToAString(std::to_string(color.R).c_str()) + space +
		ToAString(std::to_string(color.G).c_str()) + space +
		ToAString(std::to_string(color.B).c_str()) + space +
		ToAString(std::to_string(color.A).c_str()) + space +
		ToAString(std::to_string(outlineSize).c_str()) + space +
		ToAString(std::to_string(outlineColor.R).c_str()) + space +
		ToAString(std::to_string(outlineColor.G).c_str()) + space +
		ToAString(std::to_string(outlineColor.B).c_str()) + space +
		ToAString(std::to_string( outlineColor.A).c_str());

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
	auto ret = ImagePackageContainer->TryLoadWithVector(path, [this, &path](const std::vector<uint8_t>& data) -> ImagePackage_Imp*
	{
		auto ip = ImagePackage_Imp::Create(this, data); 
		return ip;
	});

	return ret;
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

void Graphics_Imp::SetRenderState(const RenderState& renderState)
{
	nextState.renderState = renderState;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::DrawPolygon(int32_t count)
{
	assert(m_vertexBufferPtr != nullptr);
	assert(m_indexBufferPtr != nullptr);
	assert(m_shaderPtr != nullptr);

	CommitRenderState(false);

	DrawPolygonInternal(
		count, 
		m_vertexBufferPtr,
		m_indexBufferPtr,
		m_shaderPtr);

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

	DrawPolygonInternal(
		offset,
		count,
		m_vertexBufferPtr,
		m_indexBufferPtr,
		m_shaderPtr);

	drawCallCountCurrent++;
}

void Graphics_Imp::DrawPolygonInstanced(int32_t count, int32_t instanceCount)
{
	assert(m_vertexBufferPtr != nullptr);
	assert(m_indexBufferPtr != nullptr);
	assert(m_shaderPtr != nullptr);

	CommitRenderState(false);

	DrawPolygonInstancedInternal(
		count,
		m_vertexBufferPtr,
		m_indexBufferPtr,
		m_shaderPtr,
		instanceCount);

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
	Texture2DContainer->Reload([this](std::shared_ptr<ResourceContainer<Texture2D_Imp>::LoadingInformation> o, uint8_t* data, int32_t size) -> void
	{
		o->ResourcePtr->Reload(data, size);
	});

	EffectContainer->Reload([this](std::shared_ptr<ResourceContainer<Effect_Imp>::LoadingInformation> o, uint8_t* data, int32_t size) -> void
	{
		o->ResourcePtr->Reload(o->LoadedPath.c_str(), m_effectSetting, data, size);
	});

	ModelContainer->ReloadWithVector([this](std::shared_ptr<ResourceContainer<Model_Imp>::LoadingInformation> o, const std::vector<uint8_t>& data) -> void
	{
		o->ResourcePtr->Reload(data, o->LoadedPath.c_str());
	});

	FontContainer->ReloadWithVector([this](std::shared_ptr<ResourceContainer<Font_Imp>::LoadingInformation> o, const std::vector<uint8_t>& data) -> void
	{
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

}

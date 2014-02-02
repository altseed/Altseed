
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Graphics_Imp.h"
#include "ace.RenderingThread.h"

#include "../../Log/ace.Log.h"

#include "Resource/ace.VertexBuffer_Imp.h"
#include "Resource/ace.IndexBuffer_Imp.h"
#include "Resource/ace.NativeShader_Imp.h"
#include "Resource/ace.Effect_Imp.h"

#include "Resource/ace.RenderState_Imp.h"

#include "Resource/ace.Shader2D_Imp.h"
#include "Resource/ace.Material2D_Imp.h"

#include "Resource/ace.Effect_Imp.h"

#include "3D/ace.Mesh_Imp.h"
#include "3D/ace.Deformer_Imp.h"
#include "3D/ace.Model_Imp.h"

#if _WIN32
#include "../DX11/ace.Graphics_Imp_DX11.h"
#endif

#include "../GL/ace.Graphics_Imp_GL.h"

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

	class EffectLoader
		: public Effekseer::EffectLoader
	{
	public:
		bool Load(const EFK_CHAR* path, void*& data, int32_t& size)
		{
#if _WIN32
			auto fp = _wfopen((const achar*)path, L"rb");
			if (fp == nullptr) return false;
#else
			auto fp = fopen(ToUtf8String((const achar*)path).c_str(), "rb");
			if (fp == nullptr) return false;
#endif
			fseek(fp, 0, SEEK_END);
			size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			data = new uint8_t[size];
			fread(data, 1, size, fp);
			fclose(fp);

			return true;
		}

		void Unload(void* data, int32_t size)
		{
			SafeDeleteArray(data);
		}
	};

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
bool ImageHelper::LoadPNGImage(void* data, int32_t size, bool rev, int32_t& imagewidth, int32_t& imageheight, void*& imagedst)
{
	imagewidth = 0;
	imageheight = 0;
	imagedst = nullptr;

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
		pixelBytes = 4;
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

	imagewidth = png_info->width;
	imageheight = png_info->height;
	uint8_t* imagedst_ = new uint8_t[imagewidth * imageheight * 4];

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

	imagedst = imagedst_;

	delete[] image;
	png_destroy_read_struct(&png, &png_info, NULL);

	return true;
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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::SavePNGImage(const achar* filepath, int32_t width, int32_t height, void* data, bool rev)
{
	png_bytep raw1D;
	png_bytepp raw2D;

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
	raw1D = (png_bytep) malloc(height * png_get_rowbytes(pp, ip));
	raw2D = (png_bytepp) malloc(height * sizeof(png_bytep));
	for (int32_t i = 0; i < height; i++)
	{
		raw2D[i] = &raw1D[i*png_get_rowbytes(pp, ip)];
	}

	memcpy((void*) raw1D, data, width * height * 4);

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
	
	/* 開放 */
	png_destroy_write_struct(&pp, &ip);
	fclose(fp);
	free(raw1D);
	free(raw2D);
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
Shader2D* Graphics_Imp::CreateShader2D_(
	const achar* shaderText,
	ShaderVariableProperty* variableProperties,
	int32_t variablePropertiesCount)
{
	std::vector <ShaderVariableProperty> pProp;

	for (int32_t i = 0; i < variablePropertiesCount; i++)
	{
		pProp.push_back(variableProperties[i]);
	}

	return CreateShader2D_Imp(
		shaderText,
		pProp);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp* Graphics_Imp::Create(Window* window, bool isOpenGLMode, Log* log, bool isMultithreadingMode)
{
#if _WIN32
	if (isOpenGLMode)
	{
		return Graphics_Imp_GL::Create(window, log, isMultithreadingMode);
	}
	else
	{
		return Graphics_Imp_DX11::Create(window, log, isMultithreadingMode);
	}
#else
	if (isOpenGLMode)
	{
		return Graphics_Imp_GL::Create(window, log, isMultithreadingMode);
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
Graphics_Imp* Graphics_Imp::Create(void* handle1, void* handle2, int32_t width, int32_t height, bool isOpenGLMode, Log* log, bool isMultithreadingMode)
{
#if _WIN32
	if (isOpenGLMode)
	{
		return Graphics_Imp_DX11::Create((HWND) handle1, width, height, log, isMultithreadingMode);
	}
	else
	{
		return Graphics_Imp_DX11::Create((HWND) handle1, width, height, log, isMultithreadingMode);
	}
#else 
	return Graphics_Imp_GL::Create_X11(handle1, handle2, width, height, log, isMultithreadingMode);
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp::Graphics_Imp(Vector2DI size, Log* log, bool isMultithreadingMode)
	: m_size(size)
	, m_renderState(nullptr)
	, m_vertexBufferPtr(nullptr)
	, m_indexBufferPtr(nullptr)
	, m_shaderPtr(nullptr)
	, m_log(log)
	, m_isMultithreadingMode(isMultithreadingMode)
{
	//SafeAddRef(m_log);
	m_resourceContainer = new GraphicsResourceContainer();

	if (IsMultithreadingMode())
	{
		m_renderingThread = std::make_shared<RenderingThread>();
	}

	m_effectSetting = Effekseer::Setting::Create();
	m_effectSetting->SetCoordinateSystem(Effekseer::eCoordinateSystem::COORDINATE_SYSTEM_RH);
	m_effectSetting->SetEffectLoader(new EffectLoader());

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp::~Graphics_Imp()
{
	SafeRelease(m_vertexBufferPtr);
	SafeRelease(m_indexBufferPtr);
	SafeRelease(m_shaderPtr);
	SafeDelete(m_renderState);

	SafeDelete(m_resourceContainer);

	SafeRelease(m_effectSetting);
	//SafeRelease(m_log);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Texture2D_Imp* Graphics_Imp::CreateTexture2D_Imp(const achar* path)
{
	{
		auto existing = GetResourceContainer()->Texture2Ds.Get(path);
		if (existing != nullptr)
		{
			SafeAddRef(existing);
			return existing;
		}
	}

#if _WIN32
	auto fp = _wfopen(path, L"rb");
	if (fp == nullptr) return nullptr;
#else
	auto fp = fopen(ToUtf8String(path).c_str(), "rb");
	if (fp == nullptr) return nullptr;
#endif
	fseek(fp, 0, SEEK_END);
	auto size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	auto data = new uint8_t[size];
	fread(data, 1, size, fp);
	fclose(fp);

	auto texture = CreateTexture2D_Imp_Internal(this, data, size);
	if (texture == nullptr)
	{
		SafeDeleteArray(data);
		return nullptr;
	}

	SafeDeleteArray(data);

	std::shared_ptr<Texture2DReloadInformation> info;
	info.reset( new Texture2DReloadInformation() );
	info->ModifiedTime = GetResourceContainer()->GetModifiedTime(path);
	info->Path = path;
	
	GetResourceContainer()->Texture2Ds.Regist(path, info, texture);
	return texture;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Shader2D_Imp* Graphics_Imp::CreateShader2D_Imp(
	const achar* shaderText,
	std::vector <ShaderVariableProperty>& variableProperties)
{
	auto shader = Shader2D_Imp::Create(
		this,
		shaderText,
		ToAString("").c_str(),
		variableProperties,
		m_log
		);

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
	{
		auto existing = GetResourceContainer()->Models.Get(path);
		if (existing != nullptr)
		{
			SafeAddRef(existing);
			return existing;
		}
	}

#if _WIN32
	auto fp = _wfopen(path, L"rb");
	if (fp == nullptr) return nullptr;
#else
	auto fp = fopen(ToUtf8String(path).c_str(), "rb");
	if (fp == nullptr) return nullptr;
#endif
	fseek(fp, 0, SEEK_END);
	auto size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	std::vector<uint8_t> data;
	data.resize(size);

	fread(&(data[0]), 1, size, fp);
	fclose(fp);

	auto model = new Model_Imp(this);
	model->Load(this, data, path);

	std::shared_ptr<ModelReloadInformation> info;
	info.reset(new ModelReloadInformation());
	info->ModifiedTime = GetResourceContainer()->GetModifiedTime(path);
	info->Path = path;

	GetResourceContainer()->Models.Regist(path, info, model);
	
	return model;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effect* Graphics_Imp::CreateEffect_(const achar* path)
{
	auto effect = Effekseer::Effect::Create(m_effectSetting, (const EFK_CHAR*)path, 1.0f);
	if (effect == nullptr) return nullptr;

	auto ret = Effect_Imp::CreateEffect(this, effect);
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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::DrawPolygon(int32_t count)
{
	assert(m_vertexBufferPtr != nullptr);
	assert(m_indexBufferPtr != nullptr);
	assert(m_shaderPtr != nullptr);

	m_renderState->Update(false);

	DrawPolygonInternal(
		count, 
		m_vertexBufferPtr,
		m_indexBufferPtr,
		m_shaderPtr);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Graphics_Imp::Begin()
{
	GetRenderState()->GetActiveState().Reset();
	GetRenderState()->Update(true);

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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}

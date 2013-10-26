
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Graphics_Imp.h"

#include "../../Log/ace.Log.h"

#include "Resource/ace.VertexBuffer_Imp.h"
#include "Resource/ace.IndexBuffer_Imp.h"
#include "Resource/ace.NativeShader_Imp.h"

#include "Resource/ace.RenderState_Imp.h"

#include "Resource/ace.Shader2D_Imp.h"
#include "Resource/ace.Material2D_Imp.h"


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
Material2D* Graphics_Imp::CreateMaterial2D_(Shader2D* shader)
{
	auto material = CreateMaterial2D_Imp((Shader2D_Imp*) shader);
	auto material2d = (Material2D*) material;
	return material2d;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp* Graphics_Imp::Create(Window* window, bool isOpenGLMode, Log* log)
{
#if _WIN32
	if (isOpenGLMode)
	{
		return Graphics_Imp_GL::Create(window, log);
	}
	else
	{
		return Graphics_Imp_DX11::Create(window, log);
	}
#else
	if (isOpenGLMode)
	{
		return Graphics_Imp_GL::Create(window, log);
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
Graphics_Imp* Graphics_Imp::Create(void* handle1, void* handle2, int32_t width, int32_t height, bool isOpenGLMode, Log* log)
{
#if _WIN32
	if (isOpenGLMode)
	{
		return Graphics_Imp_DX11::Create((HWND) handle1, width, height, log);
	}
	else
	{
		return Graphics_Imp_DX11::Create((HWND) handle1, width, height, log);
	}
#else 
	return Graphics_Imp_GL::Create_X11(handle1, handle2, width, height, log);
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Graphics_Imp::Graphics_Imp(Vector2DI size, Log* log)
	: m_size(size)
	, m_renderState(nullptr)
	, m_vertexBufferPtr(nullptr)
	, m_indexBufferPtr(nullptr)
	, m_shaderPtr(nullptr)
	, m_log(log)
{
	//SafeAddRef(m_log);
	m_resourceContainer = new GraphicsResourceContainer();
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

	//SafeRelease(m_log);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Texture2D_Imp* Graphics_Imp::CreateTexture2D_Imp(const achar* path)
{
	{
		auto existing = GetResourceContainer()->GetTexture2D(path);
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
	
	GetResourceContainer()->RegistTexture2D(path, info, texture);
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
Material2D_Imp* Graphics_Imp::CreateMaterial2D_Imp(Shader2D_Imp* shader)
{
	return Material2D_Imp::Create(shader);
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
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}

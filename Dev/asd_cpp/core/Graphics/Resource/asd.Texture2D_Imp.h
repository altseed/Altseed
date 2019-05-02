#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <Math/asd.Vector2DI.h>

#include "../../Utils/IAsyncResource.h"
#include "../asd.DeviceObject.h"
#include "asd.Texture2D.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Texture2D_Imp : public Texture2D, public DeviceObject, public IAsyncResource
{
protected:
	/**
		@brief	内部のバッファに画像ファイルから画像を展開する。
		@param	data	ファイルデータのポインタ
		@param	size	ファイルのデータサイズ
		@param	rev	上限反転で読み込む
	*/
	bool InternalLoad(void* data, int32_t size, bool rev);

	/**
		@brief	内部に展開された画像を解放する。
	*/
	void InternalUnload();

	std::vector<uint8_t> m_internalTextureData;
	int32_t m_internalTextureWidth;
	int32_t m_internalTextureHeight;

	Vector2DI m_size;

	TextureFormat m_format;
	LoadState m_loadState;

	ar::Texture2D* rhi = nullptr;
	std::vector<uint8_t> m_resource;

	Texture2D_Imp(Graphics* graphics);
	Texture2D_Imp(Graphics* graphics, ar::Texture2D* rhi, Vector2DI size, TextureFormat format);

	virtual ~Texture2D_Imp();

public:
#if !SWIG
	static Texture2D_Imp* Create(Graphics_Imp* graphics);

	static Texture2D_Imp* Create(Graphics_Imp* graphics, uint8_t* data, int32_t size, bool isEditable, bool isSRGB);

	static Texture2D_Imp* Create(Graphics_Imp* graphics, int32_t width, int32_t height, TextureFormat format, void* data);

	bool Load(uint8_t* data, int32_t size, bool isEditable, bool isSRGB);
#endif
	bool Save(const achar* path) override;

	bool Lock(TextureLockInfomation* info) override;

	void Unlock() override;

	Vector2DI GetSize() const { return m_size; }
	TextureFormat GetFormat() const override { return m_format; }
	LoadState GetLoadStateInternal() const override { return m_loadState; }
	LoadState GetLoadState() const override { return m_loadState; }
#if !SWIG
	ar::Texture2D* GetRHI() const { return rhi; }

	// リロード

public:
	virtual void Reload(void* data, int32_t size);
#endif

	// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
public:
	virtual int GetRef() { return ReferenceObject::GetRef(); }
	virtual int AddRef() { return ReferenceObject::AddRef(); }
	virtual int Release() { return ReferenceObject::Release(); }
#endif
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace asd

#pragma once

#include "../asd.DeviceObject.h"
#include "../asd.Graphics_Imp.h"
#include "asd.RenderTexture2D.h"
#include <Math/asd.Vector2DI.h>

namespace asd
{

/**
	@brief	描画先として使用できるテクスチャ
*/
class RenderTexture2D_Imp : public RenderTexture2D, public DeviceObject
{
protected:
	Vector2DI m_size;
	TextureFormat m_format;
	ar::RenderTexture2D* rhi = nullptr;

	RenderTexture2D_Imp(Graphics* graphics, ar::RenderTexture2D* rhi, Vector2DI size, TextureFormat format);
	virtual ~RenderTexture2D_Imp();

public:
	static RenderTexture2D_Imp* Create(Graphics* graphics, int32_t width, int32_t height, TextureFormat format);

	bool Save(const achar* path) override;

	bool Lock(TextureLockInfomation* info) override { return false; };

	void Unlock() override{};

	Vector2DI GetSize() const override { return m_size; }
	TextureFormat GetFormat() const override { return m_format; }
	LoadState GetLoadState() const override { return LoadState::Loaded; }

#if !SWIG
	ar::RenderTexture2D* GetRHI() const { return rhi; }
#endif

public:
	virtual int GetRef() { return ReferenceObject::GetRef(); }
	virtual int AddRef() { return ReferenceObject::AddRef(); }
	virtual int Release() { return ReferenceObject::Release(); }
};

} // namespace asd
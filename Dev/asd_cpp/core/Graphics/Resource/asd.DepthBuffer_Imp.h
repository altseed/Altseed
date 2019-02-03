#pragma once

#include <Math/asd.Vector2DI.h>
#include <asd.common.Base.h>

#include "../asd.DeviceObject.h"
#include "asd.Texture.h"

namespace asd
{

class DepthBuffer_Imp : public Texture, public DeviceObject
{
protected:
	Vector2DI m_size;
	ar::DepthTexture* rhi = nullptr;

	DepthBuffer_Imp(Graphics* graphics, ar::DepthTexture* rhi, Vector2DI size);
	virtual ~DepthBuffer_Imp();

public:
	Vector2DI GetSize() const { return m_size; }

	ar::DepthTexture* GetRHI() const { return rhi; }

	/**
	@brief	テクスチャのクラスの種類を取得する。
	@return	種類
	*/
	virtual TextureClassType GetType() override { return TextureClassType::DepthBuffer; }

	static DepthBuffer_Imp* Create(Graphics* graphics, int32_t width, int32_t height);

	// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
public:
	virtual int GetRef() { return ReferenceObject::GetRef(); }
	virtual int AddRef() { return ReferenceObject::AddRef(); }
	virtual int Release() { return ReferenceObject::Release(); }
#endif
};

} // namespace asd
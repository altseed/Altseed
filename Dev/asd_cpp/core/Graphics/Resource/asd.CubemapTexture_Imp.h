
#include "../asd.DeviceObject.h"
#include "asd.CubemapTexture.h"

namespace asd
{
class CubemapTexture_Imp : public CubemapTexture, public DeviceObject
{
protected:
	CubemapTexture_Imp(Graphics* graphics, ar::CubemapTexture* rhi, TextureFormat format, Vector2DI size, int32_t mipmapCount);
	virtual ~CubemapTexture_Imp();

	ar::CubemapTexture* rhi = nullptr;
	Vector2DI size;
	int32_t mipmapCount = 0;
	TextureFormat format;

public:
	Vector2DI GetSize() const { return size; }
	int32_t GetMipmapCount() const { return mipmapCount; }
	ar::CubemapTexture* GetRHI() const { return rhi; }

	static CubemapTexture_Imp* Create(Graphics_Imp* graphics, const achar* path);

	// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
public:
	virtual int GetRef() { return ReferenceObject::GetRef(); }
	virtual int AddRef() { return ReferenceObject::AddRef(); }
	virtual int Release() { return ReferenceObject::Release(); }
#endif
};
} // namespace asd
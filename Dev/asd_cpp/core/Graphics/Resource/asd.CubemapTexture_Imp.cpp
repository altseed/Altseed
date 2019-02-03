
#include "asd.CubemapTexture_Imp.h"

#include "../asd.Graphics_Imp.h"

namespace asd
{
CubemapTexture_Imp::CubemapTexture_Imp(Graphics* graphics, ar::CubemapTexture* rhi, TextureFormat format, Vector2DI size,
									   int32_t mipmapCount)
	: DeviceObject(graphics), format(format), size(size), mipmapCount(mipmapCount)
{
	auto vramSize = 0;

	for (int32_t i = 0; i < mipmapCount; i++)
	{
		int32_t width = size.X;
		int32_t height = size.Y;

		ImageHelper::GetMipmapSize(i, width, height);

		vramSize = ImageHelper::GetVRAMSize(format, width, height) * 6;
	}

	auto g = (Graphics_Imp*)GetGraphics();
	g->IncVRAM(vramSize);
}

CubemapTexture_Imp::~CubemapTexture_Imp()
{
	auto vramSize = 0;

	for (int32_t i = 0; i < mipmapCount; i++)
	{
		int32_t width = size.X;
		int32_t height = size.Y;

		ImageHelper::GetMipmapSize(i, width, height);

		vramSize = ImageHelper::GetVRAMSize(format, width, height) * 6;
	}

	auto g = (Graphics_Imp*)GetGraphics();
	g->DecVRAM(vramSize);

	asd::SafeDelete(rhi);
}

CubemapTexture_Imp* CubemapTexture_Imp::Create(Graphics_Imp* graphics, const achar* path)
{
	auto staticFile = graphics->GetFile()->CreateStaticFile(path);
	if (staticFile == nullptr)
		return nullptr;

	auto g = (Graphics_Imp*)graphics;

	auto rhi = ar::CubemapTexture::Create(g->GetRHI());
	if (rhi->Initialize(g->GetRHI(), (void*)staticFile->GetBuffer().data(), staticFile->GetBuffer().size()))
	{
		SafeRelease(staticFile);

		return new CubemapTexture_Imp(graphics, rhi, (asd::TextureFormat)rhi->GetFormat(), Vector2DI(rhi->GetWidth(), rhi->GetHeight()),
									  rhi->GetMipmapCount());
	}

	SafeRelease(staticFile);
	asd::SafeDelete(rhi);

	return nullptr;
}

} // namespace asd
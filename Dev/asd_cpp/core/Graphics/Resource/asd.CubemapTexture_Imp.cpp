
#include "asd.CubemapTexture_Imp.h"

#include "../asd.Graphics_Imp.h"

namespace asd
{
	CubemapTexture_Imp::CubemapTexture_Imp(Graphics* graphics, TextureFormat format, Vector2DI size, int32_t mipmapCount)
		: DeviceObject(graphics)
		, format(format)
		, size(size)
		, mipmapCount(mipmapCount)
	{
		auto vramSize = 0;

		for (int32_t i = 0; i < mipmapCount; i++)
		{
			int32_t width = size.X;
			int32_t height = size.Y;

			ImageHelper::GetMipmapSize(i, width, height);

			vramSize = ImageHelper::GetVRAMSize(format, width, height) * 6;
		}

		auto g = (Graphics_Imp*) GetGraphics();
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

		auto g = (Graphics_Imp*) GetGraphics();
		g->DecVRAM(vramSize);
	}
}
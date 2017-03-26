
#include "asd.ImagePackage_Imp.h"
#include "asd.Texture2D_Imp.h"

#include "../asd.Graphics_Imp.h"

#include <Utility/asd.BinaryReader.h>

namespace asd
{
	ImagePackage_Imp* ImagePackage_Imp::Create(Graphics* graphics, const std::vector<uint8_t>& data)
	{
		auto g = (Graphics_Imp*) graphics;

		uint8_t* p = (uint8_t*) data.data();

		if (data.size() < 12) return nullptr;

		if (p[0] != 'a' ||
			p[1] != 'i' ||
			p[2] != 'p' ||
			p[3] != 0) return nullptr;

		ImagePackage_Imp* ret = new ImagePackage_Imp(graphics);

		BinaryReader reader;
		reader.ReadIn(data.begin(), data.end());

		auto header = reader.Get<int32_t>();
		auto version = reader.Get<int32_t>();
		auto layerCount = reader.Get<int32_t>();

		ret->names.resize(layerCount);
		ret->areas.resize(layerCount);
		ret->textures.resize(layerCount);
		ret->elementTypes.resize(layerCount);
		ret->additionalElementTypes.resize(layerCount);
		
		for (int32_t i = 0; i < layerCount; i++)
		{
			auto name = reader.Get<astring>();
			auto area = reader.Get<RectI>();

			if (version >= 1)
			{
				auto src = reader.Get<RectI>();
				ret->elementTypes[i] = (ImagePackageElementType)reader.Get<int32_t>();
				ret->additionalElementTypes[i] = (ImagePackageAdditionalElementType)reader.Get<int32_t>();
				
			}

			auto data = reader.Get(area.Width * area.Height * 4);

			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				auto data_ = data;
				for (int32_t y = 0; y < area.Height; y++)
				{
					memcpy(&(data[y * area.Width * 4]), &(data_[(area.Height - 1 - y) * area.Width * 4]), area.Width * 4);
				}
			}

			ret->textures[i] = g->CreateTexture2DWithRawData(area.Width, area.Height, TextureFormat::R8G8B8A8_UNORM_SRGB, data.data());
			ret->areas[i] = area;
			ret->names[i] = name;
		}

		return ret;
	}

	ImagePackage_Imp::ImagePackage_Imp(Graphics* graphics)
		: graphics(graphics)
	{
		SafeAddRef(graphics);
	}

	ImagePackage_Imp::~ImagePackage_Imp()
	{
		for (size_t i = 0; i < textures.size(); i++)
		{
			textures[i]->Release();
		}
		textures.clear();

		auto g = (Graphics_Imp*) graphics;
		g->ImagePackageContainer->Unregister(this);

		SafeRelease(graphics);
	}

	Texture2D* ImagePackage_Imp::GetImage_(int32_t index)
	{
		if (index < 0 || textures.size() <= index) return nullptr;
		return textures[index];
	}

	int32_t ImagePackage_Imp::GetImageCount()
	{
		return (int32_t)textures.size();
	}

	const achar* ImagePackage_Imp::GetImageName(int32_t index)
	{
		if (index < 0 || textures.size() <= index) return nullptr;
		return names[index].c_str();
	}

	RectI ImagePackage_Imp::GetImageArea(int32_t index)
	{
		if (index < 0 || textures.size() <= index) return RectI();
		return areas[index];
	}

	ImagePackageElementType ImagePackage_Imp::GetElementType(int32_t index)
	{
		if (index < 0 || textures.size() <= index) return ImagePackageElementType::Image;
		return elementTypes[index];
	}

	ImagePackageAdditionalElementType ImagePackage_Imp::GetAdditionalElementType(int32_t index)
	{

#if _WIN32
		if (index < 0 || textures.size() <= index) return ImagePackageAdditionalElementType::None;

#elif __APPLE__
		if (index < 0 || textures.size() <= index) return ImagePackageAdditionalElementType::None;

#else

#undef None
		if (index < 0 || textures.size() <= index) return ImagePackageAdditionalElementType::None;
#define None 0L

#endif
		return additionalElementTypes[index];
	}
}

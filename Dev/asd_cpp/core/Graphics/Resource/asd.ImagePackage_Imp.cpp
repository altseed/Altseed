
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

		
		BinaryReader reader;
		reader.ReadIn(data.begin(), data.end());

		auto header = reader.Get<int32_t>();
		auto version = reader.Get<int32_t>();
		auto layerCount = reader.Get<int32_t>();

		std::vector<Texture2D*>	textures;
		std::vector<astring>	names;
		std::vector<RectI>		areas;

		for (int32_t i = 0; i < layerCount; i++)
		{
			auto name = reader.Get<astring>();
			auto area = reader.Get<RectI>();
			auto data = reader.Get(area.Width * area.Height * 4);
			auto texture = g->CreateTexture2DWithRawData(area.Width, area.Height, TextureFormat::R8G8B8A8_UNORM_SRGB, data.data());

			textures.push_back(texture);
			areas.push_back(area);
			names.push_back(name);
		}

		return new ImagePackage_Imp(graphics, textures, names, areas);
	}

	ImagePackage_Imp::ImagePackage_Imp(
		Graphics* graphics,
		const std::vector<Texture2D*>&	textures,
		const std::vector<astring>&		names,
		const std::vector<RectI>&		areas)
		: graphics(graphics)
		, textures(textures)
		, names(names)
		, areas(areas)
	{
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
}
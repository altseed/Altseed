
#include "PSDParser.h"
#include "BitmapWriter.h"

int main()
{
	
	FILE* fp = nullptr;
	fopen_s(&fp, "import.psd", "rb");

	if (fp == nullptr) return 0;

	std::vector<uint8_t> data;
	fseek(fp, 0, SEEK_END);
	auto length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	data.resize(length);
	fread(data.data(), 1, length, fp);
	fclose(fp);

	auto doc = PSDParser::Document::Create(data.data(), data.size());

	for (auto i = 0; i < doc->GetLayerCount(); i++)
	{
		auto layer = doc->GetLayer(i);

		auto name = layer->GetName();
		auto rect = layer->GetRect();
		
		std::vector<uint8_t> bmp;
		auto height = rect.Bottom - rect.Top;
		auto width = rect.Right - rect.Left;
		bmp.resize(width * height * 4);

		for (int32_t y = 0; y < height; y++)
		{
			for (int32_t x = 0; x < width; x++)
			{
				auto p = (uint8_t*) layer->GetData();

				bmp[(x + y * width) * 4 + 0] = p[x * 4 + width * y + 0];
				bmp[(x + y * width) * 4 + 1] = p[x * 4 + width * y + 1];
				bmp[(x + y * width) * 4 + 2] = p[x * 4 + width * y + 2];
				bmp[(x + y * width) * 4 + 3] = p[x * 4 + width * y + 3];

			}
		}

		

		BitmapWriter::Write("test.bmp", (uint32_t*)(bmp.data()), width, height);

		//break;
	}

	return 0;
}

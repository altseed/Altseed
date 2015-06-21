
#include "asd.GifAnimationHelper.h"

namespace asd
{
	bool GifAnimationHelper::Initialize(const achar* filepath, int32_t width, int32_t height, int32_t framerate, float scale)
	{
#if _WIN32
		_wfopen_s(&fp, filepath, L"wb");
#else
		fp = fopen(ToUtf8String(filepath).c_str(), "wb");
#endif
		if (fp == nullptr) return false;

		imgPtr = gdImageCreate(width * scale, height * scale);
		gdImageGifAnimBegin(imgPtr, fp, /*GlobalColorMap=*/false, 0);
		this->framerate = framerate;
		this->width = width * scale;
		this->height = height * scale;
		return true;
	}

	void GifAnimationHelper::AddImage(Color* data, int width, int height)
	{
		int delay = (int) round((1.0 / (double) framerate) * 100.0);
		gdImagePtr frameImage = gdImageCreateTrueColor(this->width, this->height);

		if (this->width == width && this->height == height)
		{
			for (int32_t y = 0; y < height; y++)
			{
				for (int32_t x = 0; x < width; x++)
				{
					auto c = data[x + y * width];
					gdImageSetPixel(frameImage, x, y, gdTrueColor(c.R, c.G, c.B));
				}
			}
		}
		else
		{
			for (int32_t y = 0; y < this->height; y++)
			{
				for (int32_t x = 0; x < this->width; x++)
				{
					auto x_ = ((float)x / (float)this->width) * width;
					auto y_ = ((float) y / (float)this->height) * height;

					int32_t xi_ = (int32_t) x_;
					int32_t yi_ = (int32_t) y_;

					auto getColor = [this,&width,&height,&data](int xi, int yi) -> Color
					{
						if (xi < 0) xi = 0;
						if (yi < 0) yi = 0;

						if (xi >= width) xi = width - 1;
						if (yi >= height) yi = height - 1;

						return data[xi + yi * width];
					};
					
					auto c00 = getColor(xi_, yi_);
					auto c10 = getColor(xi_ + 1, yi_);
					auto c01 = getColor(xi_, yi_ + 1);
					auto c11 = getColor(xi_ + 1, yi_ + 1);

					float r0 = c10.R * (x_ - xi_) + c00.R * (1.0f - (x_ - xi_));
					float g0 = c10.G * (x_ - xi_) + c00.G * (1.0f - (x_ - xi_));
					float b0 = c10.B * (x_ - xi_) + c00.B * (1.0f - (x_ - xi_));

					float r1 = c11.R * (x_ - xi_) + c01.R * (1.0f - (x_ - xi_));
					float g1 = c11.G * (x_ - xi_) + c01.G * (1.0f - (x_ - xi_));
					float b1 = c11.B * (x_ - xi_) + c01.B * (1.0f - (x_ - xi_));

					float r = r1 * (y_ - yi_) + r0 * (1.0f - (y_ - yi_));
					float g = g1 * (y_ - yi_) + g0 * (1.0f - (y_ - yi_));
					float b = b1 * (y_ - yi_) + b0 * (1.0f - (y_ - yi_));

					gdImageSetPixel(frameImage, x, y, gdTrueColor((uint8_t) r, (uint8_t) g, (uint8_t) b));
				}
			}
		}
		
		gdImageTrueColorToPalette(frameImage, /*ditherFlag=*/true, gdMaxColors);
		gdImageGifAnimAdd(frameImage, fp, /*LocalColorMap=*/true, 0, 0, delay, gdDisposalNone, NULL);
		gdImageDestroy(frameImage);
	}

	void GifAnimationHelper::Finalize()
	{
		gdImageGifAnimEnd(fp);
		fclose(fp);
		gdImageDestroy(imgPtr);

		fp = nullptr;
		imgPtr = nullptr;
	}
}
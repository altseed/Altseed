#pragma once

#ifdef __APPLE__

#include "../asd.MediaPlayer.h"

#include <chrono>

namespace asd {

class MediaPlayerAVF_Impl;
    
class MediaPlayerAVF
	: public MediaPlayer
{
private:
	Graphics* graphics = nullptr;
    std::shared_ptr<MediaPlayerAVF_Impl> impl;
    
public:

	MediaPlayerAVF(Graphics* graphics);

	virtual ~MediaPlayerAVF();

	bool Play(bool isLoopingMode);

	void Write(void* dst);

	bool SetSourceFromPath(const char16_t* path);

	int32_t GetWidth() const;

	int32_t GetHeight() const;

	int32_t GetCurrentFrame() const override;

	Vector2DI GetSize() const override
	{
		return Vector2DI(GetWidth(), GetHeight());
	}

	bool Load(const achar* path) override;

	bool WriteToTexture2D(Texture2D* target) override;
};

}

#endif

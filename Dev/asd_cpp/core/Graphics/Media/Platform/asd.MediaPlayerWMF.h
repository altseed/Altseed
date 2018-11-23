#pragma once

#ifdef _WIN32

#include "../asd.MediaPlayer.h"

#include <chrono>
#include <thread>
#include <mutex>

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")

namespace asd {

class MediaPlayerWMF
	: public MediaPlayer
{
private:
	Graphics* graphics = nullptr;
	std::mutex mtx;
	std::thread thread_;

	uint32_t width = 0;
	uint32_t height = 0;
	float fps = 0;

	std::vector<uint8_t> internalBufferYUY;
	std::vector<uint8_t> internalBuffer;
	std::vector<uint8_t> internalBufferEditing;

	bool isPlaying = false;
	bool isThreadRequiredToJoin = false;

	int32_t currentFrame = 0;
	std::chrono::system_clock::time_point startTime;

	IMFSourceReader* reader = nullptr;

	void ThreadLoop();
public:

	MediaPlayerWMF(Graphics* graphics);

	virtual ~MediaPlayerWMF();

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
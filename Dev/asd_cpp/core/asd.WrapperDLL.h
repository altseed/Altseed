
#pragma once

#include <stdio.h>
#include <stdint.h>

#include <asd.common.Base.h>

extern "C"
{
	ASD_DLLEXPORT void* ASD_STDCALL CreateWrapperDLL();
	ASD_DLLEXPORT void ASD_STDCALL DeleteWrapperDLL(void* o);
}

namespace asd
{

class WrapperDLL {
public:
	virtual void Sound_Destruct(void* self);
	virtual void* Sound_CreateSoundSource(void* self,const char16_t* path,bool isDecompressed);
	virtual int32_t Sound_Play(void* self,void* soundSource);
	virtual bool Sound_GetIsPlaying(void* self,int32_t id);
	virtual void Sound_StopAll(void* self);
	virtual void Sound_Stop(void* self,int32_t id);
	virtual void Sound_Pause(void* self,int32_t id);
	virtual void Sound_Resume(void* self,int32_t id);
	virtual void Sound_SetVolume(void* self,int32_t id,float volume);
	virtual void Sound_FadeIn(void* self,int32_t id,float second);
	virtual void Sound_FadeOut(void* self,int32_t id,float second);
	virtual void Sound_Fade(void* self,int32_t id,float second,float targetedVolume);
	virtual bool Sound_GetIsPlaybackSpeedEnabled(void* self,int32_t id);
	virtual void Sound_SetIsPlaybackSpeedEnabled(void* self,int32_t id,bool isPlaybackSpeedEnabled);
	virtual float Sound_GetPlaybackSpeed(void* self,int32_t id);
	virtual void Sound_SetPlaybackSpeed(void* self,int32_t id,float playbackSpeed);
	virtual float Sound_GetPanningPosition(void* self,int32_t id);
	virtual void Sound_SetPanningPosition(void* self,int32_t id,float panningPosition);
	virtual void SoundSource_Destruct(void* self);
	virtual float SoundSource_GetLoopStartingPoint(void* self);
	virtual void SoundSource_SetLoopStartingPoint(void* self,float startingPoint);
	virtual float SoundSource_GetLoopEndPoint(void* self);
	virtual void SoundSource_SetLoopEndPoint(void* self,float endPoint);
	virtual bool SoundSource_GetIsLoopingMode(void* self);
	virtual void SoundSource_SetIsLoopingMode(void* self,bool isLoopingMode);
	virtual float SoundSource_GetLength(void* self);
};

};


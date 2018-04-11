
#include "asd.WrapperLib.h"
#include "asd.WrapperLib.Internal.h"
#include "asd.CoreToEngine.h"

namespace asd
{

static WrapperDLL* dll = nullptr;

void InitializeWrapper(CreateWrapperDLLFunc func) {
	dll = (WrapperDLL*)func();
};

void TerminateWrapper(DeleteWrapperDLLFunc func) {
	func(dll);
};


Sound::Sound(void* self, bool isCtrlSelf) {
	this->self = self;
	this->isCtrlSelf = isCtrlSelf;
}
Sound::~Sound(){
	if (isCtrlSelf) {
		dll->Sound_Destruct(self);
	}
};

std::shared_ptr<SoundSource> Sound::CreateSoundSource(const char16_t* path,bool isDecompressed){
	auto arg0 = self;
	auto arg1 = path;
	auto arg2 = isDecompressed;
	auto ret = dll->Sound_CreateSoundSource(arg0,arg1,arg2);
	return std::shared_ptr<SoundSource>( new SoundSource(ret, true) );
};

int32_t Sound::Play(std::shared_ptr<SoundSource> soundSource){
	auto arg0 = self;
	auto arg1 = soundSource.get()->self;
	auto ret = dll->Sound_Play(arg0,arg1);
	return ret;
};

bool Sound::GetIsPlaying(int32_t id){
	auto arg0 = self;
	auto arg1 = id;
	auto ret = dll->Sound_GetIsPlaying(arg0,arg1);
	return ret;
};

void Sound::StopAll(){
	auto arg0 = self;
	dll->Sound_StopAll(arg0);
};

void Sound::Stop(int32_t id){
	auto arg0 = self;
	auto arg1 = id;
	dll->Sound_Stop(arg0,arg1);
};

void Sound::Pause(int32_t id){
	auto arg0 = self;
	auto arg1 = id;
	dll->Sound_Pause(arg0,arg1);
};

void Sound::Resume(int32_t id){
	auto arg0 = self;
	auto arg1 = id;
	dll->Sound_Resume(arg0,arg1);
};

void Sound::SetVolume(int32_t id,float volume){
	auto arg0 = self;
	auto arg1 = id;
	auto arg2 = volume;
	dll->Sound_SetVolume(arg0,arg1,arg2);
};

void Sound::FadeIn(int32_t id,float second){
	auto arg0 = self;
	auto arg1 = id;
	auto arg2 = second;
	dll->Sound_FadeIn(arg0,arg1,arg2);
};

void Sound::FadeOut(int32_t id,float second){
	auto arg0 = self;
	auto arg1 = id;
	auto arg2 = second;
	dll->Sound_FadeOut(arg0,arg1,arg2);
};

void Sound::Fade(int32_t id,float second,float targetedVolume){
	auto arg0 = self;
	auto arg1 = id;
	auto arg2 = second;
	auto arg3 = targetedVolume;
	dll->Sound_Fade(arg0,arg1,arg2,arg3);
};

bool Sound::GetIsPlaybackSpeedEnabled(int32_t id){
	auto arg0 = self;
	auto arg1 = id;
	auto ret = dll->Sound_GetIsPlaybackSpeedEnabled(arg0,arg1);
	return ret;
};

void Sound::SetIsPlaybackSpeedEnabled(int32_t id,bool isPlaybackSpeedEnabled){
	auto arg0 = self;
	auto arg1 = id;
	auto arg2 = isPlaybackSpeedEnabled;
	dll->Sound_SetIsPlaybackSpeedEnabled(arg0,arg1,arg2);
};

float Sound::GetPlaybackSpeed(int32_t id){
	auto arg0 = self;
	auto arg1 = id;
	auto ret = dll->Sound_GetPlaybackSpeed(arg0,arg1);
	return ret;
};

void Sound::SetPlaybackSpeed(int32_t id,float playbackSpeed){
	auto arg0 = self;
	auto arg1 = id;
	auto arg2 = playbackSpeed;
	dll->Sound_SetPlaybackSpeed(arg0,arg1,arg2);
};

float Sound::GetPanningPosition(int32_t id){
	auto arg0 = self;
	auto arg1 = id;
	auto ret = dll->Sound_GetPanningPosition(arg0,arg1);
	return ret;
};

void Sound::SetPanningPosition(int32_t id,float panningPosition){
	auto arg0 = self;
	auto arg1 = id;
	auto arg2 = panningPosition;
	dll->Sound_SetPanningPosition(arg0,arg1,arg2);
};

SoundSource::SoundSource(void* self, bool isCtrlSelf) {
	this->self = self;
	this->isCtrlSelf = isCtrlSelf;
}
SoundSource::~SoundSource(){
	if (isCtrlSelf) {
		dll->SoundSource_Destruct(self);
	}
};

float SoundSource::GetLoopStartingPoint() const{
	auto arg0 = self;
	auto ret = dll->SoundSource_GetLoopStartingPoint(arg0);
	return ret;
};

void SoundSource::SetLoopStartingPoint(float startingPoint){
	auto arg0 = self;
	auto arg1 = startingPoint;
	dll->SoundSource_SetLoopStartingPoint(arg0,arg1);
};

float SoundSource::GetLoopEndPoint() const{
	auto arg0 = self;
	auto ret = dll->SoundSource_GetLoopEndPoint(arg0);
	return ret;
};

void SoundSource::SetLoopEndPoint(float endPoint){
	auto arg0 = self;
	auto arg1 = endPoint;
	dll->SoundSource_SetLoopEndPoint(arg0,arg1);
};

bool SoundSource::GetIsLoopingMode() const{
	auto arg0 = self;
	auto ret = dll->SoundSource_GetIsLoopingMode(arg0);
	return ret;
};

void SoundSource::SetIsLoopingMode(bool isLoopingMode){
	auto arg0 = self;
	auto arg1 = isLoopingMode;
	dll->SoundSource_SetIsLoopingMode(arg0,arg1);
};

float SoundSource::GetLength(){
	auto arg0 = self;
	auto ret = dll->SoundSource_GetLength(arg0);
	return ret;
};


};


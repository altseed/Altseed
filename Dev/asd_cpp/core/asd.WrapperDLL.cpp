
#include "asd.WrapperDLL.h"

#include "Sound/asd.Sound.h"
#include "Sound/asd.SoundSource.h"

#include "IO/asd.File.h"
#include "IO/asd.StaticFile.h"
#include "IO/asd.StreamFile.h"

#include "Graphics/Media/asd.MediaPlayer.h"

#include "Tool/asd.Tool.h"

namespace asd
{


void WrapperDLL::File_Destruct(void* self){
	auto self_ = (File*)self;
};

void* WrapperDLL::File_CreateStaticFile(void* self,const char16_t* path){
	auto self_ = (File*)self;
	auto arg0 = path;
	auto ret = self_->CreateStaticFile(arg0);
	return ret;
};

void* WrapperDLL::File_CreateStaticFileAsync(void* self,const char16_t* path){
	auto self_ = (File*)self;
	auto arg0 = path;
	auto ret = self_->CreateStaticFileAsync(arg0);
	return ret;
};

void* WrapperDLL::File_CreateStreamFile(void* self,const char16_t* path){
	auto self_ = (File*)self;
	auto arg0 = path;
	auto ret = self_->CreateStreamFile(arg0);
	return ret;
};

void WrapperDLL::File_AddRootDirectory(void* self,const char16_t* path){
	auto self_ = (File*)self;
	auto arg0 = path;
	self_->AddRootDirectory(arg0);
};

void WrapperDLL::File_AddRootPackageWithPassword(void* self,const char16_t* path,const char16_t* password){
	auto self_ = (File*)self;
	auto arg0 = path;
	auto arg1 = password;
	self_->AddRootPackageWithPassword(arg0,arg1);
};

void WrapperDLL::File_AddRootPackage(void* self,const char16_t* path){
	auto self_ = (File*)self;
	auto arg0 = path;
	self_->AddRootPackage(arg0);
};

void WrapperDLL::File_ClearRootDirectories(void* self){
	auto self_ = (File*)self;
	self_->ClearRootDirectories();
};

bool WrapperDLL::File_Exists(void* self,const char16_t* path){
	auto self_ = (File*)self;
	auto arg0 = path;
	auto ret = self_->Exists(arg0);
	return ret;
};

void WrapperDLL::MediaPlayer_Destruct(void* self){
	auto self_ = (MediaPlayer*)self;
	SafeRelease(self_);
};

bool WrapperDLL::MediaPlayer_Play(void* self){
	auto self_ = (MediaPlayer*)self;
	auto ret = self_->Play();
	return ret;
};

bool WrapperDLL::MediaPlayer_Load(void* self,const char16_t* path){
	auto self_ = (MediaPlayer*)self;
	auto arg0 = path;
	auto ret = self_->Load(arg0);
	return ret;
};

bool WrapperDLL::MediaPlayer_WriteToTexture2D(void* self,void* target){
	auto self_ = (MediaPlayer*)self;
	auto arg0 = (Texture2D*)target;
	auto ret = self_->WriteToTexture2D(arg0);
	return ret;
};

Vector2DI WrapperDLL::MediaPlayer_GetSize(void* self){
	auto self_ = (MediaPlayer*)self;
	auto ret = self_->GetSize();
	return ret;
};

int32_t WrapperDLL::MediaPlayer_GetCurrentFrame(void* self){
	auto self_ = (MediaPlayer*)self;
	auto ret = self_->GetCurrentFrame();
	return ret;
};

bool WrapperDLL::MediaPlayer_GetIsLoopingMode(void* self){
	auto self_ = (MediaPlayer*)self;
	auto ret = self_->GetIsLoopingMode();
	return ret;
};

void WrapperDLL::MediaPlayer_SetIsLoopingMode(void* self,bool isLoopingMode){
	auto self_ = (MediaPlayer*)self;
	auto arg0 = isLoopingMode;
	self_->SetIsLoopingMode(arg0);
};

void WrapperDLL::Sound_Destruct(void* self){
	auto self_ = (Sound*)self;
};

void* WrapperDLL::Sound_CreateSoundSource(void* self,const char16_t* path,bool isDecompressed){
	auto self_ = (Sound*)self;
	auto arg0 = path;
	auto arg1 = isDecompressed;
	auto ret = self_->CreateSoundSource(arg0,arg1);
	return ret;
};

int32_t WrapperDLL::Sound_Play(void* self,void* soundSource){
	auto self_ = (Sound*)self;
	auto arg0 = (SoundSource*)soundSource;
	auto ret = self_->Play(arg0);
	return ret;
};

bool WrapperDLL::Sound_GetIsPlaying(void* self,int32_t id){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto ret = self_->GetIsPlaying(arg0);
	return ret;
};

void WrapperDLL::Sound_StopAll(void* self){
	auto self_ = (Sound*)self;
	self_->StopAll();
};

void WrapperDLL::Sound_Stop(void* self,int32_t id){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	self_->Stop(arg0);
};

void WrapperDLL::Sound_Pause(void* self,int32_t id){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	self_->Pause(arg0);
};

void WrapperDLL::Sound_Resume(void* self,int32_t id){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	self_->Resume(arg0);
};

void WrapperDLL::Sound_SetVolume(void* self,int32_t id,float volume){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto arg1 = volume;
	self_->SetVolume(arg0,arg1);
};

void WrapperDLL::Sound_FadeIn(void* self,int32_t id,float second){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto arg1 = second;
	self_->FadeIn(arg0,arg1);
};

void WrapperDLL::Sound_FadeOut(void* self,int32_t id,float second){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto arg1 = second;
	self_->FadeOut(arg0,arg1);
};

void WrapperDLL::Sound_Fade(void* self,int32_t id,float second,float targetedVolume){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto arg1 = second;
	auto arg2 = targetedVolume;
	self_->Fade(arg0,arg1,arg2);
};

bool WrapperDLL::Sound_GetIsPlaybackSpeedEnabled(void* self,int32_t id){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto ret = self_->GetIsPlaybackSpeedEnabled(arg0);
	return ret;
};

void WrapperDLL::Sound_SetIsPlaybackSpeedEnabled(void* self,int32_t id,bool isPlaybackSpeedEnabled){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto arg1 = isPlaybackSpeedEnabled;
	self_->SetIsPlaybackSpeedEnabled(arg0,arg1);
};

float WrapperDLL::Sound_GetPlaybackSpeed(void* self,int32_t id){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto ret = self_->GetPlaybackSpeed(arg0);
	return ret;
};

void WrapperDLL::Sound_SetPlaybackSpeed(void* self,int32_t id,float playbackSpeed){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto arg1 = playbackSpeed;
	self_->SetPlaybackSpeed(arg0,arg1);
};

float WrapperDLL::Sound_GetPanningPosition(void* self,int32_t id){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto ret = self_->GetPanningPosition(arg0);
	return ret;
};

void WrapperDLL::Sound_SetPanningPosition(void* self,int32_t id,float panningPosition){
	auto self_ = (Sound*)self;
	auto arg0 = id;
	auto arg1 = panningPosition;
	self_->SetPanningPosition(arg0,arg1);
};

void WrapperDLL::SoundSource_Destruct(void* self){
	auto self_ = (SoundSource*)self;
	SafeRelease(self_);
};

float WrapperDLL::SoundSource_GetLoopStartingPoint(void* self){
	auto self_ = (SoundSource*)self;
	auto ret = self_->GetLoopStartingPoint();
	return ret;
};

void WrapperDLL::SoundSource_SetLoopStartingPoint(void* self,float startingPoint){
	auto self_ = (SoundSource*)self;
	auto arg0 = startingPoint;
	self_->SetLoopStartingPoint(arg0);
};

float WrapperDLL::SoundSource_GetLoopEndPoint(void* self){
	auto self_ = (SoundSource*)self;
	auto ret = self_->GetLoopEndPoint();
	return ret;
};

void WrapperDLL::SoundSource_SetLoopEndPoint(void* self,float endPoint){
	auto self_ = (SoundSource*)self;
	auto arg0 = endPoint;
	self_->SetLoopEndPoint(arg0);
};

bool WrapperDLL::SoundSource_GetIsLoopingMode(void* self){
	auto self_ = (SoundSource*)self;
	auto ret = self_->GetIsLoopingMode();
	return ret;
};

void WrapperDLL::SoundSource_SetIsLoopingMode(void* self,bool isLoopingMode){
	auto self_ = (SoundSource*)self;
	auto arg0 = isLoopingMode;
	self_->SetIsLoopingMode(arg0);
};

float WrapperDLL::SoundSource_GetLength(void* self){
	auto self_ = (SoundSource*)self;
	auto ret = self_->GetLength();
	return ret;
};

void WrapperDLL::StaticFile_Destruct(void* self){
	auto self_ = (StaticFile*)self;
	SafeRelease(self_);
};

const std::vector<uint8_t>& WrapperDLL::StaticFile_GetBuffer(void* self){
	auto self_ = (StaticFile*)self;
	auto& ret = self_->GetBuffer();
	return ret;
};

const char16_t* WrapperDLL::StaticFile_GetFullPath(void* self){
	auto self_ = (StaticFile*)self;
	auto ret = self_->GetFullPath();
	return ret;
};

void* WrapperDLL::StaticFile_GetData(void* self){
	auto self_ = (StaticFile*)self;
	auto ret = self_->GetData();
	return ret;
};

int32_t WrapperDLL::StaticFile_GetSize(void* self){
	auto self_ = (StaticFile*)self;
	auto ret = self_->GetSize();
	return ret;
};

bool WrapperDLL::StaticFile_GetIsInPackage(void* self){
	auto self_ = (StaticFile*)self;
	auto ret = self_->GetIsInPackage();
	return ret;
};

LoadState WrapperDLL::StaticFile_GetLoadState(void* self){
	auto self_ = (StaticFile*)self;
	auto ret = self_->GetLoadState();
	return ret;
};

void WrapperDLL::StreamFile_Destruct(void* self){
	auto self_ = (StreamFile*)self;
	SafeRelease(self_);
};

int32_t WrapperDLL::StreamFile_GetSize(void* self){
	auto self_ = (StreamFile*)self;
	auto ret = self_->GetSize();
	return ret;
};

int32_t WrapperDLL::StreamFile_GetCurrentPosition(void* self){
	auto self_ = (StreamFile*)self;
	auto ret = self_->GetCurrentPosition();
	return ret;
};

int32_t WrapperDLL::StreamFile_Read(void* self,int32_t size){
	auto self_ = (StreamFile*)self;
	auto arg0 = size;
	auto ret = self_->Read(arg0);
	return ret;
};

void* WrapperDLL::StreamFile_GetTempBuffer(void* self){
	auto self_ = (StreamFile*)self;
	auto ret = self_->GetTempBuffer();
	return ret;
};

int32_t WrapperDLL::StreamFile_GetTempBufferSize(void* self){
	auto self_ = (StreamFile*)self;
	auto ret = self_->GetTempBufferSize();
	return ret;
};

void WrapperDLL::Tool_Destruct(void* self){
	auto self_ = (Tool*)self;
};

bool WrapperDLL::Tool_BeginFullscreen(void* self,const char16_t* name,int32_t offset){
	auto self_ = (Tool*)self;
	auto arg0 = name;
	auto arg1 = offset;
	auto ret = self_->BeginFullscreen(arg0,arg1);
	return ret;
};

bool WrapperDLL::Tool_Begin(void* self,const char16_t* name){
	auto self_ = (Tool*)self;
	auto arg0 = name;
	auto ret = self_->Begin(arg0);
	return ret;
};

void WrapperDLL::Tool_End(void* self){
	auto self_ = (Tool*)self;
	self_->End();
};

void WrapperDLL::Tool_Separator(void* self){
	auto self_ = (Tool*)self;
	self_->Separator();
};

void WrapperDLL::Tool_SameLine(void* self){
	auto self_ = (Tool*)self;
	self_->SameLine();
};

void WrapperDLL::Tool_Text(void* self,const char16_t* text){
	auto self_ = (Tool*)self;
	auto arg0 = text;
	self_->Text(arg0);
};

bool WrapperDLL::Tool_Button(void* self,const char16_t* label){
	auto self_ = (Tool*)self;
	auto arg0 = label;
	auto ret = self_->Button(arg0);
	return ret;
};

void WrapperDLL::Tool_Image(void* self,void* user_texture,const Vector2DF& size){
	auto self_ = (Tool*)self;
	auto arg0 = (Texture2D*)user_texture;
	auto arg1 = size;
	self_->Image(arg0,arg1);
};

bool WrapperDLL::Tool_BeginCombo(void* self,const char16_t* label,const char16_t* preview_value){
	auto self_ = (Tool*)self;
	auto arg0 = label;
	auto arg1 = preview_value;
	auto ret = self_->BeginCombo(arg0,arg1);
	return ret;
};

void WrapperDLL::Tool_EndCombo(void* self){
	auto self_ = (Tool*)self;
	self_->EndCombo();
};

bool WrapperDLL::Tool_InputText(void* self,const char16_t* label,int8_t* buf,int32_t buf_size){
	auto self_ = (Tool*)self;
	auto arg0 = label;
	auto arg1 = buf;
	auto arg2 = buf_size;
	auto ret = self_->InputText(arg0,arg1,arg2);
	return ret;
};

bool WrapperDLL::Tool_InputInt(void* self,const char16_t* label,int* v){
	auto self_ = (Tool*)self;
	auto arg0 = label;
	auto arg1 = v;
	auto ret = self_->InputInt(arg0,arg1);
	return ret;
};

bool WrapperDLL::Tool_ColorEdit4(void* self,const char16_t* label,float* vs){
	auto self_ = (Tool*)self;
	auto arg0 = label;
	auto arg1 = vs;
	auto ret = self_->ColorEdit4(arg0,arg1);
	return ret;
};

bool WrapperDLL::Tool_Selectable(void* self,const char16_t* label,bool selected){
	auto self_ = (Tool*)self;
	auto arg0 = label;
	auto arg1 = selected;
	auto ret = self_->Selectable(arg0,arg1);
	return ret;
};

bool WrapperDLL::Tool_ListBox(void* self,const char16_t* label,int* current_item,const char16_t* items){
	auto self_ = (Tool*)self;
	auto arg0 = label;
	auto arg1 = current_item;
	auto arg2 = items;
	auto ret = self_->ListBox(arg0,arg1,arg2);
	return ret;
};

bool WrapperDLL::Tool_BeginMainMenuBar(void* self){
	auto self_ = (Tool*)self;
	auto ret = self_->BeginMainMenuBar();
	return ret;
};

void WrapperDLL::Tool_EndMainMenuBar(void* self){
	auto self_ = (Tool*)self;
	self_->EndMainMenuBar();
};

bool WrapperDLL::Tool_BeginMenuBar(void* self){
	auto self_ = (Tool*)self;
	auto ret = self_->BeginMenuBar();
	return ret;
};

void WrapperDLL::Tool_EndMenuBar(void* self){
	auto self_ = (Tool*)self;
	self_->EndMenuBar();
};

bool WrapperDLL::Tool_BeginMenu(void* self,const char16_t* label){
	auto self_ = (Tool*)self;
	auto arg0 = label;
	auto ret = self_->BeginMenu(arg0);
	return ret;
};

void WrapperDLL::Tool_EndMenu(void* self){
	auto self_ = (Tool*)self;
	self_->EndMenu();
};

bool WrapperDLL::Tool_MenuItem(void* self,const char16_t* label,const char16_t* shortcut,bool* p_selected){
	auto self_ = (Tool*)self;
	auto arg0 = label;
	auto arg1 = shortcut;
	auto arg2 = p_selected;
	auto ret = self_->MenuItem(arg0,arg1,arg2);
	return ret;
};

void WrapperDLL::Tool_Columns(void* self,int count){
	auto self_ = (Tool*)self;
	auto arg0 = count;
	self_->Columns(arg0);
};

void WrapperDLL::Tool_NextColumn(void* self){
	auto self_ = (Tool*)self;
	self_->NextColumn();
};

int WrapperDLL::Tool_GetColumnIndex(void* self){
	auto self_ = (Tool*)self;
	auto ret = self_->GetColumnIndex();
	return ret;
};

float WrapperDLL::Tool_GetColumnWidth(void* self,int column_index){
	auto self_ = (Tool*)self;
	auto arg0 = column_index;
	auto ret = self_->GetColumnWidth(arg0);
	return ret;
};

void WrapperDLL::Tool_SetColumnWidth(void* self,int column_index,float width){
	auto self_ = (Tool*)self;
	auto arg0 = column_index;
	auto arg1 = width;
	self_->SetColumnWidth(arg0,arg1);
};

void WrapperDLL::Tool_SetItemDefaultFocus(void* self){
	auto self_ = (Tool*)self;
	self_->SetItemDefaultFocus();
};

const char16_t* WrapperDLL::Tool_OpenDialog(void* self,const char16_t* filterList,const char16_t* defaultPath){
	auto self_ = (Tool*)self;
	auto arg0 = filterList;
	auto arg1 = defaultPath;
	auto ret = self_->OpenDialog(arg0,arg1);
	return ret;
};

const char16_t* WrapperDLL::Tool_SaveDialog(void* self,const char16_t* filterList,const char16_t* defaultPath){
	auto self_ = (Tool*)self;
	auto arg0 = filterList;
	auto arg1 = defaultPath;
	auto ret = self_->SaveDialog(arg0,arg1);
	return ret;
};

const char16_t* WrapperDLL::Tool_PickFolder(void* self,const char16_t* defaultPath){
	auto self_ = (Tool*)self;
	auto arg0 = defaultPath;
	auto ret = self_->PickFolder(arg0);
	return ret;
};

void WrapperDLL::Tool_AddFontFromFileTTF(void* self,const char16_t* filename,float size_pixels){
	auto self_ = (Tool*)self;
	auto arg0 = filename;
	auto arg1 = size_pixels;
	self_->AddFontFromFileTTF(arg0,arg1);
};

ToolDialogSelection WrapperDLL::Tool_ShowDialog(void* self,const char16_t* message,const char16_t* title,ToolDialogStyle style,ToolDialogButtons buttons){
	auto self_ = (Tool*)self;
	auto arg0 = message;
	auto arg1 = title;
	auto arg2 = style;
	auto arg3 = buttons;
	auto ret = self_->ShowDialog(arg0,arg1,arg2,arg3);
	return ret;
};


};

extern "C"
{

ASD_DLLEXPORT void* ASD_STDCALL CreateWrapperDLL()
{
	return new asd::WrapperDLL();
}

ASD_DLLEXPORT void ASD_STDCALL DeleteWrapperDLL(void* o)
{
	auto o_ = (asd::WrapperDLL*)o;
	delete o_;
}

}



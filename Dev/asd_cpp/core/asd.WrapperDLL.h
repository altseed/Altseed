
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <memory>
#include <vector>

#include <asd.common.Base.h>
#include "asd.Core.Base.h"

extern "C"
{
	ASD_DLLEXPORT void* ASD_STDCALL CreateWrapperDLL();
	ASD_DLLEXPORT void ASD_STDCALL DeleteWrapperDLL(void* o);
}

namespace asd
{

class WrapperDLL {
public:
	virtual void File_Destruct(void* self);
	virtual void* File_CreateStaticFile(void* self,const char16_t* path);
	virtual void* File_CreateStaticFileAsync(void* self,const char16_t* path);
	virtual void* File_CreateStreamFile(void* self,const char16_t* path);
	virtual void File_AddRootDirectory(void* self,const char16_t* path);
	virtual void File_AddRootPackageWithPassword(void* self,const char16_t* path,const char16_t* password);
	virtual void File_AddRootPackage(void* self,const char16_t* path);
	virtual void File_ClearRootDirectories(void* self);
	virtual bool File_Exists(void* self,const char16_t* path);
	virtual void MediaPlayer_Destruct(void* self);
	virtual bool MediaPlayer_Play(void* self);
	virtual bool MediaPlayer_Load(void* self,const char16_t* path);
	virtual bool MediaPlayer_WriteToTexture2D(void* self,void* target);
	virtual Vector2DI MediaPlayer_GetSize(void* self);
	virtual int32_t MediaPlayer_GetCurrentFrame(void* self);
	virtual bool MediaPlayer_GetIsLoopingMode(void* self);
	virtual void MediaPlayer_SetIsLoopingMode(void* self,bool isLoopingMode);
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
	virtual void StaticFile_Destruct(void* self);
	virtual const std::vector<uint8_t>& StaticFile_GetBuffer(void* self);
	virtual const char16_t* StaticFile_GetFullPath(void* self);
	virtual void* StaticFile_GetData(void* self);
	virtual int32_t StaticFile_GetSize(void* self);
	virtual bool StaticFile_GetIsInPackage(void* self);
	virtual LoadState StaticFile_GetLoadState(void* self);
	virtual void StreamFile_Destruct(void* self);
	virtual int32_t StreamFile_GetSize(void* self);
	virtual int32_t StreamFile_GetCurrentPosition(void* self);
	virtual int32_t StreamFile_Read(void* self,int32_t size);
	virtual void* StreamFile_GetTempBuffer(void* self);
	virtual int32_t StreamFile_GetTempBufferSize(void* self);
	virtual void Tool_Destruct(void* self);
	virtual bool Tool_BeginFullscreen(void* self,const char16_t* name,int32_t offset);
	virtual bool Tool_Begin(void* self,const char16_t* name);
	virtual void Tool_End(void* self);
	virtual void Tool_Separator(void* self);
	virtual void Tool_SameLine(void* self);
	virtual void Tool_Text(void* self,const char16_t* text);
	virtual bool Tool_Button(void* self,const char16_t* label);
	virtual void Tool_Image(void* self,void* user_texture,const Vector2DF& size);
	virtual bool Tool_BeginCombo(void* self,const char16_t* label,const char16_t* preview_value);
	virtual void Tool_EndCombo(void* self);
	virtual bool Tool_InputText(void* self,const char16_t* label,int8_t* buf,int32_t buf_size);
	virtual bool Tool_InputInt(void* self,const char16_t* label,int* v);
	virtual bool Tool_ColorEdit4(void* self,const char16_t* label,float* vs);
	virtual bool Tool_Selectable(void* self,const char16_t* label,bool selected);
	virtual bool Tool_ListBox(void* self,const char16_t* label,int* current_item,const char16_t* items);
	virtual bool Tool_BeginMainMenuBar(void* self);
	virtual void Tool_EndMainMenuBar(void* self);
	virtual bool Tool_BeginMenuBar(void* self);
	virtual void Tool_EndMenuBar(void* self);
	virtual bool Tool_BeginMenu(void* self,const char16_t* label);
	virtual void Tool_EndMenu(void* self);
	virtual bool Tool_MenuItem(void* self,const char16_t* label,const char16_t* shortcut,bool* p_selected);
	virtual void Tool_Columns(void* self,int count);
	virtual void Tool_NextColumn(void* self);
	virtual int Tool_GetColumnIndex(void* self);
	virtual float Tool_GetColumnWidth(void* self,int column_index);
	virtual void Tool_SetColumnWidth(void* self,int column_index,float width);
	virtual void Tool_SetItemDefaultFocus(void* self);
	virtual const char16_t* Tool_OpenDialog(void* self,const char16_t* filterList,const char16_t* defaultPath);
	virtual const char16_t* Tool_SaveDialog(void* self,const char16_t* filterList,const char16_t* defaultPath);
	virtual const char16_t* Tool_PickFolder(void* self,const char16_t* defaultPath);
	virtual void Tool_AddFontFromFileTTF(void* self,const char16_t* filename,float size_pixels);
	virtual ToolDialogSelection Tool_ShowDialog(void* self,const char16_t* message,const char16_t* title,ToolDialogStyle style,ToolDialogButtons buttons);
};

};


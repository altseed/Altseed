
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


File::File(void* self, bool isCtrlSelf) {
	this->self = self;
	this->isCtrlSelf = isCtrlSelf;
}
File::~File(){
	if (isCtrlSelf) {
		dll->File_Destruct(self);
	}
};

std::shared_ptr<StaticFile> File::CreateStaticFile(const char16_t* path){
	auto arg0 = self;
	auto arg1 = path;
	auto ret = dll->File_CreateStaticFile(arg0,arg1);
	return ret != nullptr ? std::shared_ptr<StaticFile>( new StaticFile(ret, true) ) : nullptr;
};

std::shared_ptr<StaticFile> File::CreateStaticFileAsync(const char16_t* path){
	auto arg0 = self;
	auto arg1 = path;
	auto ret = dll->File_CreateStaticFileAsync(arg0,arg1);
	return ret != nullptr ? std::shared_ptr<StaticFile>( new StaticFile(ret, true) ) : nullptr;
};

std::shared_ptr<StreamFile> File::CreateStreamFile(const char16_t* path){
	auto arg0 = self;
	auto arg1 = path;
	auto ret = dll->File_CreateStreamFile(arg0,arg1);
	return ret != nullptr ? std::shared_ptr<StreamFile>( new StreamFile(ret, true) ) : nullptr;
};

void File::AddRootDirectory(const char16_t* path){
	auto arg0 = self;
	auto arg1 = path;
	dll->File_AddRootDirectory(arg0,arg1);
};

void File::AddRootPackageWithPassword(const char16_t* path,const char16_t* password){
	auto arg0 = self;
	auto arg1 = path;
	auto arg2 = password;
	dll->File_AddRootPackageWithPassword(arg0,arg1,arg2);
};

void File::AddRootPackage(const char16_t* path){
	auto arg0 = self;
	auto arg1 = path;
	dll->File_AddRootPackage(arg0,arg1);
};

void File::ClearRootDirectories(){
	auto arg0 = self;
	dll->File_ClearRootDirectories(arg0);
};

bool File::Exists(const char16_t* path) const{
	auto arg0 = self;
	auto arg1 = path;
	auto ret = dll->File_Exists(arg0,arg1);
	return ret;
};

MediaPlayer::MediaPlayer(void* self, bool isCtrlSelf) {
	this->self = self;
	this->isCtrlSelf = isCtrlSelf;
}
MediaPlayer::~MediaPlayer(){
	if (isCtrlSelf) {
		dll->MediaPlayer_Destruct(self);
	}
};

bool MediaPlayer::Play(bool isLoopingMode){
	auto arg0 = self;
	auto arg1 = isLoopingMode;
	auto ret = dll->MediaPlayer_Play(arg0,arg1);
	return ret;
};

bool MediaPlayer::Load(const char16_t* path){
	auto arg0 = self;
	auto arg1 = path;
	auto ret = dll->MediaPlayer_Load(arg0,arg1);
	return ret;
};

bool MediaPlayer::WriteToTexture2D(std::shared_ptr<Texture2D> target){
	auto arg0 = self;
	auto arg1 = target.get();
	auto ret = dll->MediaPlayer_WriteToTexture2D(arg0,arg1);
	return ret;
};

Vector2DI MediaPlayer::GetSize() const{
	auto arg0 = self;
	auto ret = dll->MediaPlayer_GetSize(arg0);
	return ret;
};

int32_t MediaPlayer::GetCurrentFrame() const{
	auto arg0 = self;
	auto ret = dll->MediaPlayer_GetCurrentFrame(arg0);
	return ret;
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
	return ret != nullptr ? std::shared_ptr<SoundSource>( new SoundSource(ret, true) ) : nullptr;
};

int32_t Sound::Play(std::shared_ptr<SoundSource> soundSource){
	auto arg0 = self;
	auto arg1 = soundSource.get() != nullptr ? soundSource.get()->self : nullptr;
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

float Sound::GetPlaybackPercent(int32_t id){
	auto arg0 = self;
	auto arg1 = id;
	auto ret = dll->Sound_GetPlaybackPercent(arg0,arg1);
	return ret;
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

StaticFile::StaticFile(void* self, bool isCtrlSelf) {
	this->self = self;
	this->isCtrlSelf = isCtrlSelf;
}
StaticFile::~StaticFile(){
	if (isCtrlSelf) {
		dll->StaticFile_Destruct(self);
	}
};

const std::vector<uint8_t>& StaticFile::GetBuffer() const{
	auto arg0 = self;
	auto& ret = dll->StaticFile_GetBuffer(arg0);
	return ret;
};

const char16_t* StaticFile::GetFullPath() const{
	auto arg0 = self;
	auto ret = dll->StaticFile_GetFullPath(arg0);
	return ret;
};

void* StaticFile::GetData(){
	auto arg0 = self;
	auto ret = dll->StaticFile_GetData(arg0);
	return ret;
};

int32_t StaticFile::GetSize(){
	auto arg0 = self;
	auto ret = dll->StaticFile_GetSize(arg0);
	return ret;
};

bool StaticFile::GetIsInPackage() const{
	auto arg0 = self;
	auto ret = dll->StaticFile_GetIsInPackage(arg0);
	return ret;
};

LoadState StaticFile::GetLoadState() const{
	auto arg0 = self;
	auto ret = dll->StaticFile_GetLoadState(arg0);
	return ret;
};

StreamFile::StreamFile(void* self, bool isCtrlSelf) {
	this->self = self;
	this->isCtrlSelf = isCtrlSelf;
}
StreamFile::~StreamFile(){
	if (isCtrlSelf) {
		dll->StreamFile_Destruct(self);
	}
};

int32_t StreamFile::GetSize() const{
	auto arg0 = self;
	auto ret = dll->StreamFile_GetSize(arg0);
	return ret;
};

int32_t StreamFile::GetCurrentPosition() const{
	auto arg0 = self;
	auto ret = dll->StreamFile_GetCurrentPosition(arg0);
	return ret;
};

int32_t StreamFile::Read(int32_t size){
	auto arg0 = self;
	auto arg1 = size;
	auto ret = dll->StreamFile_Read(arg0,arg1);
	return ret;
};

void* StreamFile::GetTempBuffer(){
	auto arg0 = self;
	auto ret = dll->StreamFile_GetTempBuffer(arg0);
	return ret;
};

int32_t StreamFile::GetTempBufferSize(){
	auto arg0 = self;
	auto ret = dll->StreamFile_GetTempBufferSize(arg0);
	return ret;
};

Tool::Tool(void* self, bool isCtrlSelf) {
	this->self = self;
	this->isCtrlSelf = isCtrlSelf;
}
Tool::~Tool(){
	if (isCtrlSelf) {
		dll->Tool_Destruct(self);
	}
};

bool Tool::BeginFullscreen(const char16_t* name,int32_t offset){
	auto arg0 = self;
	auto arg1 = name;
	auto arg2 = offset;
	auto ret = dll->Tool_BeginFullscreen(arg0,arg1,arg2);
	return ret;
};

bool Tool::Begin(const char16_t* name){
	auto arg0 = self;
	auto arg1 = name;
	auto ret = dll->Tool_Begin(arg0,arg1);
	return ret;
};

void Tool::End(){
	auto arg0 = self;
	dll->Tool_End(arg0);
};

void Tool::Separator(){
	auto arg0 = self;
	dll->Tool_Separator(arg0);
};

void Tool::SameLine(){
	auto arg0 = self;
	dll->Tool_SameLine(arg0);
};

void Tool::Text(const char16_t* text){
	auto arg0 = self;
	auto arg1 = text;
	dll->Tool_Text(arg0,arg1);
};

bool Tool::Button(const char16_t* label){
	auto arg0 = self;
	auto arg1 = label;
	auto ret = dll->Tool_Button(arg0,arg1);
	return ret;
};

void Tool::Image(std::shared_ptr<Texture2D> user_texture,const Vector2DF& size){
	auto arg0 = self;
	auto arg1 = user_texture.get();
	auto arg2 = size;
	dll->Tool_Image(arg0,arg1,arg2);
};

bool Tool::BeginCombo(const char16_t* label,const char16_t* preview_value){
	auto arg0 = self;
	auto arg1 = label;
	auto arg2 = preview_value;
	auto ret = dll->Tool_BeginCombo(arg0,arg1,arg2);
	return ret;
};

void Tool::EndCombo(){
	auto arg0 = self;
	dll->Tool_EndCombo(arg0);
};

bool Tool::InputText(const char16_t* label,int8_t* buf,int32_t buf_size){
	auto arg0 = self;
	auto arg1 = label;
	auto arg2 = buf;
	auto arg3 = buf_size;
	auto ret = dll->Tool_InputText(arg0,arg1,arg2,arg3);
	return ret;
};

bool Tool::InputInt(const char16_t* label,int* v){
	auto arg0 = self;
	auto arg1 = label;
	auto arg2 = v;
	auto ret = dll->Tool_InputInt(arg0,arg1,arg2);
	return ret;
};

bool Tool::ColorEdit4(const char16_t* label,float* vs){
	auto arg0 = self;
	auto arg1 = label;
	auto arg2 = vs;
	auto ret = dll->Tool_ColorEdit4(arg0,arg1,arg2);
	return ret;
};

bool Tool::Selectable(const char16_t* label,bool selected){
	auto arg0 = self;
	auto arg1 = label;
	auto arg2 = selected;
	auto ret = dll->Tool_Selectable(arg0,arg1,arg2);
	return ret;
};

bool Tool::ListBox(const char16_t* label,int* current_item,const char16_t* items){
	auto arg0 = self;
	auto arg1 = label;
	auto arg2 = current_item;
	auto arg3 = items;
	auto ret = dll->Tool_ListBox(arg0,arg1,arg2,arg3);
	return ret;
};

bool Tool::BeginMainMenuBar(){
	auto arg0 = self;
	auto ret = dll->Tool_BeginMainMenuBar(arg0);
	return ret;
};

void Tool::EndMainMenuBar(){
	auto arg0 = self;
	dll->Tool_EndMainMenuBar(arg0);
};

bool Tool::BeginMenuBar(){
	auto arg0 = self;
	auto ret = dll->Tool_BeginMenuBar(arg0);
	return ret;
};

void Tool::EndMenuBar(){
	auto arg0 = self;
	dll->Tool_EndMenuBar(arg0);
};

bool Tool::BeginMenu(const char16_t* label){
	auto arg0 = self;
	auto arg1 = label;
	auto ret = dll->Tool_BeginMenu(arg0,arg1);
	return ret;
};

void Tool::EndMenu(){
	auto arg0 = self;
	dll->Tool_EndMenu(arg0);
};

bool Tool::MenuItem(const char16_t* label,const char16_t* shortcut,bool* p_selected){
	auto arg0 = self;
	auto arg1 = label;
	auto arg2 = shortcut;
	auto arg3 = p_selected;
	auto ret = dll->Tool_MenuItem(arg0,arg1,arg2,arg3);
	return ret;
};

void Tool::Columns(int count){
	auto arg0 = self;
	auto arg1 = count;
	dll->Tool_Columns(arg0,arg1);
};

void Tool::NextColumn(){
	auto arg0 = self;
	dll->Tool_NextColumn(arg0);
};

int Tool::GetColumnIndex(){
	auto arg0 = self;
	auto ret = dll->Tool_GetColumnIndex(arg0);
	return ret;
};

float Tool::GetColumnWidth(int column_index){
	auto arg0 = self;
	auto arg1 = column_index;
	auto ret = dll->Tool_GetColumnWidth(arg0,arg1);
	return ret;
};

void Tool::SetColumnWidth(int column_index,float width){
	auto arg0 = self;
	auto arg1 = column_index;
	auto arg2 = width;
	dll->Tool_SetColumnWidth(arg0,arg1,arg2);
};

void Tool::SetItemDefaultFocus(){
	auto arg0 = self;
	dll->Tool_SetItemDefaultFocus(arg0);
};

const char16_t* Tool::OpenDialog(const char16_t* filterList,const char16_t* defaultPath){
	auto arg0 = self;
	auto arg1 = filterList;
	auto arg2 = defaultPath;
	auto ret = dll->Tool_OpenDialog(arg0,arg1,arg2);
	return ret;
};

const char16_t* Tool::SaveDialog(const char16_t* filterList,const char16_t* defaultPath){
	auto arg0 = self;
	auto arg1 = filterList;
	auto arg2 = defaultPath;
	auto ret = dll->Tool_SaveDialog(arg0,arg1,arg2);
	return ret;
};

const char16_t* Tool::PickFolder(const char16_t* defaultPath){
	auto arg0 = self;
	auto arg1 = defaultPath;
	auto ret = dll->Tool_PickFolder(arg0,arg1);
	return ret;
};

void Tool::AddFontFromFileTTF(const char16_t* filename,float size_pixels){
	auto arg0 = self;
	auto arg1 = filename;
	auto arg2 = size_pixels;
	dll->Tool_AddFontFromFileTTF(arg0,arg1,arg2);
};

ToolDialogSelection Tool::ShowDialog(const char16_t* message,const char16_t* title,ToolDialogStyle style,ToolDialogButtons buttons){
	auto arg0 = self;
	auto arg1 = message;
	auto arg2 = title;
	auto arg3 = style;
	auto arg4 = buttons;
	auto ret = dll->Tool_ShowDialog(arg0,arg1,arg2,arg3,arg4);
	return ret;
};


};


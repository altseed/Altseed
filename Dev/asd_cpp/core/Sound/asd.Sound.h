
#pragma once

#include "../asd.Core.Base.h"
#include "../asd.ReferenceObject.h"
#include "../IO/asd.File_Imp.h"
#include "../Utils/asd.ResourceContainer.h"

#include <OpenSoundMixer.h>

#ifdef _WIN32
#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/OpenSoundMixer.lib")
#else
#pragma comment(lib,"x64/Release/OpenSoundMixer.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/OpenSoundMixer.lib")
#else
#pragma comment(lib,"x86/Release/OpenSoundMixer.lib")
#endif

#endif
#endif

namespace asd
{
	class Core_Imp;

	/**
		@brief	音を管理するクラス
	*/
	class Sound
		: public ReferenceObject
	{
		friend class Core;
		friend class Core_Imp;

	private:
		osm::Manager*	m_manager;
		Log*			log;
		File*			m_file;

	protected:
		Sound(File *file, Log* log, bool isReloadingEnabled);
		virtual ~Sound();

	public:
		
		/**
			@brief	音を読み込む。
			@param	path	パス
			@param	isDecompressed	解凍するか?
			@return	音源
		*/
		virtual SoundSource* CreateSoundSource(const char16_t* path, bool isDecompressed);

		/**
			@brief	音を再生する。
			@param	soundSource	音源
			@return	ID
		*/
		virtual int32_t Play(SoundSource* soundSource);

		/**
		@brief	音が再生中か、取得する。
		@param	id	ID
		@return	再生中か?
		*/
		virtual bool GetIsPlaying(int32_t id);

		/**
		@brief	全ての再生中の音を停止する。
		*/
		virtual void StopAll();

		/**
		@brief	指定した音を停止する。
		@param	id	ID
		*/
		virtual void Stop(int32_t id);

		/**
		@brief	指定した音を一時停止する。
		@param	id	ID
		*/
		virtual void Pause(int32_t id);

		/**
		@brief	指定した一時停止中の音の一時停止を解除する。
		@param	id	ID
		*/
		virtual void Resume(int32_t id);

		/**
		@brief	指定した音の音量を設定する。
		@param	id	ID
		@param	volume	音量(0.0～1.0)
		*/
		virtual void SetVolume(int32_t id, float volume);

		/**
		@brief	指定した音をフェードインさせる。
		@param	id	ID
		@param	second	フェードインに使用する時間(秒)
		*/
		virtual void FadeIn(int32_t id, float second);

		/**
		@brief	指定した音をフェードアウトさせる。
		@param	id	ID
		@param	second	フェードアウトに使用する時間(秒)
		*/
		virtual void FadeOut(int32_t id, float second);

		/**
		@brief	任意の音量に音量を一定時間かけて変更する。
		@param	id		音のID
		@param	second	変更にかかる時間(秒)
		@param	targetedVolume	変更先の音量(0.0～1.0)
		@note
		この指定される音量はSetVolumeに指定される音量とは異なり、FadeIn、FadeOutに使用される音量と共通である。
		つまり、このメソッドはFadeIn、FadeOutの任意音量指定版と言える。
		*/
		virtual void Fade(int32_t id, float second, float targetedVolume);

		/**
		@brief	再生速度変更するかを取得する。
		@param	id	音のID
		@return	再生速度変更するか?
		*/
		virtual bool GetIsPlaybackSpeedEnabled(int32_t id);

		/**
		@brief	再生速度変更するかを設定する。
		@param	id	音のID
		@param	isPlaybackSpeedEnabled	再生速度変更するか?
		*/
		virtual void SetIsPlaybackSpeedEnabled(int32_t id, bool isPlaybackSpeedEnabled);

		/**
		@brief	再生速度を取得する。
		@param	id	音のID
		@return	再生速度(比率)
		@note
		設定値は再生速度に比例する。1.0で等速。範囲は0.25から4.0。
		音程は再生速度に比例して変化する。
		*/
		virtual float GetPlaybackSpeed(int32_t id);

		/**
		@brief	再生速度を設定する。
		@param	id	音のID
		@param	playbackSpeed	再生速度(比率)
		@note
		設定値は再生速度に比例する。1.0で等速。範囲は0.25から4.0。
		音程は再生速度に比例して変化する。
		*/
		virtual void SetPlaybackSpeed(int32_t id, float playbackSpeed);

		/**
		@brief	パン位置を取得する。
		@param	id	音のID
		@return	パン位置, 0.0で中央, -1.0で左, 1.0で右。
		*/
		virtual float GetPanningPosition(int32_t id);

		/**
		@brief	パン位置を設定する。
		@param	id	音のID
		@param	panningPosition	パン位置, 0.0で中央, -1.0で左, 1.0で右。
		*/
		virtual void SetPanningPosition(int32_t id, float panningPosition);

		/**
		@brief	現在の再生位置を0～1で取得する。
		@param	id	音のID
		@return	再生位置
		*/
		virtual float GetPlaybackPercent(int32_t id);

#if !SWIG
		std::shared_ptr<ResourceContainer<SoundSource>>	SoundSourcesContainer;

		/**
			@brief	リロードする。
			@note
			#Ignore
		*/
		void Reload();

		osm::Manager* GetManager() { return m_manager; }
#endif
	};
}

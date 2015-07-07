
#pragma once

#include "../asd.Core.Base.h"
#include "../asd.ReferenceObject.h"

namespace asd
{
	/**
		@brief	音を管理するクラス
	*/
	class Sound
		: public IReference
	{
		friend class Accessor;

	protected:
		Sound() {}
		~Sound() {}

		virtual SoundSource* CreateSoundSource_(const achar* path, bool isDecompressed) = 0;
	public:
		
#if !SWIG
		/**
			@brief	音を読み込む。
			@param	path	パス
			@param	isDecompressed	解凍するか?
			@return	音源
		*/
		std::shared_ptr<SoundSource> CreateSoundSource(const achar* path, bool isDecompressed)
		{
			return CreateSharedPtrWithReleaseDLL(CreateSoundSource_(path, isDecompressed));
		}
#endif

		/**
			@brief	音を再生する。
			@param	soundSource	音源
			@return	ID
		*/
		virtual int32_t Play(SoundSource* soundSource) = 0;

#if !SWIG
		/**
		@brief	音を再生する。
		@param	soundSource	音源
		@return	ID
		*/
		int32_t Play(std::shared_ptr<SoundSource> soundSource)
		{
			return Play(soundSource.get());
		}
#endif

		/**
		@brief	音が再生中か、取得する。
		@param	id	ID
		@return	再生中か?
		*/
		virtual bool GetIsPlaying(int32_t id) = 0;

		/**
		@brief	全ての再生中の音を停止する。
		*/
		virtual void StopAll() = 0;

		/**
		@brief	指定した音を停止する。
		@param	id	ID
		*/
		virtual void Stop(int32_t id) = 0;

		/**
		@brief	指定した音を一時停止する。
		@param	id	ID
		*/
		virtual void Pause(int32_t id) = 0;

		/**
		@brief	指定した一時停止中の音の一時停止を解除する。
		@param	id	ID
		*/
		virtual void Resume(int32_t id) = 0;

		/**
		@brief	指定した音の音量を設定する。
		@param	id	ID
		@param	volume	音量(0.0～1.0)
		*/
		virtual void SetVolume(int32_t id, float volume) = 0;

		/**
		@brief	指定した音をフェードインさせる。
		@param	id	ID
		@param	second	フェードインに使用する時間(秒)
		*/
		virtual void FadeIn(int32_t id, float second) = 0;

		/**
		@brief	指定した音をフェードアウトさせる。
		@param	id	ID
		@param	second	フェードアウトに使用する時間(秒)
		*/
		virtual void FadeOut(int32_t id, float second) = 0;

		/**
		@brief	任意の音量に音量を一定時間かけて変更する。
		@param	id		音のID
		@param	second	変更にかかる時間(秒)
		@param	targetedVolume	変更先の音量(0.0～1.0)
		@note
		この指定される音量はSetVolumeに指定される音量とは異なり、FadeIn、FadeOutに使用される音量と共通である。
		つまり、このメソッドはFadeIn、FadeOutの任意音量指定版と言える。
		*/
		virtual void Fade(int32_t id, float second, float targetedVolume) = 0;
	};
}
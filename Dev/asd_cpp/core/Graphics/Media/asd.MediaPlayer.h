#pragma once

#include "../../asd.Core.Base.h"
#include "../../asd.ReferenceObject.h"

#include <Math/asd.Vector2DI.h>

namespace asd {

/**
	@brief	映像ファイルを再生するためのクラス
*/
class MediaPlayer
	: public ReferenceObject
{
private:

protected:
	MediaPlayer() {}
	virtual ~MediaPlayer() {}
	bool isLoopingMode = false;
public:

	/**
		@brief	映像を再生する。
	*/
	virtual bool Play() = 0;

	/**
		@brief	映像を読み込む。
		@param	path	ファイルのパス
		@note
		H264を推奨する。
	*/
	virtual bool Load(const char16_t* path) = 0;

	/**
		@brief	現在の映像をテクスチャに書き込む。
		@param	target	書き込み先
		@note
		映像とテクスチャの解像度は等しい必要がある。
	*/
	virtual bool WriteToTexture2D(Texture2D* target) = 0;

	/**
		@brief	読み込んだ映像の大きさを取得する。
	*/
	virtual Vector2DI GetSize() const = 0;

	/**
		@brief	現在再生中の映像のフレーム番号を取得する。
	*/
	virtual int32_t GetCurrentFrame() const = 0;

	/**
	@brief	ループするかを取得する。
	@return	ループするか?
	*/
	virtual bool GetIsLoopingMode() const;

	/**
	@brief	ループするかを設定する。
	@param	isLoopingMode	ループするか?
	*/
	virtual void SetIsLoopingMode(bool isLoopingMode);
};

}
#pragma once

#include "asd.Object2D.h"

namespace asd
{
	/**
		@brief	2Dレイヤーの一部を描画するためのカメラ
	*/
	class CameraObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreCameraObject2D> m_coreObject;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<CameraObject2D> Ptr;

		CameraObject2D();
		virtual ~CameraObject2D();

		/**
			@brief	描画元の領域を取得する。
			@return	描画元
		*/
		RectI GetSrc() const;

		/**
		@brief	描画元の領域を設定する。
		@param	value	描画元
		*/
		void SetSrc(RectI value);

		/**
		@brief	描画先の領域を取得する。
		@return	描画先
		*/
		RectI GetDst() const;

		/**
		@brief	描画先の領域を設定する。
		@param	value	描画先
		*/
		void SetDst(RectI value);

		/**
		@brief	画面に描画せずにテクスチャに描画するか、を取得する。
		@return	画面に描画せずにテクスチャに描画するか
		*/
		bool GetIsOffscreenMode();

		/**
		@brief	画面に描画せずにテクスチャに描画するか、を設定する。
		@param	value	画面に描画せずにテクスチャに描画するか
		*/
		void SetIsOffscreenMode(bool value);

		/**
		@brief	カメラが描画した結果をテクスチャとして取得する。
		@return	描画結果
		*/
		std::shared_ptr<Texture2D> GetTexture();
	};
}
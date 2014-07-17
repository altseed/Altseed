#pragma once

#include "ace.Object2D.h"

namespace ace
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
	};
}
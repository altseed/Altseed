
#pragma once

#include "asd.Object3D.h"

namespace asd
{
	class PostEffect;

	/**
	@brief	カメラの機能を提供するクラス
	*/
	class CameraObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreCameraObject3D> m_coreObject;

		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;

		void OnUpdateInternal() override;

	protected:
		/**
		@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart() {};

		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		CameraObject3D();
		virtual ~CameraObject3D();

		/**
			@brief	画面サイズを取得する。
			@return	画面サイズ
		*/
		Vector2DI GetWindowSize() const;

		/**
			@brief	画面サイズを設定する。
			@param	size	画面サイズ
		*/
		void SetWindowSize(const Vector2DI& size);

		/**
			@brief	カメラが見る座標を取得する。
			@return	カメラが見る座標
		*/
		Vector3DF GetFocus();

		/**
			@brief	カメラが見る座標を設定する。
			@param	focus	カメラが見る座標
		*/
		void SetFocus(const Vector3DF& focus);

		/**
		@brief	画角を取得する。
		@return	画角
		*/
		float GetFieldOfView();

		/**
		@brief	画角を設定する。
		@param	fov	画角
		*/
		void SetFieldOfView(float fov);

		/**
		@brief	画角を取得する。
		@return	画角
		*/
		float GetZFar();

		/**
		@brief	画角を設定する。
		@param	zfar	画角
		*/
		void SetZFar(float zfar);

		/**
		@brief	描画する最遠距離を取得する。
		@return	描画する最遠距離
		*/
		float GetZNear();

		/**
		@brief	描画する最近距離を設定する。
		@param	znear	描画する最近距離
		*/
		void SetZNear(float znear);

		/**
		@brief	描画先のフォーマットがHDRか取得する。
		@return	HDRか?
		*/
		bool GetHDRMode() const;

		/**
		@brief	描画先のフォーマットをHDRにするか設定する。
		@param	value	HDRか?
		*/
		void SetHDRMode(bool value);

		/**
		@brief	ポストエフェクトを追加する。
		@param	postEffect	ポストエフェクト
		*/
		void AddPostEffect(const std::shared_ptr<PostEffect>& postEffect);

		/**
		@brief	ポストエフェクトを全て消去する。
		*/
		void ClearPostEffects();

	};
}
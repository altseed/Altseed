
#pragma once

#include "ace.Object3D.h"

namespace ace
{
	class CameraObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreCameraObject3D> m_coreObject;

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

		Vector2DI GetWindowSize() const;
		void SetWindowSize(const Vector2DI& size);

		Vector3DF GetFocus();
		void SetFocus(const Vector3DF& focus);

		float GetFieldOfView();
		void SetFieldOfView(float fov);

		float GetZFar();
		void SetZFar(float zfar);

		float GetZNear();
		void SetZNear(float znear);
	};
}
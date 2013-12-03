
#pragma once

#include "ace.Object3D.h"

namespace ace
{
	class DirectionalLightObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreDirectionalLightObject3D> m_coreObject;

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
		DirectionalLightObject3D();
		virtual ~DirectionalLightObject3D();

		Color GetColor();
		void SetColor(const Color& color);
	};
}
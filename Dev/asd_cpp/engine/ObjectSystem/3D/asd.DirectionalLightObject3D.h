
#pragma once

#include "asd.Object3D.h"

namespace asd
{
	/**
	@brief	無限遠光源の機能を提供するクラス
	*/
	class DirectionalLightObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreDirectionalLightObject3D> m_coreObject;

	protected:
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

		/**
			@brief	光源の色を取得する。
			@return	光源の色
		*/
		Color GetColor();

		/**
		@brief	光源の色を設定する。
		@param	color	光源の色
		*/
		void SetColor(const Color& color);

		/**
			@brief	光源の強さを取得する。
			@return	光源の強さ
			@note
			デフォルトは1.0である。
		*/
		float GetIntensity();

		/**
		@brief	光源の強さを設定する。
		@param	intensity	光源の強さ
		@note
		デフォルトは1.0である。
		*/
		void SetIntensity(float intensity);
	};
}
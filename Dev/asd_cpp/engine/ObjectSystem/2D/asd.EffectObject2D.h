#pragma once

#include "asd.Object2D.h"

namespace asd
{
	class EffectObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreEffectObject2D> m_coreObject;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<EffectObject2D> Ptr;

		EffectObject2D();
		virtual ~EffectObject2D();

		/**
		@brief	描画に使用するエフェクトを取得する。
		@return	エフェクト
		*/
		std::shared_ptr<Effect> GetEffect();

		/**
		@brief	描画に使用するエフェクトを設定する。
		@param	effect	エフェクト
		*/
		void SetEffect(std::shared_ptr<Effect> effect);


		/**
		@brief	設定されている全てのエフェクトを再生する。
		@return	再生されたエフェクトのID
		*/
		int32_t Play();

		/**
		@brief	このオブジェクトから再生されたエフェクトを全て停止する。
		*/
		void Stop();

		/**
		@brief	このオブジェクトから再生されたエフェクトのルートを全て停止する。
		*/
		void StopRoot();

		/**
		@brief	このオブジェクトから再生されたエフェクトを表示状態にする。
		*/
		void Show();

		/**
		@brief	このオブジェクトから再生されたエフェクトを非表示状態にする。
		*/
		void Hide();

		/**
		@brief	このオブジェクトから再生されたエフェクトが再生中か取得する。
		@return	再生中か?
		*/
		bool GetIsPlaying();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得する。
		@return	フラグ
		*/
		bool GetSyncEffects();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか設定する。
		@param	value	フラグ
		*/
		void SetSyncEffects(bool value);

		/**
		@brief	このオブジェクトから再生されるエフェクトの向きを取得する。
		@return	value	回転量(度)
		*/
		float GetEffectRotation() const;

		/**
		@brief	このオブジェクトから再生されるエフェクトの向き(度)を指定する。
		@param	value	回転量(度)
		@note
		2D表示だとエフェクトが画面に対して前後に表示されるように作成されていた場合、予期せぬ見た目で表示されてしまうことがある。
		そのため、Y軸を中心にエフェクトを回転させ2Dとして違和感のない表示ができるようにする。
		*/
		void SetEffectRotation(float value);

		/**
		@brief	このオブジェクトの描画優先度を取得する。
		*/
		int GetDrawingPriority() const;

		/**
		@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int priority);
	};
}

#pragma once

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector3DF.h>

#include "ace.RenderedObject3D.h"

namespace ace
{
	class RenderedEffectObject3DProxy
		: public RenderedObject3DProxy
	{
	public:
		RenderedObject3DType GetObjectType() const override { return RenderedObject3DType::Effect; }
	};

	class RenderedEffectObject3D
		: public RenderedObject3D
	{
	private:
		static const int32_t GCThreshold = 32;

		std::vector<Effekseer::Handle>	m_handles;
		Effect*							m_effect;
		Renderer3D*						m_renderer = nullptr;
		bool							m_syncEffects = nullptr;

		RenderedEffectObject3DProxy*				proxy = nullptr;

	public:
		RenderedEffectObject3D(Graphics* graphics);
		virtual ~RenderedEffectObject3D();

		void Flip(float deltaTime) override;

		RenderedObject3DProxy* GetProxy() const override { return proxy; }

		void SetEffect(Effect* effect);

		/**
			@brief	設定されている全てのエフェクトを再生する。
		*/
		void Play();

		/**
			@brief	このオブジェクトから再生されたエフェクトを全て停止する。
		*/
		void Stop();

		/**
		@brief	このオブジェクトから再生されたエフェクトのルートを全て停止する。
		*/
		void StopRoot();

		/**
		@brief	このオブジェクトから再生されたエフェクトが再生中か取得する。
		*/
		bool IsPlaying();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得する。
		@return	フラグ
		*/
		bool GetSyncEffects() { return m_syncEffects; }

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか設定する。
		@param	value	フラグ
		*/
		void SetSyncEffects(bool value) { m_syncEffects = value; }

		void OnAdded(Renderer3D* renderer) override;

		void OnRemoving(Renderer3D* renderer) override;

		RenderedObject3DType GetObjectType() const override { return RenderedObject3DType::Effect; }
	};
}
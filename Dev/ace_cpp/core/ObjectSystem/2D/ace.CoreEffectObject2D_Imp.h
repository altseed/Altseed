
#include "ace.CoreEffectObject2D.h"
#include "ace.CoreObject2D_Imp.h"

namespace ace
{
	class CoreEffectObject2D_Imp
		: public CoreEffectObject2D
		, public CoreObject2D_Imp
		, public ReferenceObject
	{
	private:
		static const int32_t GCThreshold = 0;

		std::vector<Effekseer::Handle>	m_handles;
		Effect*							m_effect;
		bool							m_syncEffects = nullptr;
		Renderer2D_Imp*					m_renderer;

		/**
			@brief	Y軸回転(度)
		*/
		float							m_rotation = 0.0f;

		Effekseer::Matrix43 CalcEffectMatrix();

	public:
		CoreEffectObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreEffectObject2D_Imp();

		Object2DType GetObjectType() const override { return Object2DType::Effect; }

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
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得する。
		@return	フラグ
		*/
		bool GetSyncEffects() { return m_syncEffects; }

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか設定する。
		@param	value	フラグ
		*/
		void SetSyncEffects(bool value) { m_syncEffects = value; }

		float GetEffectRotation() const override;

		void SetEffectRotation(float value) override;

		void OnAdded(Renderer2D* renderer) override;

		void OnRemoving(Renderer2D* renderer) override;

		void Draw(Renderer2D* renderer) override;

		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
	};
}

#include "asd.CoreEffectObject2D.h"
#include "asd.CoreObject2D_Imp.h"

namespace asd
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
#if _WIN32
		bool							m_syncEffects = nullptr;
#elif __APPLE__
		bool							m_syncEffects = nullptr;
#else
        bool							m_syncEffects = false;
#endif
		Renderer2D_Imp*					m_renderer;
		int								m_drawingPtiority;

		/**
		@brief	X軸(3D)回転(度)
		*/
		float							m_rotationX = 0.0f;
		
		/**
		@brief	Y軸(3D)回転(度)
		*/
		float							m_rotationY = 0.0f;

		Effekseer::Matrix43 CalcEffectMatrix();

		Effect* GetEffect_() const override;

	public:
		CoreEffectObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreEffectObject2D_Imp();

		Object2DType GetObjectType() const override { return Object2DType::Effect; }

		void SetEffect(Effect* effect);

		/**
		@brief	設定されている全てのエフェクトを再生する。
		*/
		int32_t Play() override;

		/**
		@brief	このオブジェクトから再生されたエフェクトを全て停止する。
		*/
		void Stop();

		/**
		@brief	このオブジェクトから再生されたエフェクトのルートを全て停止する。
		*/
		void StopRoot();

		void Show() override;

		void Hide() override;


		bool GetIsPlaying() override;

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

		float GetEffectRotationX() const override;

		void SetEffectRotationX(float value) override;

		float GetEffectRotationY() const override;

		void SetEffectRotationY(float value) override;

		float GetEffectRotation() const override;

		void SetEffectRotation(float value) override;

		void OnAdding(Renderer2D* renderer) override;

		void OnRemoving(Renderer2D* renderer) override;

		void Draw(Renderer2D* renderer) override;

		int GetDrawingPriority() const override;
		void SetDrawingPriority(int priority) override;
		/*
		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
		*/

#include "asd.CoreObject2D_Imp_Methods.h"
	};
}

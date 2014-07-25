
#pragma once
#include "ace.Transition.h"

namespace ace
{
	/**
	@brief	フェードしながらシーン遷移する効果を適用するクラス
	*/
	class TransitionFade
		: public Transition
	{
	private:
		float fadeinDuration = 0;
		float fadeoutDuration = 0;
		float time = 0;

	public:
		/**
			@brief	コンストラクタ
			@param	fadeoutDuration	フェードアウトにかかる時間(1/60秒単位)
			@param	fadeinDuration	フェードインにかかる時間(1/60秒単位)
		*/
		TransitionFade(float fadeoutDuration, float fadeinDuration);
		virtual ~TransitionFade();

		void OnUpdate() override;
	};
}

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
			@param	fadeoutDuration	フェードアウトにかかる実時間(1/60秒を1とした値)
			@param	fadeinDuration	フェードインにかかる実時間(1/60秒を1とした値)
		*/
		TransitionFade(float fadeoutDuration, float fadeinDuration);
		virtual ~TransitionFade();

		void OnUpdate() override;
	};
}

#pragma once
#include "asd.Transition.h"

namespace asd
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
			@param	fadeoutDuration	フェードアウトにかかる実時間(秒)
			@param	fadeinDuration	フェードインにかかる実時間(秒)
		*/
		TransitionFade(float fadeoutDuration, float fadeinDuration);
		virtual ~TransitionFade();

		void OnUpdate() override;
	};
}
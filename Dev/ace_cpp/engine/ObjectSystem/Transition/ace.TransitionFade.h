
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
		int32_t fadeinDuration = 0;
		int32_t fadeoutDuration = 0;
		int32_t time = 0;

	public:
		/**
			@brief	コンストラクタ
			@param	fadeoutDuration	フェードアウトにかかるフレーム数
			@param	fadeinDuration	フェードインにかかるフレーム数
		*/
		TransitionFade(int32_t fadeoutDuration, int32_t fadeinDuration);
		virtual ~TransitionFade();

		void OnUpdate() override;
	};
}

#include "ace.TransitionFade.h"
#include "../../ace.Engine.h"

namespace ace
{
	TransitionFade::TransitionFade(float fadeoutDuration, float fadeinDuration)
		: fadeoutDuration(fadeoutDuration)
		, fadeinDuration(fadeinDuration)
	{

	}

	TransitionFade::~TransitionFade()
	{

	}

	void TransitionFade::OnUpdate()
	{
		float cp = 0.0f;
		float cn = 0.0f;

		if (time < fadeoutDuration)
		{
			cp = 1.0f - time / fadeoutDuration;
		}
		else if (time <= fadeoutDuration + fadeinDuration)
		{
			if (!IsSceneChanged())
			{
				ChangeScene();
			}

			cn = (time - fadeoutDuration) / fadeinDuration;
		}
		else
		{
			if (!IsSceneChanged())
			{
				ChangeScene();
			}

			if (!IsFinished())
			{
				Finish();
			}

			cn = 1.0f;
		}

		uint8_t cp_ = Clamp(cp * 255, 255, 0);
		uint8_t cn_ = Clamp(cn * 255, 255, 0);

		DrawRectangleWithPreviousScene(
			Vector2DF(0.0, 0.0),
			Vector2DF(1.0, 0.0),
			Vector2DF(1.0, 1.0),
			Vector2DF(0.0, 1.0),
			Color(255, 255, 255, cp_),
			Color(255, 255, 255, cp_),
			Color(255, 255, 255, cp_),
			Color(255, 255, 255, cp_),
			Vector2DF(0.0, 0.0),
			Vector2DF(1.0, 0.0),
			Vector2DF(1.0, 1.0),
			Vector2DF(0.0, 1.0));

		DrawRectangleWithNextScene(
			Vector2DF(0.0, 0.0),
			Vector2DF(1.0, 0.0),
			Vector2DF(1.0, 1.0),
			Vector2DF(0.0, 1.0),
			Color(255, 255, 255, cn_),
			Color(255, 255, 255, cn_),
			Color(255, 255, 255, cn_),
			Color(255, 255, 255, cn_),
			Vector2DF(0.0, 0.0),
			Vector2DF(1.0, 0.0),
			Vector2DF(1.0, 1.0),
			Vector2DF(0.0, 1.0));

		time += Engine::GetDeltaTime();
	}
}
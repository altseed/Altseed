using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// フェードしながらシーン遷移する効果を適用するクラス
	/// </summary>
	public class TransitionFade : Transition
	{
		int fadeinDuration = 0;
		int fadeoutDuration = 0;
		int time = 0;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="fadeoutDuration">フェードアウトにかかるフレーム数</param>
		/// <param name="fadeinDuration">フェードインにかかるフレーム数</param>
		public TransitionFade(int fadeoutDuration, int fadeinDuration)
		{
			this.fadeinDuration = fadeinDuration;
			this.fadeoutDuration = fadeoutDuration;
		}

		protected internal override void OnUpdate()
		{
			float cp = 0.0f;
			float cn = 0.0f;

			if (time < fadeoutDuration)
			{
				cp = 1.0f - (float)time / (float)fadeoutDuration;
			}
			else if (time < fadeoutDuration + fadeinDuration)
			{
				cn = (float)(time - fadeoutDuration) / (float)fadeinDuration;
			}

			if (time == fadeoutDuration)
			{
				ChangeScene();
			}

			if (time == fadeoutDuration + fadeinDuration)
			{
				Finish();
				cn = 1.0f;
			}

			byte cp_ = (byte)Math.Max(Math.Min(cp * 255, 255), 0);
			byte cn_ = (byte)Math.Max(Math.Min(cn * 255, 255), 0);

			DrawRectangleWithPreviousScene(
				new Vector2DF(0.0f, 0.0f),
				new Vector2DF(1.0f, 0.0f),
				new Vector2DF(1.0f, 1.0f),
				new Vector2DF(0.0f, 1.0f),
				new Color(255, 255, 255, cp_),
				new Color(255, 255, 255, cp_),
				new Color(255, 255, 255, cp_),
				new Color(255, 255, 255, cp_),
				new Vector2DF(0.0f, 0.0f),
				new Vector2DF(1.0f, 0.0f),
				new Vector2DF(1.0f, 1.0f),
				new Vector2DF(0.0f, 1.0f));

			DrawRectangleWithNextScene(
				new Vector2DF(0.0f, 0.0f),
				new Vector2DF(1.0f, 0.0f),
				new Vector2DF(1.0f, 1.0f),
				new Vector2DF(0.0f, 1.0f),
				new Color(255, 255, 255, cn_),
				new Color(255, 255, 255, cn_),
				new Color(255, 255, 255, cn_),
				new Color(255, 255, 255, cn_),
				new Vector2DF(0.0f, 0.0f),
				new Vector2DF(1.0f, 0.0f),
				new Vector2DF(1.0f, 1.0f),
				new Vector2DF(0.0f, 1.0f));

			time++;
		}
	}
}

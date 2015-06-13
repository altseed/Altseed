using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Graphics.PostEffect
{
	/// <summary>
	/// ポストエフェクトを適用する。
	/// </summary>
	class PostEffect : ISample
	{
		public void Run()
		{
			// 説明
			Console.WriteLine("Z : グレースケール追加");
			Console.WriteLine("X : セピア調追加");
			Console.WriteLine("C : ガウスぼかし追加");
			Console.WriteLine("V : ライトブルーム追加");
			Console.WriteLine("A : 全てのポストエフェクト消去");

			// aceを初期化する
			asd.Engine.Initialize("PostEffect", 640, 480, new asd.EngineOption());

			// シーン等を初期化する
			var scene = new asd.Scene();
			var layer = new asd.Layer2D();
			var effectObj = new asd.EffectObject2D();
			var effect = asd.Engine.Graphics.CreateEffect("Data/Effect/magic.efk");

			scene.HDRMode = true; ;
			scene.AddLayer(layer);
			layer.AddObject(effectObj);
			asd.Engine.ChangeScene(scene);

			effectObj.Scale = new asd.Vector2DF(50, 50);
			effectObj.Position = new asd.Vector2DF(320, 240);
			effectObj.Effect = effect;
			effectObj.Play();

			// aceが進行可能かチェックする。
			while (asd.Engine.DoEvents())
			{
				// aceを更新する。
				asd.Engine.Update();

				// Aを押したらポストエフェクトを全て破棄する。
				if (asd.Engine.Keyboard.GetKeyState(asd.Keys.A) == asd.KeyState.Push)
				{
					layer.ClearPostEffects();
				}

				// Zを押したらグレースケールを追加する。
				if (asd.Engine.Keyboard.GetKeyState(asd.Keys.Z) == asd.KeyState.Push)
				{
					var posteffect = new asd.PostEffectGrayScale();
					layer.AddPostEffect(posteffect);
				}

				// Xを押したらセピアを追加する。
				if (asd.Engine.Keyboard.GetKeyState(asd.Keys.X) == asd.KeyState.Push)
				{
					var posteffect = new asd.PostEffectSepia();
					layer.AddPostEffect(posteffect);
				}

				// Cを押したらガウスぼかしを追加する。
				if (asd.Engine.Keyboard.GetKeyState(asd.Keys.C) == asd.KeyState.Push)
				{
					var posteffect = new asd.PostEffectGaussianBlur();
					posteffect.Intensity = 5.0f;
					layer.AddPostEffect(posteffect);
				}

				// Vを押したらライトブルームを追加する。
				if (asd.Engine.Keyboard.GetKeyState(asd.Keys.V) == asd.KeyState.Push)
				{
					var posteffect = new asd.PostEffectLightBloom();
					posteffect.Intensity = 10.0f;
					posteffect.Exposure = 1.0f;
					posteffect.Threshold = 0.3f;
					layer.AddPostEffect(posteffect);
				}
			}

			// aceを終了する。
			asd.Engine.Terminate();
		}
	}
}

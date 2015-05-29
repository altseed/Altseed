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

			// AC-Engineを初期化する。
			ace.Engine.Initialize("PostEffect", 640, 480, new ace.EngineOption());

			// シーン等を初期化する
			var scene = new ace.Scene();
			var layer = new ace.Layer2D();
			var effectObj = new ace.EffectObject2D();
			var effect = ace.Engine.Graphics.CreateEffect("Data/Effect/magic.efk");

			scene.HDRMode = true; ;
			scene.AddLayer(layer);
			layer.AddObject(effectObj);
			ace.Engine.ChangeScene(scene);

			effectObj.Scale = new ace.Vector2DF(50, 50);
			effectObj.Position = new ace.Vector2DF(320, 240);
			effectObj.Effect = effect;
			effectObj.Play();

			// AC-Engineのウインドウが閉じられていないか確認する。
			while (ace.Engine.DoEvents())
			{
				// AC-Engineを更新する。
				ace.Engine.Update();

				// Aを押したらポストエフェクトを全て破棄する。
				if (ace.Engine.Keyboard.GetKeyState(ace.Keys.A) == ace.KeyState.Push)
				{
					layer.ClearPostEffects();
				}

				// Zを押したらグレースケールを追加する。
				if (ace.Engine.Keyboard.GetKeyState(ace.Keys.Z) == ace.KeyState.Push)
				{
					var posteffect = new ace.PostEffectGrayScale();
					layer.AddPostEffect(posteffect);
				}

				// Xを押したらセピアを追加する。
				if (ace.Engine.Keyboard.GetKeyState(ace.Keys.X) == ace.KeyState.Push)
				{
					var posteffect = new ace.PostEffectSepia();
					layer.AddPostEffect(posteffect);
				}

				// Cを押したらガウスぼかしを追加する。
				if (ace.Engine.Keyboard.GetKeyState(ace.Keys.C) == ace.KeyState.Push)
				{
					var posteffect = new ace.PostEffectGaussianBlur();
					posteffect.Intensity = 5.0f;
					layer.AddPostEffect(posteffect);
				}

				// Vを押したらライトブルームを追加する。
				if (ace.Engine.Keyboard.GetKeyState(ace.Keys.V) == ace.KeyState.Push)
				{
					var posteffect = new ace.PostEffectLightBloom();
					posteffect.Intensity = 10.0f;
					posteffect.Exposure = 1.0f;
					posteffect.Threshold = 0.3f;
					layer.AddPostEffect(posteffect);
				}
			}

			// AC-Engineの終了処理をする。
			ace.Engine.Terminate();
		}
	}
}


/// <summary>
/// 様々なポストエフェクトを適用するサンプルです。
/// </summary>
class PostEffect : ISample
{
	public void Run()
	{
		// 説明
		System.Console.WriteLine("Z : グレースケール追加");
		System.Console.WriteLine("X : セピア調追加");
		System.Console.WriteLine("C : ガウスぼかし追加");
		System.Console.WriteLine("V : ライトブルーム追加");
		System.Console.WriteLine("A : 全てのポストエフェクト消去");

		// Altseedを初期化する。
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

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
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

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}


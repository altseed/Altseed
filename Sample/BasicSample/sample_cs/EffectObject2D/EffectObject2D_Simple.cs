
namespace sample_cs
{
	class EffectObject2D_Simple : ISample
	{
		// エフェクトを再生するサンプル。
		public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("EffectObject2D", 640, 480, new asd.EngineOption());

			// エフェクトを読み込む
			var effect = asd.Engine.Graphics.CreateEffect("Data/Effect/magic.efk");

			// エフェクトオブジェクトを生成する
			var effectObj = new asd.EffectObject2D();

			// エフェクトオブジェクトをエンジンに追加する
			asd.Engine.AddObject2D(effectObj);

			// エフェクトを設定する
			effectObj.Effect = effect;

			// エフェクトの位置、大きさを指定する
			effectObj.Position = new asd.Vector2DF(320, 240);
			effectObj.Scale = new asd.Vector2DF(50, 50);

			// エフェクトを再生する
			effectObj.Play();

			// Altseedのウインドウが閉じられていないか確認する。
			while (asd.Engine.DoEvents())
			{
				// Altseedを更新する。
				asd.Engine.Update();
			}

			// Altseedの終了処理をする。
			asd.Engine.Terminate();
		}
	}
}

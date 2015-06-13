
/// <summary>
/// EffectObject2Dのサンプル。エフェクトを表示しています。
/// </summary>
class EffectObject2D : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("EffectObject2D", 640, 480, new asd.EngineOption());

		// シーンを生成する
		var scene = new asd.Scene();

		// レイヤーを生成する
		var layer = new asd.Layer2D();

		// シーンにレイヤーを追加する
		scene.AddLayer(layer);

		// シーンを切り替える
		asd.Engine.ChangeScene(scene);

		// エフェクトを読み込む
		var effect = asd.Engine.Graphics.CreateEffect("Data/Effect/magic.efk");

		// エフェクトオブジェクトを生成する
		var effectObj = new asd.EffectObject2D();

		// エフェクトをレイヤーに追加する
		layer.AddObject(effectObj);

		// エフェクトの位置、大きさを指定する
		effectObj.Position = new asd.Vector2DF(320, 240);
		effectObj.Scale = new asd.Vector2DF(50, 50);

		// エフェクトを設定する
		effectObj.Effect = effect;

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

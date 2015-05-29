
/// <summary>
/// EffectObject2Dのサンプル。エフェクトを表示しています。
/// </summary>
class EffectObject2D : ISample
{
	public void Run()
	{
		// AC-Engineを初期化する。
		ace.Engine.Initialize("EffectObject2D", 640, 480, new ace.EngineOption());

		// シーンを生成する
		var scene = new ace.Scene();

		// レイヤーを生成する
		var layer = new ace.Layer2D();

		// シーンにレイヤーを追加する
		scene.AddLayer(layer);

		// シーンを切り替える
		ace.Engine.ChangeScene(scene);

		// エフェクトを読み込む
		var effect = ace.Engine.Graphics.CreateEffect("Data/Effect/magic.efk");

		// エフェクトオブジェクトを生成する
		var effectObj = new ace.EffectObject2D();

		// エフェクトをレイヤーに追加する
		layer.AddObject(effectObj);

		// エフェクトの位置、大きさを指定する
		effectObj.Position = new ace.Vector2DF(320, 240);
		effectObj.Scale = new ace.Vector2DF(50, 50);

		// エフェクトを設定する
		effectObj.Effect = effect;

		// エフェクトを再生する
		effectObj.Play();

		// AC-Engineのウインドウが閉じられていないか確認する。
		while (ace.Engine.DoEvents())
		{
			// AC-Engineを更新する。
			ace.Engine.Update();
		}

		// AC-Engineの終了処理をする。
		ace.Engine.Terminate();
	}
}


/// <summary>
/// Object2DComponentを用いて、オブジェクトのパラメーターを変更するサンプルです。
/// </summary>
class Object2DComponent : ISample
{
	/// <summary>
	/// オブジェクトを回転させるコンポーネント
	/// </summary>
	class RotateComponent : asd.Object2DComponent
	{
		protected override void OnUpdate()
		{
			// 毎フレーム、オブジェクトの角度を回転させる
			Owner.Angle += 2;
		}
	}

	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Object2DComponent", 640, 480, new asd.EngineOption());

		// 使用するシーン、レイヤー、オブジェクト、コンポーネントを生成する。
		var scene = new asd.Scene();
		var layer = new asd.Layer2D();
		var obj = new asd.TextureObject2D();
		var component = new RotateComponent();

		// シーンを変更する。
		asd.Engine.ChangeScene(scene);

		// シーンにレイヤーを追加する。
		scene.AddLayer(layer);

		// レイヤーにオブジェクトを追加する。
		layer.AddObject(obj);

		// オブジェクトに回転コンポーネントをコンポーネント名"Rotate"で追加する。
		obj.AddComponent(component, "Rotate");

		// 画像を読み込む。
		var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Block1.png");

		// オブジェクトに画像を設定する。
		obj.Texture = texture;

		// オブジェクトの位置を設定する。
		obj.Position = new asd.Vector2DF(320, 240);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
		}

		//Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}


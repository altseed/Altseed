
/// <summary>
/// Object2DComponentを用いて、オブジェクトのパラメーターを変更するサンプルです。
/// </summary>
class Object2DComponent : ISample
{
	/// <summary>
	/// オブジェクトを回転させるコンポーネント
	/// </summary>
	class RotateComponent : ace.Object2DComponent
	{
		protected override void OnUpdate()
		{
			// 毎フレーム、オブジェクトの角度を回転させる
			Owner.Angle += 2;
		}
	}

	public void Run()
	{
		// AC-Engineを初期化する。
		ace.Engine.Initialize("Object2DComponent", 640, 480, new ace.EngineOption());

		// 使用するシーン、レイヤー、オブジェクト、コンポーネントを生成する。
		var scene = new ace.Scene();
		var layer = new ace.Layer2D();
		var obj = new ace.TextureObject2D();
		var component = new RotateComponent();

		// シーンを変更する。
		ace.Engine.ChangeScene(scene);

		// シーンにレイヤーを追加する。
		scene.AddLayer(layer);

		// レイヤーにオブジェクトを追加する。
		layer.AddObject(obj);

		// オブジェクトに回転コンポーネントをコンポーネント名"Rotate"で追加する。
		obj.AddComponent(component, "Rotate");

		// 画像を読み込む。
		var texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Block1.png");

		// オブジェクトに画像を設定する。
		obj.Texture = texture;

		// オブジェクトの位置を設定する。
		obj.Position = new ace.Vector2DF(320, 240);

		// AC-Engineのウインドウが閉じられていないか確認する。
		while (ace.Engine.DoEvents())
		{
			// AC-Engineを更新する。
			ace.Engine.Update();
		}

		//AC-Engineの終了処理をする。
		ace.Engine.Terminate();
	}
}


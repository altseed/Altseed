
namespace sample_cs
{
	class Object2DComponent_Custom : ISample
	{
		// オブジェクトを回転させるコンポーネント。
		class RotateComponent : asd.Object2DComponent
		{
			protected override void OnUpdate()
			{
				// 毎フレーム、オブジェクトの角度を回転させる
				Owner.Angle += 2;
			}
		}

		// Object2DComponentを用いて、オブジェクトのパラメーターを変更するサンプル。
		public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("Object2DComponent_Custom", 640, 480, new asd.EngineOption());

			// オブジェクトを生成する。
			var obj = new asd.TextureObject2D();

			// 画像を読み込む。
			var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

			// オブジェクトに画像を設定する。
			obj.Texture = texture;

			// オブジェクトの位置を設定する。
			obj.Position = new asd.Vector2DF(320, 240);

			// エンジンにオブジェクトを追加する。
			asd.Engine.AddObject2D(obj);

			// 回転コンポーネントを生成する。
			var component = new RotateComponent();

			// オブジェクトに回転コンポーネントをコンポーネント名"Rotate"で追加する。
			obj.AddComponent(component, "Rotate");

			// Altseedのウインドウが閉じられていないか確認する。
			while (asd.Engine.DoEvents())
			{
				// Altseedを更新する。
				asd.Engine.Update();
				Recorder.CaptureScreen("Object2DComponent_Custom", 0, 30, 0.5f, 0.5f);
			}

			//Altseedの終了処理をする。
			asd.Engine.Terminate();
		}
	}
}

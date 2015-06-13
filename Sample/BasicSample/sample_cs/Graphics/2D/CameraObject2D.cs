
/// <summary>
/// CameraObject2Dのサンプル。マウスポインタの先を拡大表示しています。
/// </summary>
public class CameraObject2D : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("CameraObject2D", 640, 480, new asd.EngineOption());

		// シーンを生成する。
		var scene = new asd.Scene();

		// レイヤーを生成する。
		var layer = new asd.Layer2D();

		// シーンにレイヤーを追加する。
		scene.AddLayer(layer);

		// シーンを切り替える。
		asd.Engine.ChangeScene(scene);

		// 画面にテクスチャを表示するオブジェクトを生成する。
		{
			var tex0 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
			var obj0 = new asd.TextureObject2D();
			obj0.Texture = tex0;
			layer.AddObject(obj0);
		}

		// 画面全体を表示するカメラを生成する。
		{
			var baseCameraObj = new asd.CameraObject2D();
			baseCameraObj.Src = new asd.RectI(0, 0, 640, 480);
			baseCameraObj.Dst = new asd.RectI(0, 0, 640, 480);
			layer.AddObject(baseCameraObj);
		}

		// マウスポインタの周辺を拡大表示するカメラを生成する。
		var cameraObj = new asd.CameraObject2D();
		layer.AddObject(cameraObj);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// マウスポインタの位置を取得する。
			var pos = asd.Engine.Mouse.Position;

			// 拡大用カメラの描画元を指定する。
			cameraObj.Src = new asd.RectI((int)(pos.X) - 25, (int)(pos.Y) - 25, 50, 50);

			// ポインタを中心に100x100の拡大画像を表示する。
			cameraObj.Dst = new asd.RectI((int)(pos.X) - 50, (int)(pos.Y) - 50, 100, 100);

			// Altseedを更新する。
			asd.Engine.Update();
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}


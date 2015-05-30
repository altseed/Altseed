
/// <summary>
/// CameraObject2Dのサンプル。マウスポインタの先を拡大表示しています。
/// </summary>
public class CameraObject2D : ISample
{
	public void Run()
	{
		// AC-Engineを初期化する。
		ace.Engine.Initialize("CameraObject2D", 640, 480, new ace.EngineOption());

		// シーンを生成する。
		var scene = new ace.Scene();

		// レイヤーを生成する。
		var layer = new ace.Layer2D();

		// シーンにレイヤーを追加する。
		scene.AddLayer(layer);

		// シーンを切り替える。
		ace.Engine.ChangeScene(scene);

		// 画面にテクスチャを表示するオブジェクトを生成する。
		{
			var tex0 = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
			var obj0 = new ace.TextureObject2D();
			obj0.Texture = tex0;
			layer.AddObject(obj0);
		}

		// 画面全体を表示するカメラを生成する。
		{
			var baseCameraObj = new ace.CameraObject2D();
			baseCameraObj.Src = new ace.RectI(0, 0, 640, 480);
			baseCameraObj.Dst = new ace.RectI(0, 0, 640, 480);
			layer.AddObject(baseCameraObj);
		}

		// マウスポインタの周辺を拡大表示するカメラを生成する。
		var cameraObj = new ace.CameraObject2D();
		layer.AddObject(cameraObj);

		// AC-Engineのウインドウが閉じられていないか確認する。
		while (ace.Engine.DoEvents())
		{
			// マウスポインタの位置を取得する。
			var pos = ace.Engine.Mouse.Position;

			// 拡大用カメラの描画元を指定する。
			cameraObj.Src = new ace.RectI((int)(pos.X) - 25, (int)(pos.Y) - 25, 50, 50);

			// ポインタを中心に100x100の拡大画像を表示する。
			cameraObj.Dst = new ace.RectI((int)(pos.X) - 50, (int)(pos.Y) - 50, 100, 100);

			// AC-Engineを更新する。
			ace.Engine.Update();
		}

		// AC-Engineの終了処理をする。
		ace.Engine.Terminate();
	}
}


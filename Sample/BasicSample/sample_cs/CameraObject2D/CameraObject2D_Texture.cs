
/// <summary>
/// カメラで撮影した結果をテクスチャとして表示するサンプル。
/// </summary>
public class CameraObject2D_Texture : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("CameraObject2D_Texture", 640, 480, new asd.EngineOption());

		// 画像を読み込む。
		var tex = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

		// テクスチャを描画するオブジェクトを設定する。
		var obj1 = new asd.TextureObject2D();
		obj1.Texture = tex;
		obj1.Position = new asd.Vector2DF(200, 10);
		obj1.Angle = 45.0f;
		obj1.Scale = new asd.Vector2DF(0.5f, 0.5f);

		// グループを設定する。
		obj1.CameraGroup = 1;
		asd.Engine.AddObject2D(obj1);

		// カメラを設定する。
		var cameraOffscreen = new asd.CameraObject2D();
		cameraOffscreen.Src = new asd.RectI(0, 0, 640, 480);
		cameraOffscreen.Dst = new asd.RectI(0, 0, 640, 480);

		// グループを設定する。
		cameraOffscreen.CameraGroup = 1;

		// オフスクリーンモードであることを設定する。
		cameraOffscreen.IsOffsecreenMode = true;

		asd.Engine.AddObject2D(cameraOffscreen);

		// テクスチャを描画するオブジェクトを設定する。
		var obj2 = new asd.TextureObject2D();
		obj2.Texture = cameraOffscreen.Texture;
		obj2.Position = new asd.Vector2DF(310, 10);
		obj2.Scale = new asd.Vector2DF(0.5f, 0.5f);

		// グループを設定する。
		obj2.CameraGroup = 2;
		asd.Engine.AddObject2D(obj2);

		// カメラを設定する。
		var camera = new asd.CameraObject2D();
		camera.Src = new asd.RectI(0, 0, 640, 480);
		camera.Dst = new asd.RectI(0, 0, 640, 480);

		// グループを設定する。
		camera.CameraGroup = 2;
		asd.Engine.AddObject2D(camera);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("CameraObject2D_Texture", 30);
		}

		// Altseedを終了する。
		asd.Engine.Terminate();
	}
}


/// <summary>
/// Transitionを用いて、シーンの変更の見栄えを変更するサンプルです。
/// </summary>
public class Transition : ISample
{
	/// <summary>
	/// Scene1の画像を表示するシーン
	/// </summary>
	class Scene1 : asd.Scene
	{
		public Scene1()
		{
			// レイヤーを追加する。
			var layer = new asd.Layer2D();
			AddLayer(layer);

			// Scene1を描画するインスタンスを生成してレイヤーに追加する。
			var obj = new asd.TextureObject2D();
			obj.Texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Scene1.png");

			layer.AddObject(obj);
		}

		protected override void OnUpdated()
		{
			// マウスが左クリックされた時にフェード付きでScene2に遷移する。
			if (asd.Engine.Mouse.LeftButton.ButtonState == asd.MouseButtonState.Push)
			{
				var scene2 = new Scene2();
				asd.Engine.ChangeSceneWithTransition(scene2, new asd.TransitionFade(1.0f, 1.0f));
			}
		}
	}

	/// <summary>
	/// Scene2の画像を表示するシーン
	/// </summary>
	class Scene2 : asd.Scene
	{
		public Scene2()
		{
			// レイヤーを追加する。
			var layer = new asd.Layer2D();
			AddLayer(layer);

			// Scene2を描画するインスタンスを生成してレイヤーに追加する。
			var obj = new asd.TextureObject2D();
			obj.Texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Scene2.png");

			layer.AddObject(obj);
		}

		protected override void OnUpdated()
		{
			// マウスが左クリックされた時にScene1へ遷移する。
			if (asd.Engine.Mouse.LeftButton.ButtonState == asd.MouseButtonState.Push)
			{
				var scene1 = new Scene1();
				asd.Engine.ChangeScene(scene1);
			}
		}
	}

	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Transition", 640, 480, new asd.EngineOption());

		// シーンをScene1に変更する。
		var scene = new Scene1();
		asd.Engine.ChangeScene(scene);

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

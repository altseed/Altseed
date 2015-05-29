
/// <summary>
/// Transitionを用いて、シーンの変更の見栄えを変更するサンプルです。
/// </summary>
public class Transition : ISample
{
	/// <summary>
	/// Scene1の画像を表示するシーン
	/// </summary>
	class Scene1 : ace.Scene
	{
		public Scene1()
		{
			// レイヤーを追加する。
			var layer = new ace.Layer2D();
			AddLayer(layer);

			// Scene1を描画するインスタンスを生成してレイヤーに追加する。
			var obj = new ace.TextureObject2D();
			obj.Texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Scene1.png");

			layer.AddObject(obj);
		}

		protected override void OnUpdated()
		{
			// マウスが左クリックされた時にフェード付きでScene2に遷移する。
			if (ace.Engine.Mouse.LeftButton.ButtonState == ace.MouseButtonState.Push)
			{
				var scene2 = new Scene2();
				ace.Engine.ChangeSceneWithTransition(scene2, new ace.TransitionFade(1.0f, 1.0f));
			}
		}
	}

	/// <summary>
	/// Scene2の画像を表示するシーン
	/// </summary>
	class Scene2 : ace.Scene
	{
		public Scene2()
		{
			// レイヤーを追加する。
			var layer = new ace.Layer2D();
			AddLayer(layer);

			// Scene2を描画するインスタンスを生成してレイヤーに追加する。
			var obj = new ace.TextureObject2D();
			obj.Texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Scene2.png");

			layer.AddObject(obj);
		}

		protected override void OnUpdated()
		{
			// マウスが左クリックされた時にScene1へ遷移する。
			if (ace.Engine.Mouse.LeftButton.ButtonState == ace.MouseButtonState.Push)
			{
				var scene1 = new Scene1();
				ace.Engine.ChangeScene(scene1);
			}
		}
	}

	public void Run()
	{
		// AC-Engineを初期化する。
		ace.Engine.Initialize("Transition", 640, 480, new ace.EngineOption());

		// シーンをScene1に変更する。
		var scene = new Scene1();
		ace.Engine.ChangeScene(scene);

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

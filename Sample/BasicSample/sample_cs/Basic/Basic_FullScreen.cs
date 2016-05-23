
namespace sample_cs
{
	class Basic_FullScreen : ISample
	{
		public void Run()
		{
			// フルスクリーンで起動するように初期化オプションを生成する。
			var option = new asd.EngineOption
			{
				IsFullScreen = true
			};

			// 作成した初期化オプションを用いてAltseedを初期化する。
			asd.Engine.Initialize("Empty", 640, 480, option);

			// 操作説明文を文字列オブジェクトとして作成。CreateDynamicFontで作成したフォントオブジェクトを用いる
			var font = asd.Engine.Graphics.CreateDynamicFont("", 20, new asd.Color(255, 255, 255), 2, new asd.Color(0, 0, 0));
			var obj = new asd.TextObject2D
			{
				Font = font,
				Text = "Escキーで終了"
			};

			// 操作説明文のオブジェクトをエンジンに登録する。
			asd.Engine.AddObject2D(obj);

			// Altseedのウインドウが閉じられていないか確認する。
			while(asd.Engine.DoEvents())
			{
				// Altseedを更新する。
				asd.Engine.Update();

				// Escキーが押されていたら
				if(asd.Engine.Keyboard.GetKeyState(asd.Keys.Escape) == asd.KeyState.Push)
				{
					// ゲームループを抜ける
					break;
				}
			}

			// Altseedの終了処理をする。
			asd.Engine.Terminate();
		}

		public string Description
		{
			get { return "アプリケーションをフルスクリーンで起動するサンプル。"; }
		}

		public string Title
		{
			get { return "フルスクリーンモード"; }
		}
	}
}

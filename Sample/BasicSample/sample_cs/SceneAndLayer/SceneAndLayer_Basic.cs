namespace sample_cs
{
	class SceneAndLayer_Basic : ISample
    {
        public string Description
        {
            get { return "シーンとレイヤーを手動で生成して操作するサンプル。"; }
        }
		public string Title
		{
			get { return "シーンとレイヤーの基本"; }
		}

        // シーンとレイヤーを手動で生成する。
        public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("SceneAndLayer_Basic", 640, 480, new asd.EngineOption());

			// 画像を読み込む。
			var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

			// シーンクラスのインスタンスを生成する。
			var scene = new asd.Scene();

			// レイヤークラスのインスタンスを生成する。
			var layer = new asd.Layer2D();

			//画像描画オブジェクトのインスタンスを生成する。
			var obj = new asd.TextureObject2D();

			// オブジェクトの位置とテクスチャを設定する。
			obj.Position = new asd.Vector2DF(50, 50);
			obj.Texture = texture;
			
			// 描画するシーンを指定する。
			asd.Engine.ChangeScene(scene);

			// 描画するレイヤーをシーンに追加する。
			scene.AddLayer(layer);

			// 描画するオブジェクトをレイヤーに追加する。
			layer.AddObject(obj);
			
			// Altseedのウインドウが閉じられていないか確認する。
			while (asd.Engine.DoEvents())
			{
				// Altseedを更新する。
				asd.Engine.Update();
			}
			
			// Altseedを終了する。
			asd.Engine.Terminate();
		}
	}
}

﻿
class PostEffect_GaussianBlur : ISample
{
	public string Description
	{
		get { return "レイヤーの描画結果にガウスぼかしをかけるサンプル。"; }
	}
	public string Title
	{
		get { return "ガウスぼかし"; }
	}
	public string ClassName
	{
		get { return "PostEffect_GaussianBlur"; }
	}

	// レイヤーにガウスぼかしのポストエフェクトを適用する。
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("PostEffect_GaussianBlur", 640, 480, new asd.EngineOption());

		var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

		// シーンクラスのインスタンスを生成する。
		var scene = new asd.Scene();

		// レイヤークラスのインスタンスを生成する。
		var layer = new asd.Layer2D();

		// 画像描画オブジェクトのインスタンスを生成する。
		var obj = new asd.TextureObject2D();

		// オブジェクトの位置とテクスチャを設定する。
		obj.Position = new asd.Vector2DF(50, 50);
		obj.Texture = texture;

		// シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
		asd.Engine.ChangeScene(scene);
		scene.AddLayer(layer);
		layer.AddObject(obj);

		// レイヤーにガウスぼかしのポストエフェクトを適用。
		layer.AddPostEffect(new asd.PostEffectGaussianBlur());

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
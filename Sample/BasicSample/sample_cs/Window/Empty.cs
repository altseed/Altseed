
/// <summary>
/// 空のウインドウを表示するサンプルです。
/// </summary>
class Empty : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Empty", 640, 480, new asd.EngineOption());

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


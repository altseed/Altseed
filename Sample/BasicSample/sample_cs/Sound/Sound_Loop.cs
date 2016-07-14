
class Sound_Loop : ISample
{
	public string Description
	{
		get { return "BGMをループ再生するサンプルです。"; }
	}
	public string Title
	{
		get { return "BGMのループ再生"; }
	}
	public string ClassName
	{
		get { return "Sound_Loop"; }
	}

	// BGMをループ再生するサンプル。
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Sound_Loop", 640, 480, new asd.EngineOption());

		// 音声ファイルを読み込む。BGMの場合、第２引数を false に設定することで、再生しながらファイルを解凍することが推奨されている。
		asd.SoundSource bgm1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/bgm1.ogg", false);

		// 音声のループを有効にする。
		bgm1.IsLoopingMode = true;

		// 音声のループ始端を1秒に、ループ終端を6秒に設定する。
		bgm1.LoopStartingPoint = 1.0f;
		bgm1.LoopEndPoint = 6.0f;

		// 音声を再生する。
		int id_bgm1 = asd.Engine.Sound.Play(bgm1);

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
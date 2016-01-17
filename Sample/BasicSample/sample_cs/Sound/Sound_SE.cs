
namespace sample_cs
{
	class Sound_SE : ISample
    {
        public string Description
        {
            get { return ""; }
        }

        // Soundを用いて音を鳴らすサンプル。
        public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("Sound_SE", 640, 480, new asd.EngineOption());

			// 音ファイルを読み込む。SEの場合、第２引数を true に設定することで、この場でファイルを解凍することが推奨されている。
			asd.SoundSource se1 = asd.Engine.Sound.CreateSoundSource("Data/Sound/se1.wav", true);

			// 音を再生する。
			int id_se1 = asd.Engine.Sound.Play(se1);

			// Altseedのウインドウが閉じられていないか確認する。
			while (asd.Engine.DoEvents())
			{
				// Altseedを更新する。
				asd.Engine.Update();

				// 音が再生終了してるか調べる。
				if (!asd.Engine.Sound.GetIsPlaying(id_se1))
				{
					break;
				}
			}

			// Altseedの終了処理をする。
			asd.Engine.Terminate();
		}
	}
}


#include <Altseed.h>

// Soundを用いてBGMを鳴らすサンプルです。
void Sound_BGM()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Sound_BGM").c_str(), 640, 480, asd::EngineOption());

	// 音声ファイルを読み込む。
	std::shared_ptr<asd::SoundSource> bgm1 = asd::Engine::GetSound()->CreateSoundSource(asd::ToAString("Data/Sound/bgm1.ogg").c_str(), false);

	// 音声を再生する。
	int id_bgm1 = asd::Engine::GetSound()->Play(bgm1);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();

		// 音が再生終了してるか調べる。
		if (!asd::Engine::GetSound()->GetIsPlaying(id_bgm1))
		{
			break;
		}
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();
}

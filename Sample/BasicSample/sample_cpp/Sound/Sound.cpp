
#include <Altseed.h>

// Soundを用いて音を鳴らすサンプルです。
void Sound()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Sound").c_str(), 640, 480, asd::EngineOption());

	// 音ファイルを読み込む。
	std::shared_ptr<asd::SoundSource> se1 = asd::Engine::GetSound()->CreateSoundSource(asd::ToAString("Data/Sound/se1.wav").c_str(), true);
	std::shared_ptr<asd::SoundSource> bgm1 = asd::Engine::GetSound()->CreateSoundSource(asd::ToAString("Data/Sound/bgm1.ogg").c_str(), false);

	// 音を再生する。
	int id_se1 = asd::Engine::GetSound()->Play(se1);
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

	return;
}

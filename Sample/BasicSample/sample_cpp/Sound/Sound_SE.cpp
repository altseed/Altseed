
#include <Altseed.h>

// Soundを用いて効果音を鳴らすサンプルです。
void Sound_SE()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Sound").c_str(), 640, 480, asd::EngineOption());

	// 音ファイルを読み込む。
	std::shared_ptr<asd::SoundSource> se1 = asd::Engine::GetSound()->CreateSoundSource(asd::ToAString("Data/Sound/se1.wav").c_str(), true);

	// 音を再生する。
	int id_se1 = asd::Engine::GetSound()->Play(se1);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();

		// 音が再生終了してるか調べる。
		if (!asd::Engine::GetSound()->GetIsPlaying(id_se1))
		{
			break;
		}
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();
}

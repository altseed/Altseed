
#include <Base.h>

/**
@brief	音を鳴らすサンプル
*/
void Sound()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Sound").c_str(), 640, 480, asd::EngineOption());

	// ファイルを読み込む。
	std::shared_ptr<asd::SoundSource> se1 = asd::Engine::GetSound()->CreateSoundSource(asd::ToAString("Data/Sound/se1.wav").c_str(), true);
	std::shared_ptr<asd::SoundSource> bgm1 = asd::Engine::GetSound()->CreateSoundSource(asd::ToAString("Data/Sound/bgm1.ogg").c_str(), false);

	// 再生する。
	int id_se1 = asd::Engine::GetSound()->Play(se1);
	int id_bgm1 = asd::Engine::GetSound()->Play(bgm1);

	// Altseedが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		// aceを更新する。
		asd::Engine::Update();

		// 音が再生終了してるか調べる。
		if (!asd::Engine::GetSound()->GetIsPlaying(id_bgm1))
		{
			break;
		}
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}


#include <ace.h>

// Soundを用いて音を鳴らすサンプルです。
void Sound()
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("Sound").c_str(), 640, 480, ace::EngineOption());

	// 音ファイルを読み込む。
	std::shared_ptr<ace::SoundSource> se1 = ace::Engine::GetSound()->CreateSoundSource(ace::ToAString("Data/Sound/se1.wav").c_str(), true);
	std::shared_ptr<ace::SoundSource> bgm1 = ace::Engine::GetSound()->CreateSoundSource(ace::ToAString("Data/Sound/bgm1.ogg").c_str(), false);

	// 音を再生する。
	int id_se1 = ace::Engine::GetSound()->Play(se1);
	int id_bgm1 = ace::Engine::GetSound()->Play(bgm1);

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		// AC-Engineを更新する。
		ace::Engine::Update();

		// 音が再生終了してるか調べる。
		if (!ace::Engine::GetSound()->GetIsPlaying(id_bgm1))
		{
			break;
		}
	}

	// AC-Engineの終了処理をする。
	ace::Engine::Terminate();

	return;
}

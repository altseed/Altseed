
#include <Base.h>

/**
@brief	音を鳴らす。
*/
void Sound()
{
	// aceを初期化する
	ace::Engine::Initialize(L"Sound", 640, 480, ace::EngineOption());

	// ファイルを読み込む
	std::shared_ptr<ace::SoundSource> se1 = ace::Engine::GetSound()->CreateSoundSource(ace::ToAString("Data/Sound/se1.wav").c_str(), true);
	std::shared_ptr<ace::SoundSource> bgm1 = ace::Engine::GetSound()->CreateSoundSource(ace::ToAString("Data/Sound/bgm1.ogg").c_str(), false);

	// 再生する
	int id_se1 = ace::Engine::GetSound()->Play(se1);
	int id_bgm1 = ace::Engine::GetSound()->Play(bgm1);

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		// aceを更新する。
		ace::Engine::Update();

		// 音が再生終了してるか調べる
		if (!ace::Engine::GetSound()->IsPlaying(id_bgm1))
		{
			break;
		}
	}

	// aceを終了する。
	ace::Engine::Terminate();
}

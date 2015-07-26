
#include <Altseed.h>

// BGMをループ再生するサンプル。
void Sound_Loop()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Sound_Loop").c_str(), 640, 480, asd::EngineOption());

	// 音声ファイルを読み込む。
	std::shared_ptr<asd::SoundSource> bgm1 = asd::Engine::GetSound()->CreateSoundSource(asd::ToAString("Data/Sound/bgm1.ogg").c_str(), false);

	// 音声のループを有効にする。
	bgm1->SetIsLoopingMode(true);

	// 音声のループ始端を1秒に、ループ終端を6秒に設定する。
	bgm1->SetLoopStartingPoint(1.0f);
	bgm1->SetLoopEndPoint(6.0f);

	// 音声を再生する。
	int id_bgm1 = asd::Engine::GetSound()->Play(bgm1);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();
}


#include <Altseed.h>

// 空のウインドウを表示するサンプルです。
void Basic_Empty()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Empty").c_str(), 640, 480, asd::EngineOption());

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}

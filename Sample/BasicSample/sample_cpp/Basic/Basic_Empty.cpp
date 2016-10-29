
#include <Altseed.h>

/**
@brief	空のウインドウを表示するサンプル。
*/
void Basic_Empty()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(u"Empty", 640, 480, asd::EngineOption());

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

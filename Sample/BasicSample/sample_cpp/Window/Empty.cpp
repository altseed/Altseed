
#include <ace.h>

/**
	@brief	ウインドウを表示する。
*/
void Empty()
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("Empty").c_str(), 640, 480, ace::EngineOption());

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	// AC-Engineの終了処理をする。
	ace::Engine::Terminate();
}

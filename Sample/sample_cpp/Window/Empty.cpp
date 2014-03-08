
#include <Base.h>

/**
	@brief	ウインドウを表示する。
*/
void Empty()
{
	// aceを初期化する
	ace::Engine::Initialize(L"Empty", 640, 480, ace::EngineOption());

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		// aceを更新する。
		ace::Engine::Update();
	}

	// aceを終了する。
	ace::Engine::Terminate();
}

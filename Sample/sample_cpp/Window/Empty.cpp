
#include <Base.h>

/**
	@brief	ウインドウを表示する。
*/
void Empty()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("Empty").c_str(), 640, 480, asd::EngineOption());

	// aceが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		// aceを更新する。
		asd::Engine::Update();
	}

	// aceを終了する。
	asd::Engine::Terminate();
}


#include <Base.h>

/**
	@brief	ウインドウを表示する。
*/
void Empty()
{
	// aceを初期化する
	ace::GetEngine()->Initialize(L"Empty", 640, 480, false);

	// aceが進行可能かチェックする。
	while (ace::GetEngine()->DoEvents())
	{
		// aceを更新する。
		ace::GetEngine()->Update();
	}

	// aceを終了する。
	ace::GetEngine()->Terminate();
}
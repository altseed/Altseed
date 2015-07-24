
#include <Altseed.h>

#if _WIN32
#ifdef _DEBUG
#pragma comment(lib,"Debug/Altseed.lib")
#else
#pragma comment(lib,"Release/Altseed.lib")
#endif
#endif

int main()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Game").c_str(), 640, 480, asd::EngineOption());

	// 動的フォントを生成する。
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 35, asd::Color(255, 0, 0, 255), 1, asd::Color(255, 255, 255, 255));

	// TextObject2Dを生成する
	auto obj = std::make_shared<asd::TextObject2D>();

	// 描画に使うフォントを設定する。
	obj->SetFont(font);

	// 描画位置を指定する。
	obj->SetPosition(asd::Vector2DF(100, 100));

	// 描画する文字列を指定する。
	obj->SetText(asd::ToAString("Hello Altseed").c_str());

	// 文字描画オブジェクトのインスタンスをエンジンへ追加する。
	asd::Engine::AddObject2D(obj);
	

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
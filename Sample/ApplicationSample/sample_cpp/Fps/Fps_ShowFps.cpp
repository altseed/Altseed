#include <Altseed.h>

// 現在のFPSを画面に表示するサンプル。

void Fps_ShowFps()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Fps_ShowFps").c_str(), 640, 480, asd::EngineOption());

	// 動的フォントを生成する。
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(
		asd::ToAString("").c_str(),
		32,
		asd::Color(255, 255, 255, 255),
		1,
		asd::Color(255, 255, 255, 255));

	// FPSを表示するためのオブジェクトを生成する。
	auto obj = std::make_shared<asd::TextObject2D>();
	obj->SetFont(font);

	// オブジェクトをエンジンに追加する。
	asd::Engine::AddObject2D(obj);

	while (asd::Engine::DoEvents())
	{
		asd::Engine::Update();

		// 現在のFPSを取得する。
		float fps = asd::Engine::GetCurrentFPS();
		
		// 表示する文字列を生成する。
		char str[16];
		sprintf_s(str, 16, "%2.1ffps", fps);

		// 文字列をオブジェクトに設定する。
		obj->SetText(asd::ToAString(str).c_str());
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();
}
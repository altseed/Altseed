#include <Altseed.h>

void Basic_FullScreen()
{
	// フルスクリーンで起動するように初期化オプションを生成する。
	auto option = asd::EngineOption();
	option.IsFullScreen = true;

	// 作成した初期化オプションを用いてAltseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Empty").c_str(), 640, 480, option);

	// 操作説明文を文字列オブジェクトとして作成。CreateDynamicFontで作成したフォントオブジェクトを用いる
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 20, asd::Color(255, 255, 255, 255), 2, asd::Color(0, 0, 0, 255));
	auto obj = std::make_shared<asd::TextObject2D>();
	obj->SetFont(font);
	obj->SetText(asd::ToAString("Escキーで終了").c_str());

	// 操作説明文のオブジェクトをエンジンに登録する。
	asd::Engine::AddObject2D(obj);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();

		// Escキーが押されていたら
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Escape) == asd::KeyState::Push)
		{
			// ゲームループを抜ける
			break;
		}
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();
}
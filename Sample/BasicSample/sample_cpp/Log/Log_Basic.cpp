#include <Altseed.h>

void Log_Basic()
{
	//Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Log_Basic").c_str(), 640, 480, asd::EngineOption());

	// Engineの標準のロガーを使う（Log.htmlに出力される）
	auto log = asd::Engine::GetLogger();

	// ヘッダー文字列を出力する
	log->WriteHeading("サンプル出力");

	// 文字列を出力する
	log->Write("文字列");

	// 改行付きで文字列を出力する
	log->WriteLine("文字列＋改行");

	// 山括弧のエスケープもされる
	log->WriteLine("<文字列>");

	// 水平線(<hr/>)
	log->WriteHorizontalRule();

	// 強調された文字列を出力する
	log->WriteLineStrongly("強調文字列");


	// メッセージ用の文字列を用意する。TextObject2D_DynamicFontのサンプル参照
	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 35, asd::Color(255, 0, 0, 255), 1, asd::Color(255, 255, 255, 255));
	auto obj = std::make_shared<asd::TextObject2D>();
	obj->SetFont(font);
	obj->SetPosition(asd::Vector2DF(0, 0));
	obj->SetText(asd::ToAString("Log.htmlにログを出力しました。\nウィンドウを閉じる前に確認してください。").c_str());
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
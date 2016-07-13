#include <Altseed.h>

void Log_Table()
{
	//Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Log_OutputLevel").c_str(), 640, 480, asd::EngineOption());

	// Engineの標準のロガーを使う（Log.htmlに出力される）
	auto log = asd::Engine::GetLogger();

	// ヘッダー文字列を出力する
	log->WriteHeading("サンプル出力");

	// 表組みでの出力を開始する
	log->BeginTable();

	// 表のセルに文字列を出力する
	log->Write("セル１");

	// 次の列に進む
	log->ChangeColumn();

	// 表のセルに文字列を出力する
	log->Write("セル２(ChangeColmun)");

	// 次の行に進む。前の行にはもう戻れない
	log->ChangeRow();

	// 表のセルに文字列を出力する
	log->Write("セル３(ChangeRow)");

	// 表組みでの出力を終了する
	log->EndTable();

	// SetOutputLevelメソッドで指定したものより低いレベルに指定した出力は、実行されない
	// レベルが高い方から順に、Information, Warning, Critical, Error
	log->SetOutputLevel(asd::LogLevel::Critical);

	// Warningレベルで表組みで出力する。Criticalより低いレベルなので出力されない
	log->BeginTable(asd::LogLevel::Warning);
	log->Write("出力されないテーブル", asd::LogLevel::Warning);
	log->EndTable(asd::LogLevel::Warning);


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
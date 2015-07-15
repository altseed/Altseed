
#include <Altseed.h>

// StaticFile を用いて、ファイルからテキストを読み込むサンプルです。
void File_StaticFile()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("StaticFile").c_str(), 640, 480, asd::EngineOption());

	// フォントを生成する。
	auto font = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Data/Font/Font1.aff").c_str());

	// オブジェクトを生成する。
	auto obj = std::make_shared<asd::TextObject2D>();

	// 描画に使うフォントを設定する
	obj->SetFont(font);

	// 描画位置を指定する
	obj->SetPosition(asd::Vector2DF(100, 100));

	// ファイルオブジェクト作成
	auto staticFile = asd::Engine::GetFile()->CreateStaticFile(asd::ToAString("Data/Text/HelloWorld.txt").c_str());

	// 描画したいテキストをファイルから読み取る
	char text[32] = { 0 };
	memcpy(text, staticFile->GetBuffer().data(), staticFile->GetBuffer().size());

	// 描画する文字列の指定
	obj->SetText(asd::ToAString(text).c_str());

	// オブジェクトのインスタンスをエンジンへ追加する。
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

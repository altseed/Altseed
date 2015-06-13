
#include <Base.h>

/**
@brief	エフェクトを表示する。
*/
void TextObject2D()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("TextObject2D").c_str(), 640, 480, asd::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<asd::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	asd::Engine::ChangeScene(scene);

	{
		// フォントを生成する
		auto smallFont = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Data/Font/small.aff").c_str());

		//TextObject2Dを生成する
		auto smallObj = std::make_shared<asd::TextObject2D>();

		//描画に使うフォントを設定する
		smallObj->SetFont(smallFont);

		//描画位置を指定する
		smallObj->SetPosition(asd::Vector2DF(100, 100));

		//描画する文字列の指定
		smallObj->SetText(asd::ToAString("文字列サンプル1").c_str());

		//レイヤーへ追加する
		layer->AddObject(smallObj);
	}

	{
		auto largeFont = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Data/Font/large.aff").c_str());
		auto largeObj = std::make_shared<asd::TextObject2D>();
		largeObj->SetFont(largeFont);
		largeObj->SetPosition(asd::Vector2DF(100, 200));
		largeObj->SetText(asd::ToAString("文字列サンプル2").c_str());
		layer->AddObject(largeObj);
	}

	{
		auto edgeFont = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Data/Font/edge.aff").c_str());
		auto edgeObj = std::make_shared<asd::TextObject2D>();
		edgeObj->SetFont(edgeFont);
		edgeObj->SetPosition(asd::Vector2DF(100, 300));

		//30度回転させて描画する
		edgeObj->SetAngle(30);
		edgeObj->SetText(asd::ToAString("文字列サンプル3").c_str());
		layer->AddObject(edgeObj);
	}
	// aceが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		// aceを更新する。
		asd::Engine::Update();
	}

	// aceを終了する。
	asd::Engine::Terminate();
}
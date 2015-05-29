
#include <ace.h>

/**
@brief	エフェクトを表示する。
*/
void TextObject2D()
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("TextObject2D").c_str(), 640, 480, ace::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<ace::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<ace::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	ace::Engine::ChangeScene(scene);

	{
		// フォントを生成する
		auto smallFont = ace::Engine::GetGraphics()->CreateFont(ace::ToAString("Data/Font/small.aff").c_str());

		//TextObject2Dを生成する
		auto smallObj = std::make_shared<ace::TextObject2D>();

		//描画に使うフォントを設定する
		smallObj->SetFont(smallFont);

		//描画位置を指定する
		smallObj->SetPosition(ace::Vector2DF(100, 100));

		//描画する文字列の指定
		smallObj->SetText(ace::ToAString("文字列サンプル1").c_str());

		//レイヤーへ追加する
		layer->AddObject(smallObj);
	}

	{
		auto largeFont = ace::Engine::GetGraphics()->CreateFont(ace::ToAString("Data/Font/large.aff").c_str());
		auto largeObj = std::make_shared<ace::TextObject2D>();
		largeObj->SetFont(largeFont);
		largeObj->SetPosition(ace::Vector2DF(100, 200));
		largeObj->SetText(ace::ToAString("文字列サンプル2").c_str());
		layer->AddObject(largeObj);
	}

	{
		auto edgeFont = ace::Engine::GetGraphics()->CreateFont(ace::ToAString("Data/Font/edge.aff").c_str());
		auto edgeObj = std::make_shared<ace::TextObject2D>();
		edgeObj->SetFont(edgeFont);
		edgeObj->SetPosition(ace::Vector2DF(100, 300));

		//30度回転させて描画する
		edgeObj->SetAngle(30);
		edgeObj->SetText(ace::ToAString("文字列サンプル3").c_str());
		layer->AddObject(edgeObj);
	}
	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	// AC-Engineの終了処理をする。
	ace::Engine::Terminate();
}
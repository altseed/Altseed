#include <Base.h>

/**
@brief	円を描画するサンプル。
*/
void GeometryObject2D_CircleShape()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("GeometryObject2D_CircleShape").c_str(), 640, 480, asd::EngineOption());

	// 図形描画クラスのコンストラクタを呼び出す。
	auto geometryObj = std::make_shared<asd::GeometryObject2D>();

	// 図形描画クラスをレイヤーに追加する。
	asd::Engine::AddObject2D(geometryObj);

	// 弧を図形描画クラスにて描画する。
	{
		// 弧の外径、中心位置を指定。
		auto arc = std::make_shared<asd::ArcShape>();
		arc->SetOuterDiameter(100);
		arc->SetPosition(asd::Vector2DF(300, 100));

		// 弧を描画する図形として設定する。
		geometryObj->SetShape(arc);
		geometryObj->SetPosition(asd::Vector2DF(0, 0));
	}

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
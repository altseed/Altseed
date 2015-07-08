#include <Base.h>

/**
@brief	矩形を表示するサンプル。
*/
void GeometryObject2D_RectangleShape()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("GeometryObject2D_RectangleShape").c_str(), 640, 480, asd::EngineOption());

	// 図形描画クラスのコンストラクタを呼び出す。
	auto geometryObj = std::make_shared<asd::GeometryObject2D>();

	// 図形描画クラスをレイヤーに追加する。
	asd::Engine::AddObject2D(geometryObj);

	// 矩形を図形描画クラスにて描画する。
	{
		// 矩形の描画範囲、UV範囲を指定。
		auto rect = std::make_shared<asd::RectangleShape>();
		rect->SetDrawingArea(asd::RectF(10, 210, 300, 200));
		rect->SetUV(asd::RectF(0, 0, 0.5f, 0.5f));

		// 矩形を描画する図形として設定し、合成するテクスチャも設定。
		geometryObj->SetShape(rect);
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
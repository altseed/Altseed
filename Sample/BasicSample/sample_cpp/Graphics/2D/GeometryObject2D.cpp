#include <Base.h>

/**
@brief	GeometryObject2Dのサンプル。図形を表示しています。
*/
void GeometryObject2D()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("GeometryObject2D").c_str(), 640, 480, asd::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーを生成する。
	auto layer = std::make_shared<asd::Layer2D>();

	// テクスチャを画像ファイルから生成する。
	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());

	// 図形描画クラスのコンストラクタを呼び出す。
	auto geometryObj1 = std::make_shared<asd::GeometryObject2D>();
	auto geometryObj2 = std::make_shared<asd::GeometryObject2D>();
	auto geometryObj3 = std::make_shared<asd::GeometryObject2D>();

	// 図形描画クラスをレイヤーに追加する。
	layer->AddObject(geometryObj1);
	layer->AddObject(geometryObj2);
	layer->AddObject(geometryObj3);

	// レイヤーをシーンに追加する。
	scene->AddLayer(layer);

	// シーンを変更する。
	asd::Engine::ChangeScene(scene);
	
	// 弧を図形描画クラス1にて描画する。
	{
		// 弧の外径、内径、頂点数、中心位置、開始頂点番号、終了頂点番号を指定する。
		auto arc = std::make_shared<asd::ArcShape>();
		arc->SetOuterDiameter(100);
		arc->SetInnerDiameter(10);
		arc->SetNumberOfCorners(96);
		arc->SetPosition(asd::Vector2DF(300, 100));
		arc->SetStartingCorner(90);
		arc->SetEndingCorner(5);

		// 弧を描画する図形として設定し、合成するテクスチャも設定する。
		geometryObj1->SetShape(arc);
		geometryObj1->SetTexture(texture);
		geometryObj1->SetPosition(asd::Vector2DF(0, 0));
	}

	// 矩形を図形描画クラス2にて描画する。
	{
		// 矩形の描画範囲、UV範囲を指定する。
		auto rect = std::make_shared<asd::RectangleShape>();
		rect->SetDrawingArea(asd::RectF(10, 210, 300, 200));
		rect->SetUV(asd::RectF(0, 0, 0.5, 0.5));

		// 矩形を描画する図形として設定し、合成するテクスチャも設定する。
		geometryObj2->SetShape(rect);
		geometryObj2->SetTexture(texture);
		geometryObj2->SetPosition(asd::Vector2DF(0, 0));
	}

	// 多角形を図形描画クラス3にて描画する。
	{

		auto polygon = std::make_shared<asd::PolygonShape>();
		// 多角形を構成する頂点を追加していく。（星形になるようにする。）
		for (int i = 0; i < 10; ++i)
		{
			asd::Vector2DF vec = asd::Vector2DF(1, 0);
			vec.SetDegree(i * 36);
			vec.SetLength((i % 2 == 0) ? 100 : 55);
			polygon->AddVertex(vec + asd::Vector2DF(500, 250));

		}

		// 多角形を描画する図形として設定し、合成するテクスチャも設定。
		geometryObj3->SetShape(polygon);
		geometryObj3->SetTexture(texture);
		geometryObj3->SetPosition(asd::Vector2DF(0, 0));
	}

	// Altseedが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();

	return;
}
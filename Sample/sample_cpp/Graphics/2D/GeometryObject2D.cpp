#include <Base.h>

/**
@brief	図形を表示する。
*/
void GeometryObject2D()
{
	// aceを初期化する
	ace::Engine::Initialize(ace::ToAString("GeometryObject2D").c_str(), 640, 480, ace::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<ace::Scene>();

	auto layer = std::make_shared<ace::Layer2D>();
	auto texture = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());

	//図形描画クラスのコンストラクタを呼び出す。
	auto geometryObj1 = std::make_shared<ace::GeometryObject2D>();
	auto geometryObj2 = std::make_shared<ace::GeometryObject2D>();
	auto geometryObj3 = std::make_shared<ace::GeometryObject2D>();

	//図形描画クラスをレイヤーに追加する。
	layer->AddObject(geometryObj1);
	layer->AddObject(geometryObj2);
	layer->AddObject(geometryObj3);

	//レイヤーをシーンに追加する。
	scene->AddLayer(layer);

	//シーンを変更する。
	ace::Engine::ChangeScene(scene);

	//弧を図形描画クラス1にて描画する。
	{
		//弧の外径、内径、頂点数、中心位置、開始頂点番号、終了頂点番号を指定。
		auto arc = std::make_shared<ace::ArcShape>();
		arc->SetOuterDiameter(100);
		arc->SetInnerDiameter(10);
		arc->SetNumberOfCorners(96);
		arc->SetPosition(ace::Vector2DF(300, 100));
		arc->SetStartingCorner(90);
		arc->SetEndingCorner(5);

		//弧を描画する図形として設定し、合成するテクスチャも設定。
		geometryObj1->SetShape(arc);
		geometryObj1->SetTexture(texture);
		geometryObj1->SetPosition(ace::Vector2DF(0, 0));
	}

	//矩形を図形描画クラス2にて描画する。
	{
		//矩形の描画範囲、UV範囲を指定。
		auto rect = std::make_shared<ace::RectangleShape>();
		rect->SetDrawingArea(ace::RectF(10, 210, 300, 200));
		rect->SetUV(ace::RectF(0, 0, 0.5, 0.5));

		//矩形を描画する図形として設定し、合成するテクスチャも設定。
		geometryObj2->SetShape(rect);
		geometryObj2->SetTexture(texture);
		geometryObj2->SetPosition(ace::Vector2DF(0, 0));
	}

	//多角形を図形描画クラス3にて描画する。
	{

		auto polygon = std::make_shared<ace::PolygonShape>();
		//多角形を構成する頂点を追加していく。（星形になるようにする。）
		for (int i = 0; i < 10; ++i)
		{
			ace::Vector2DF vec = ace::Vector2DF(1, 0);
			vec.SetDegree(i * 36);
			vec.SetLength((i % 2 == 0) ? 100 : 55);
			polygon->AddVertex(vec + ace::Vector2DF(500, 250));

		}

		//多角形を描画する図形として設定し、合成するテクスチャも設定。
		geometryObj3->SetShape(polygon);
		geometryObj3->SetTexture(texture);
		geometryObj3->SetPosition(ace::Vector2DF(0, 0));
	}

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		// aceを更新する。
		ace::Engine::Update();
	}

	// aceを終了する。
	ace::Engine::Terminate();
}
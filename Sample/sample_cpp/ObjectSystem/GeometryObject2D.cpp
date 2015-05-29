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

	//図形描画クラスのコンストラクタを呼び出す
	auto geometryObj1 = std::make_shared<ace::GeometryObject2D>();
	auto geometryObj2 = std::make_shared<ace::GeometryObject2D>();
	auto geometryObj3 = std::make_shared<ace::GeometryObject2D>();
	auto geometryObj4 = std::make_shared<ace::GeometryObject2D>();
	auto geometryObj5 = std::make_shared<ace::GeometryObject2D>();
	auto geometryObj6 = std::make_shared<ace::GeometryObject2D>();

	//図形描画クラスをレイヤーに追加する。
	layer->AddObject(geometryObj1);
	layer->AddObject(geometryObj2);
	layer->AddObject(geometryObj3);
	layer->AddObject(geometryObj4);
	layer->AddObject(geometryObj5);
	layer->AddObject(geometryObj6);

	//レイヤーをシーンに追加する。
	scene->AddLayer(layer);

	//シーンを変更する。
	ace::Engine::ChangeScene(scene);

	//円を図形描画クラス1にて描画する。
	{
		//円の外径、内径、頂点数、中心位置を指定。
		auto circle = std::make_shared<ace::CircleShape>();
		circle->SetOuterDiameter(100);
		circle->SetInnerDiameter(10);
		circle->SetNumberOfCorners(96);
		circle->SetPosition(ace::Vector2DF(100, 50));

		//円を描画する図形として設定し、合成するテクスチャも設定。
		geometryObj1->SetShape(circle);
		geometryObj1->SetTexture(texture);
		geometryObj1->SetPosition(ace::Vector2DF(0, 0));
	}

	//弧を図形描画クラス2にて描画する。
		{
			//弧の外径、内径、頂点数、中心位置、開始頂点番号、終了頂点番号を指定。
			auto arc = std::make_shared<ace::ArcShape>();
			arc->SetOuterDiameter(100);
			arc->SetInnerDiameter(10);
			arc->SetNumberOfCorners(96);
			arc->SetPosition(ace::Vector2DF(300, 50));
			arc->SetStartingCorner(90);
			arc->SetEndingCorner(5);

			//弧を描画する図形として設定し、合成するテクスチャも設定。
			geometryObj2->SetShape(arc);
			geometryObj2->SetTexture(texture);
			geometryObj2->SetPosition(ace::Vector2DF(0, 0));
		}

		//線分を図形描画クラス3にて描画する。
		{
			//線分の始点、終点、太さを指定。
			auto line = std::make_shared<ace::LineShape>();
			line->SetStartingPosition(ace::Vector2DF(410, 50));
			line->SetEndingPosition(ace::Vector2DF(630, 50));
			line->SetThickness(5.0f);

			//線分を描画する図形として設定。
			geometryObj3->SetShape(line);
			geometryObj3->SetPosition(ace::Vector2DF(0, 0));
		}

		//矩形を図形描画クラス4にて描画する。
		{
			//矩形の描画範囲、UV範囲を指定。
			auto rect = std::make_shared<ace::RectangleShape>();
			rect->SetDrawingArea(ace::RectF(10, 110, 300, 200));
			rect->SetUV(ace::RectF(0, 0, 0.5, 0.5));

			//矩形を描画する図形として設定し、合成するテクスチャも設定。
			geometryObj4->SetShape(rect);
			geometryObj4->SetTexture(texture);
			geometryObj4->SetPosition(ace::Vector2DF(0, 0));
		}

		//三角形を図形描画クラス5にて描画する。
		{
			//三角形の各頂点の位置とUVを指定。
			auto triangle = std::make_shared<ace::TriangleShape>();
			triangle->SetPointByIndex(ace::Vector2DF(320, 350), 0);
			triangle->SetPointByIndex(ace::Vector2DF(100, 450), 1);
			triangle->SetPointByIndex(ace::Vector2DF(540, 450), 2);

			triangle->SetUVByIndex(ace::Vector2DF(0.5, 0.2), 0);
			triangle->SetUVByIndex(ace::Vector2DF(0.1, 0.5), 1);
			triangle->SetUVByIndex(ace::Vector2DF(0.9, 0.7), 2);

			//三角形を描画する図形として設定し、合成するテクスチャも設定。
			geometryObj5->SetShape(triangle);
			geometryObj5->SetTexture(texture);
			geometryObj5->SetPosition(ace::Vector2DF(0, 0));
		}

		//多角形を図形描画クラス6にて描画する。
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
			geometryObj6->SetShape(polygon);
			geometryObj6->SetTexture(texture);
			geometryObj6->SetPosition(ace::Vector2DF(0, 0));
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
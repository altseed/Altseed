#include <Altseed.h>

// Collision2Dのサンプル。マウスによって操作する円がいずれかのオブジェクトにヒットしたら円が赤く変化します。
void Collision2D()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("GeometryObject2D").c_str(), 640, 480, asd::EngineOption());

	// 図形描画クラスのコンストラクタを呼び出す。
	std::shared_ptr<asd::GeometryObject2D> geometryObj0 = std::make_shared<asd::GeometryObject2D>();
	std::shared_ptr<asd::GeometryObject2D> geometryObj1 = std::make_shared<asd::GeometryObject2D>();
	std::shared_ptr<asd::GeometryObject2D> geometryObj2 = std::make_shared<asd::GeometryObject2D>();
	std::shared_ptr<asd::GeometryObject2D> geometryObj3 = std::make_shared<asd::GeometryObject2D>();

	// マウスによって動かす円。
	std::shared_ptr<asd::CircleShape> selfCircle;

	// 停止している円、三角形、矩形。
	std::shared_ptr<asd::CircleShape> circle;
	std::shared_ptr<asd::TriangleShape> triangle;
	std::shared_ptr<asd::RectangleShape> rect;

	// シーンを生成する。
	auto scene = std::make_shared<asd::Scene>();

	// シーンにレイヤーを追加する。
	auto layer = std::make_shared<asd::Layer2D>();


	// 図形描画クラスをレイヤーに追加する。
	layer->AddObject(geometryObj0);
	layer->AddObject(geometryObj1);
	layer->AddObject(geometryObj2);
	layer->AddObject(geometryObj3);

	// レイヤーを追加する。
	scene->AddLayer(layer);

	// シーンを切り替える。
	asd::Engine::ChangeScene(scene);

	// マウスによって動かす円の形状と描画の設定を行う。
	{
		// 円の外径、内径、頂点数、中心位置を指定する。
		selfCircle = std::make_shared<asd::CircleShape>();
		selfCircle->SetOuterDiameter(100);
		selfCircle->SetInnerDiameter(0);
		selfCircle->SetNumberOfCorners(96);
		selfCircle->SetPosition(asd::Vector2DF(100, 50));

		// 円を描画する図形として最前面に描画されるように設定する。
		geometryObj0->SetShape(selfCircle);
		geometryObj0->SetPosition(asd::Vector2DF(0, 0));
		geometryObj0->SetDrawingPriority(1);
	}

	// 停止している円の形状と描画の設定を行う。
	{
		// 円の外径、内径、頂点数、中心位置を指定する。
		circle = std::make_shared<asd::CircleShape>();
		circle->SetOuterDiameter(100);
		circle->SetInnerDiameter(0);
		circle->SetNumberOfCorners(96);
		circle->SetPosition(asd::Vector2DF(100, 50));

		// 円を描画する図形として設定する。
		geometryObj1->SetShape(circle);
		geometryObj1->SetPosition(asd::Vector2DF(0, 0));
	}

	// 停止している三角形の形状と描画の設定を行う。
	{
		// 三角形の各頂点の位置を指定します。
		triangle = std::make_shared<asd::TriangleShape>();
		triangle->SetPointByIndex(asd::Vector2DF(400, 350), 0);
		triangle->SetPointByIndex(asd::Vector2DF(300, 450), 1);
		triangle->SetPointByIndex(asd::Vector2DF(540, 450), 2);

		// 三角形を描画する図形として設定する。
		geometryObj2->SetShape(triangle);
		geometryObj2->SetPosition(asd::Vector2DF(0, 0));
	}

	// 停止している矩形の形状と描画の設定を行う。
	{
		rect = std::make_shared<asd::RectangleShape>();
		rect->SetDrawingArea(asd::RectF(10, 110, 300, 200));
		rect->SetAngle(15);

		geometryObj3->SetShape(rect);
		geometryObj3->SetPosition(asd::Vector2DF(0, 0));
	}


	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		{
			// マウスによって制御される円の中心位置をマウスの位置とする。
			selfCircle->SetPosition(asd::Engine::GetMouse()->GetPosition());

			// 停止している円、三角形、矩形にマウスによって動く円が衝突した時に円を赤く変化させる。
			// そうでない時は白く変化させる。
			if (selfCircle->GetIsCollidedWith(circle)
				|| selfCircle->GetIsCollidedWith(triangle)
				|| selfCircle->GetIsCollidedWith(rect))
			{
				geometryObj0->SetColor(asd::Color(255, 0, 0, 255));
			}
			else
			{
				geometryObj0->SetColor(asd::Color(255, 255, 255, 255));
			}

			printf("%d",geometryObj1->GetDrawingPriority());
		}
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
#include <ace.h>

// Collision2Dのサンプル。マウスによって操作する円がいずれかのオブジェクトにヒットしたら円が赤く染まります。
void Collision2D()
{
	// AC-Engineを初期化する
	ace::Engine::Initialize(ace::ToAString("GeometryObject2D").c_str(), 640, 480, ace::EngineOption());

	//図形描画クラスのコンストラクタを呼び出す。
	std::shared_ptr<ace::GeometryObject2D> geometryObj0 = std::make_shared<ace::GeometryObject2D>();
	std::shared_ptr<ace::GeometryObject2D> geometryObj1 = std::make_shared<ace::GeometryObject2D>();
	std::shared_ptr<ace::GeometryObject2D> geometryObj2 = std::make_shared<ace::GeometryObject2D>();
	std::shared_ptr<ace::GeometryObject2D> geometryObj3 = std::make_shared<ace::GeometryObject2D>();

	//マウスによって動かす円
	std::shared_ptr<ace::CircleShape> selfCircle;

	//停止している円、三角形、矩形
	std::shared_ptr<ace::CircleShape> circle;
	std::shared_ptr<ace::TriangleShape> triangle;
	std::shared_ptr<ace::RectangleShape> rect;

	// シーンを生成する
	auto scene = std::make_shared<ace::Scene>();

	// シーンにレイヤーを追加する。
	auto layer = std::make_shared<ace::Layer2D>();


	//図形描画クラスをレイヤーに追加する。
	layer->AddObject(geometryObj0);
	layer->AddObject(geometryObj1);
	layer->AddObject(geometryObj2);
	layer->AddObject(geometryObj3);

	//レイヤーを追加する。
	scene->AddLayer(layer);

	// シーンを切り替える。
	ace::Engine::ChangeScene(scene);

	//マウスによって動かす円の形状と描画の設定を行う。
	{
		//円の外径、内径、頂点数、中心位置を指定。
		selfCircle = std::make_shared<ace::CircleShape>();
		selfCircle->SetOuterDiameter(100);
		selfCircle->SetInnerDiameter(0);
		selfCircle->SetNumberOfCorners(96);
		selfCircle->SetPosition(ace::Vector2DF(100, 50));

		//円を描画する図形として設定する。
		geometryObj0->SetShape(selfCircle);
		geometryObj0->SetPosition(ace::Vector2DF(0, 0));
	}

	//停止している円の形状と描画の設定を行う。
	{
		//円の外径、内径、頂点数、中心位置を指定。
		circle = std::make_shared<ace::CircleShape>();
		circle->SetOuterDiameter(100);
		circle->SetInnerDiameter(0);
		circle->SetNumberOfCorners(96);
		circle->SetPosition(ace::Vector2DF(100, 50));

		//円を描画する図形として設定する。
		geometryObj1->SetShape(circle);
		geometryObj1->SetPosition(ace::Vector2DF(0, 0));
	}

	//停止している三角形の形状と描画の設定を行う。
	{
		//三角形の各頂点の位置を指定。
		triangle = std::make_shared<ace::TriangleShape>();
		triangle->SetPointByIndex(ace::Vector2DF(400, 350), 0);
		triangle->SetPointByIndex(ace::Vector2DF(300, 450), 1);
		triangle->SetPointByIndex(ace::Vector2DF(540, 450), 2);

		//三角形を描画する図形として設定する。
		geometryObj2->SetShape(triangle);
		geometryObj2->SetPosition(ace::Vector2DF(0, 0));
	}

	//停止している矩形の形状と描画の設定を行う。
	{
		rect = std::make_shared<ace::RectangleShape>();
		rect->SetDrawingArea(ace::RectF(10, 110, 300, 200));
		rect->SetAngle(15);

		geometryObj3->SetShape(rect);
		geometryObj3->SetPosition(ace::Vector2DF(0, 0));
	}


	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		{
			//マウスによって制御される円の中心位置をマウスの位置とする。
			selfCircle->SetPosition(ace::Engine::GetMouse()->GetPosition());

			//停止している円、三角形、矩形にマウスによって動く円が衝突した時に円を赤く塗りつぶす。
			//そうでない時は白く塗りつぶす。
			if (selfCircle->GetIsCollidedWith(circle)
				|| selfCircle->GetIsCollidedWith(triangle)
				|| selfCircle->GetIsCollidedWith(rect))
			{
				geometryObj0->SetColor(ace::Color(255, 0, 0, 255));
			}
			else
			{
				geometryObj0->SetColor(ace::Color(255, 255, 255, 255));
			}
		}
		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	// AC-Engineの終了処理をする。
	ace::Engine::Terminate();

}
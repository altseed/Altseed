#include <Altseed.h>

// Collision2Dのサンプル。マウスによって操作する円が固定された円にヒットしたら円が赤く変化します。
void Collision2D_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Collision2D_Basic").c_str(), 640, 480, asd::EngineOption());

	// 図形描画オブジェクトのインスタンスを生成する。
	std::shared_ptr<asd::GeometryObject2D> geometryObj0 = std::make_shared<asd::GeometryObject2D>();
	std::shared_ptr<asd::GeometryObject2D> geometryObj1 = std::make_shared<asd::GeometryObject2D>();

	// マウスによって動かす円。
	std::shared_ptr<asd::CircleShape> selfCircle;

	// 停止している円、三角形、矩形。
	std::shared_ptr<asd::CircleShape> circle;

	// 図形描画クラスをエンジンに追加する。
	asd::Engine::AddObject2D(geometryObj0);
	asd::Engine::AddObject2D(geometryObj1);

	// マウスによって動かす円の形状と描画の設定を行う。
	{
		// 円の外径、内径、頂点数、中心位置を指定する。
		selfCircle = std::make_shared<asd::CircleShape>();
		selfCircle->SetOuterDiameter(100);

		// 円を描画する図形として最前面に描画されるように設定する。
		geometryObj0->SetShape(selfCircle);
	}

	// 停止している円の形状と描画の設定を行う。
	{
		// 円の外径、内径、頂点数、中心位置を指定する。
		circle = std::make_shared<asd::CircleShape>();
		circle->SetOuterDiameter(100);
		circle->SetPosition(asd::Vector2DF(100, 100));

		// 円を描画する図形として設定する。
		geometryObj1->SetShape(circle);
	}


	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// マウスによって制御される円の中心位置をマウスの位置とする。
		selfCircle->SetPosition(asd::Engine::GetMouse()->GetPosition());

		// 固定されている円に、マウスによって動く円が衝突した時に円を赤く変化させる。
		// そうでない時は白く変化させる。
		if (selfCircle->GetIsCollidedWith(circle))
		{
			geometryObj0->SetColor(asd::Color(255, 0, 0, 255));
		}
		else
		{
			geometryObj0->SetColor(asd::Color(255, 255, 255, 255));
		}

		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
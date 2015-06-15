#include <Base.h>

/**
@brief	矩形を表示するサンプル。
*/
void GeometryObject2D_RectangleShape()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("GeometryObject2D_Rectangle").c_str(), 640, 480, asd::EngineOption());

	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());

	//図形描画クラスのコンストラクタを呼び出す。
	auto geometryObj = std::make_shared<asd::GeometryObject2D>();

	//図形描画クラスをレイヤーに追加する。
	asd::Engine::AddObject2D(geometryObj);

	//矩形を図形描画クラス2にて描画する。
	{
		//矩形の描画範囲、UV範囲を指定。
		auto rect = std::make_shared<asd::RectangleShape>();
		rect->SetDrawingArea(asd::RectF(10, 210, 300, 200));
		rect->SetUV(asd::RectF(0, 0, 0.5f, 0.5f));

		//矩形を描画する図形として設定し、合成するテクスチャも設定。
		geometryObj->SetShape(rect);
		geometryObj->SetTexture(texture);
		geometryObj->SetPosition(asd::Vector2DF(0, 0));
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
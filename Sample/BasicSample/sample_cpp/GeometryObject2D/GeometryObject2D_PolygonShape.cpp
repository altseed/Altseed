#include <Base.h>

/**
@brief	多角形を表示するサンプル。
*/
void GeometryObject2D_PolygonShape()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("GeometryObject2D_PolygonShape").c_str(), 640, 480, asd::EngineOption());

	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());

	//図形描画クラスのコンストラクタを呼び出す。
	auto geometryObj = std::make_shared<asd::GeometryObject2D>();

	//図形描画クラスをレイヤーに追加する。
	asd::Engine::AddObject2D(geometryObj);

	//多角形を図形描画クラスにて描画する。
	{

		auto polygon = std::make_shared<asd::PolygonShape>();
		//多角形を構成する頂点を追加していく。（星形になるようにする。）
		for (int i = 0; i < 10; ++i)
		{
			asd::Vector2DF vec = asd::Vector2DF(1, 0);
			vec.SetDegree(i * 36);
			vec.SetLength((i % 2 == 0) ? 100 : 55);
			polygon->AddVertex(vec + asd::Vector2DF(500, 250));

		}

		//多角形を描画する図形として設定し、合成するテクスチャも設定。
		geometryObj->SetShape(polygon);
		geometryObj->SetTexture(texture);
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
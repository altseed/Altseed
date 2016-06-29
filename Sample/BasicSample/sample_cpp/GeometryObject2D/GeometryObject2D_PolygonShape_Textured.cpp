#include <Base.h>

/**
@brief	多角形をテクスチャを合成した上で表示するサンプル。
*/
void GeometryObject2D_PolygonShape_Textured()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("GeometryObject2D_PolygonShape_Textured").c_str(), 640, 480, asd::EngineOption());

	// テクスチャとして図形に合成する画像を読み込む。
	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());

	// 図形描画オブジェクトのインスタンスを生成する。
	auto geometryObj = std::make_shared<asd::GeometryObject2D>();

	// 図形描画オブジェクトのインスタンスをエンジンに追加する。
	asd::Engine::AddObject2D(geometryObj);

	// 多角形の図形クラスのインスタンスを生成する。
	auto polygon = std::make_shared<asd::PolygonShape>();

	// 多角形を構成する頂点を追加していく。（星形になるようにする。）
	for (int i = 0; i < 10; ++i)
	{
		asd::Vector2DF vec = asd::Vector2DF(1, 0);
		vec.SetDegree(i * 36);

		if (i % 2 == 0)
		{
			vec.SetLength(200);
		}
		else
		{
			vec.SetLength(75);
		}
		polygon->AddVertex(vec + asd::Vector2DF(320, 240));

	}

	// 多角形を描画する図形として設定し、合成するテクスチャも設定する。
	geometryObj->SetShape(polygon);
	geometryObj->SetTexture(texture);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
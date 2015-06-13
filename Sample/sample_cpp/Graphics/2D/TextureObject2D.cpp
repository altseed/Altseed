
#include <Base.h>

/**
@brief	エフェクトを表示する。
*/
void TextureObject2D()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("TextureObject2D").c_str(), 640, 480, asd::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<asd::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	asd::Engine::ChangeScene(scene);

	{
		// テクスチャを生成する
		auto tex0 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample2.png").c_str());

		//TextureObject2Dを生成する
		auto obj0 = std::make_shared<asd::TextureObject2D>();

		//描画に使うテクスチャを設定する
		obj0->SetTexture(tex0);

		//描画位置を指定する
		obj0->SetPosition(asd::Vector2DF(100, 400));

		//レイヤーへ追加する
		layer->AddObject(obj0);
	}

	{
		auto tex1 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample2.png").c_str());

		auto obj1 = std::make_shared<asd::TextureObject2D>();

		obj1->SetTexture(tex1);

		obj1->SetPosition(asd::Vector2DF(500, 400));

		//画像を上下に反転する
		obj1->SetTurnUL(true);

		layer->AddObject(obj1);
	}

	auto obj2 = std::make_shared<asd::TextureObject2D>();
	{

		auto tex2 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Character.png").c_str());

		obj2->SetTexture(tex2);

		obj2->SetPosition(asd::Vector2DF(500, 50));

		layer->AddObject(obj2);
	}


	auto obj3 = std::make_shared<asd::TextureObject2D>();
	{

		auto tex3 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());

		//画像(サイズ 512 x 512)の中心(256 x 256 の地点)を描画の起点とする
		obj3->SetCenterPosition(asd::Vector2DF(256, 256));
		obj3->SetTexture(tex3);

		obj3->SetPosition(asd::Vector2DF(320, 240));

		//画像を中心を軸に45度回転する
		obj3->SetAngle(45);

		//画像をX,Y方向に0.4倍に縮小する
		obj3->SetScale(asd::Vector2DF(0.4, 0.4));

		layer->AddObject(obj3);
	}

	{
		auto obj4 = std::make_shared<asd::TextureObject2D>();

		auto tex4 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Character.png").c_str());

		obj4->SetTexture(tex4);

		obj4->SetPosition(asd::Vector2DF(50, 50));

		layer->AddObject(obj4);
	}


	int count = 0;
	int index = 0;
	// aceが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{

		int xi = index % 3;
		int yi = (index / 3) % 4;

		//テクスチャの切り出し範囲を指定する。
		obj2->SetSrc(asd::RectF(32 * xi, 32 * yi, 32, 32));

		if (count % 5 == 0)
		{
			index = (++index) % 12;
		}
		++count;

		float angle = obj3->GetAngle();
		angle += 0.5;
		obj3->SetAngle(angle);

		// aceを更新する。
		asd::Engine::Update();
	}

	// aceを終了する。
	asd::Engine::Terminate();
}

#include <ace.h>

/**
@brief	エフェクトを表示する。
*/
void TextureObject2D()
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("TextureObject2D").c_str(), 640, 480, ace::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<ace::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<ace::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	ace::Engine::ChangeScene(scene);

	{
		// テクスチャを生成する
		auto tex0 = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample2.png").c_str());

		//TextureObject2Dを生成する
		auto obj0 = std::make_shared<ace::TextureObject2D>();

		//描画に使うテクスチャを設定する
		obj0->SetTexture(tex0);

		//描画位置を指定する
		obj0->SetPosition(ace::Vector2DF(100, 400));

		//レイヤーへ追加する
		layer->AddObject(obj0);
	}

	{
		auto tex1 = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample2.png").c_str());

		auto obj1 = std::make_shared<ace::TextureObject2D>();

		obj1->SetTexture(tex1);

		obj1->SetPosition(ace::Vector2DF(500, 400));

		//画像を上下に反転する
		obj1->SetTurnUL(true);

		layer->AddObject(obj1);
	}

	auto obj2 = std::make_shared<ace::TextureObject2D>();
	{

		auto tex2 = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Character.png").c_str());

		obj2->SetTexture(tex2);

		obj2->SetPosition(ace::Vector2DF(500, 50));

		layer->AddObject(obj2);
	}


	auto obj3 = std::make_shared<ace::TextureObject2D>();
	{

		auto tex3 = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());

		//画像(サイズ 512 x 512)の中心(256 x 256 の地点)を描画の起点とする
		obj3->SetCenterPosition(ace::Vector2DF(256, 256));
		obj3->SetTexture(tex3);

		obj3->SetPosition(ace::Vector2DF(320, 240));

		//画像を中心を軸に45度回転する
		obj3->SetAngle(45);

		//画像をX,Y方向に0.4倍に縮小する
		obj3->SetScale(ace::Vector2DF(0.4, 0.4));

		layer->AddObject(obj3);
	}

	{
		auto obj4 = std::make_shared<ace::TextureObject2D>();

		auto tex4 = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Character.png").c_str());

		obj4->SetTexture(tex4);

		obj4->SetPosition(ace::Vector2DF(50, 50));

		layer->AddObject(obj4);
	}


	int count = 0;
	int index = 0;
	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{

		int xi = index % 3;
		int yi = (index / 3) % 4;

		//テクスチャの切り出し範囲を指定する。
		obj2->SetSrc(ace::RectF(32 * xi, 32 * yi, 32, 32));

		if (count % 5 == 0)
		{
			index = (++index) % 12;
		}
		++count;

		float angle = obj3->GetAngle();
		angle += 0.5;
		obj3->SetAngle(angle);

		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	// AC-Engineの終了処理をする。
	ace::Engine::Terminate();
}
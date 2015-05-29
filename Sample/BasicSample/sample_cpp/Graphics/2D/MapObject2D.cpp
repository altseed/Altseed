
#include <ace.h>

/**
@brief	エフェクトを表示する。
*/
void MapObject2D()
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("MapObject2D").c_str(), 640, 480, ace::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<ace::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<ace::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	ace::Engine::ChangeScene(scene);

	//カメラを設定する。
	auto camera = std::make_shared<ace::CameraObject2D>();

	camera->SetSrc(ace::RectI(0, 0, 640, 480));
	camera->SetDst(ace::RectI(0, 0, 640, 480));

	{
		//マップオブジェクトを生成する。
		auto mapObject = std::make_shared<ace::MapObject2D>();

		auto texture = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample2.png").c_str());

		//マップオブジェクトに50*50=2500個のチップを登録する。
		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				//チップを生成する。
				auto chip = std::make_shared<ace::Chip2D>();

				//チップにテクスチャを設定する。
				chip->SetTexture(texture);

				//チップの描画先を指定する。
				chip->SetPosition(ace::Vector2DF(i * 40 - 1000, j * 40 - 1000));

				//マップオブジェクトにチップを追加する。
				mapObject->AddChip(chip);
			}
		}

		//レイヤーにマップオブジェクトを追加する。
		layer->AddObject(mapObject);

		//レイヤーにカメラオブジェクトを追加する。
		layer->AddObject(camera);
	}

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		//カメラを移動させる
		auto pos = camera->GetSrc();
		pos.X += 1;
		pos.Y += 1;
		pos.X %= 1000;
		pos.Y %= 1000;
		camera->SetSrc(pos);

		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	// AC-Engineの終了処理をする。
	ace::Engine::Terminate();
}
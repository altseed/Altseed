
#include <Base.h>

/**
@brief	エフェクトを表示する。
*/
void MapObject2D()
{
	// aceを初期化する
	ace::Engine::Initialize(ace::ToAString("MapObject2D").c_str(), 640, 480, ace::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<ace::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<ace::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	ace::Engine::ChangeScene(scene);

	auto camera = std::make_shared<ace::CameraObject2D>();

	camera->SetSrc(ace::RectI(0, 0, 640, 480));
	camera->SetDst(ace::RectI(0, 0, 640, 480));

	{
		auto mapObject = std::make_shared<ace::MapObject2D>();

		auto texture = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample2.png").c_str());

		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				auto chip = ace::Engine::GetGraphics()->CreateChip2D();
				chip->SetTexture(texture);
				chip->SetSrc(ace::RectF(i * 40 - 1000, j * 40 - 1000, 40, 40));
				mapObject->AddChip(chip);
			}
		}

		layer->AddObject(mapObject);
		layer->AddObject(camera);
	}
	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		auto pos = camera->GetSrc();
		pos.X += 1;
		pos.Y += 1;
		camera->SetSrc(pos);
		// aceを更新する。
		ace::Engine::Update();
	}

	// aceを終了する。
	ace::Engine::Terminate();
}
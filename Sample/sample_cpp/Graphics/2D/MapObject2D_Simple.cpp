
#include <Base.h>

/**
@brief	マップを表示する。
*/
void MapObject2D_Simple()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("MapObject2D_Simple").c_str(), 640, 480, asd::EngineOption());

	{
		//マップオブジェクトを生成する。
		auto mapObject = std::make_shared<asd::MapObject2D>();

		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample2.png").c_str());

		//マップオブジェクトに50*50=2500個のチップを登録する。
		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				//チップを生成する。
				auto chip = std::make_shared<asd::Chip2D>();

				//チップにテクスチャを設定する。
				chip->SetTexture(texture);

				//チップの描画先を指定する。
				chip->SetPosition(asd::Vector2DF(i * 40 - 1000, j * 40 - 1000));

				//マップオブジェクトにチップを追加する。
				mapObject->AddChip(chip);
			}
		}

		//レイヤーにマップオブジェクトを追加する。
		asd::Engine::AddObject2D(mapObject);
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
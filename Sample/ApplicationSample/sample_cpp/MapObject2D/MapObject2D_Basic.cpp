
#include <Base.h>

/**
@brief	マップを表示するサンプル。
*/
void MapObject2D_Basic()
{
	// Altseedを初期化する
	asd::Engine::Initialize(u"MapObject2D_Basic", 640, 480, asd::EngineOption());

	{
		//マップオブジェクトを生成する。
		auto mapObject = std::make_shared<asd::MapObject2D>();

		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(u"Data/Texture/Chip1.png");

		//マップオブジェクトに16*12=184個のチップを登録する。
		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < 12; ++j)
			{
				//チップを生成する。
				auto chip = std::make_shared<asd::Chip2D>();

				//チップにテクスチャを設定する。
				chip->SetTexture(texture);

				//チップの描画先を指定する。
				chip->SetPosition(asd::Vector2DF(i * 40, j * 40));

				//マップオブジェクトにチップを追加する。
				mapObject->AddChip(chip);
			}
		}

		// マップオブジェクトのインスタンスをエンジンへ追加する。
		asd::Engine::AddObject2D(mapObject);
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
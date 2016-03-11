
#include <Altseed.h>

// アクションゲームでカメラがキャラクターを追うサンプル。
void Action2D_Camera()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Action2D_Camera").c_str(), 640, 480, asd::EngineOption());

	// カメラを設定する。
	auto camera = std::make_shared<asd::CameraObject2D>();

	camera->SetSrc(asd::RectI(0, 0, 640, 480));
	camera->SetDst(asd::RectI(0, 0, 640, 480));

	// エンジンにカメラオブジェクトを追加する。
	asd::Engine::AddObject2D(camera);

	{
		// マップオブジェクトを生成する。
		auto mapObject = std::make_shared<asd::MapObject2D>();
	
		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Chip1.png").c_str());
	
		// マップオブジェクトに50*50=2500個のチップを登録する。
		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				// チップを生成する。
				auto chip = std::make_shared<asd::Chip2D>();
	
				// チップにテクスチャを設定する。
				chip->SetTexture(texture);
	
				// チップの描画先を指定する。
				chip->SetPosition(asd::Vector2DF(i * 40 - 1000, j * 40 - 1000));
	
				// マップオブジェクトにチップを追加する。
				mapObject->AddChip(chip);
			}
		}
	
		// マップオブジェクトのインスタンスをエンジンへ追加する。
		asd::Engine::AddObject2D(mapObject);
	}

	// キャラクターを設定する。
	auto charactor = std::make_shared<asd::TextureObject2D>();

	// キャラクターの画像を読み込む。
	charactor->SetTexture(asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Character1.png").c_str()));

	// キャラクターをエンジンに追加する。
	asd::Engine::AddObject2D(charactor);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// キャラクターを移動させる。
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Up) == asd::KeyState::Hold) charactor->SetPosition(charactor->GetPosition() + asd::Vector2DF(0, -2));
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Down) == asd::KeyState::Hold) charactor->SetPosition(charactor->GetPosition() + asd::Vector2DF(0, +2));
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Left) == asd::KeyState::Hold) charactor->SetPosition(charactor->GetPosition() + asd::Vector2DF(-2, 0));
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Right) == asd::KeyState::Hold) charactor->SetPosition(charactor->GetPosition() + asd::Vector2DF(+2, 0));

		//カメラをキャラクターの位置に合わせる。
		auto pos = camera->GetSrc();
		pos.X = charactor->GetPosition().X - 640 / 2;
		pos.Y = charactor->GetPosition().Y - 480 / 2;
		camera->SetSrc(pos);

		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
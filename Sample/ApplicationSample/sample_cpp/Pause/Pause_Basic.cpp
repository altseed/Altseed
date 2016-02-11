
#include <Altseed.h>

// ポーズのサンプル。

/**
@brief	ポーズを実行するレイヤー
*/
class PauseLayer
	: public asd::Layer2D
{
	void OnAdded() override
	{
		// ポーズ以外のレイヤーの更新を停止する。
		for(auto& layer : GetScene()->GetLayers())
		{
			if (layer.get() != this) layer->SetIsUpdated(false);
		}

		// ポーズを表示するオブジェクトを生成する。
		auto obj = std::make_shared<asd::TextureObject2D>();
		obj->SetTexture(asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Pause1.png").c_str()));
		AddObject(obj);
	}

	void OnUpdated() override
	{
		// スペースが押されたら、他のレイヤーの更新を再開して、ポーズレイヤーを破棄する。
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Space) == asd::KeyState::Push)
		{
			for (auto& layer : GetScene()->GetLayers())
			{
				layer->SetIsUpdated(true);
			}
			Dispose();
		}
	}
};

/**
	@brief	ゲームの挙動を描画するレイヤー
*/
class MainLayer 
	: public asd::Layer2D
{
	std::shared_ptr<asd::TextureObject2D> obj = nullptr;

public:
	MainLayer()
	{
		// 画像を表示するオブジェクトを生成する。
		obj = std::make_shared<asd::TextureObject2D>();
		obj->SetTexture(asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str()));
		obj->SetPosition(asd::Vector2DF(320, 240));
		obj->SetCenterPosition(asd::Vector2DF(obj->GetTexture()->GetSize().X / 2, obj->GetTexture()->GetSize().Y / 2));
		AddObject(obj);
	}

	void OnUpdated() override
	{
		// スペースが押されたら、ポーズレイヤーを追加する。
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Space) == asd::KeyState::Push)
		{
			auto pauseLayer = std::make_shared<PauseLayer>();
			GetScene()->AddLayer(pauseLayer);
		}

		// 画像を回転させる。
		obj->SetAngle(obj->GetAngle() + 1.0f);
	}
};

void Pause_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Pause_Basic").c_str(), 640, 480, asd::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<MainLayer>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	asd::Engine::ChangeScene(scene);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
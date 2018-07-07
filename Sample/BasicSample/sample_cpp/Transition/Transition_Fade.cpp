#include <Altseed.h>

// フェードアウト・フェードインでシーン遷移をするサンプル。

// 遷移後のシーン
class Transition_Fade_Scene2
	: public asd::Scene
{
public:
	void OnRegistered() override
	{
		// 画像を読み込み、オブジェクトに設定する。
		auto obj = std::make_shared<asd::TextureObject2D>();
		auto tex = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Scene2.png").c_str());
		obj->SetTexture(tex);

		// シーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
		auto layer = std::make_shared<asd::Layer2D>();
		AddLayer(layer);
		layer->AddObject(obj);
	}
};

// 遷移前のシーン
class Transition_Fade_Scene1
	: public asd::Scene
{
public:
	void OnRegistered() override
	{
		// 画像を読み込み、オブジェクトに設定する。
		auto obj = std::make_shared<asd::TextureObject2D>();
		auto tex = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Scene1.png").c_str());
		obj->SetTexture(tex);

		// シーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
		auto layer = std::make_shared<asd::Layer2D>();
		AddLayer(layer);
		layer->AddObject(obj);
	}

	void OnUpdated() override
	{
		// マウスの左ボタンが押されるのを待つ。
		if (asd::Engine::GetMouse()->GetLeftButton()->GetButtonState() == asd::ButtonState::Push)
		{
			// フェードアウト・インによるシーン遷移を開始する。
			// 1秒かけてフェードアウトし、1.5秒かけてフェードイン。
			asd::Engine::ChangeSceneWithTransition(std::make_shared<Transition_Fade_Scene2>(), std::make_shared<asd::TransitionFade>(1.0f, 1.5f));
		}
	}
};

void Transition_Fade()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Transition_Fade").c_str(), 640, 480, asd::EngineOption());

	// シーンのインスタンスを生成する。
	auto scene = std::make_shared<Transition_Fade_Scene1>();

	// シーンをシーン1に設定する。
	asd::Engine::ChangeScene(scene);
	
	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}
	
	// Altseedを終了する。
	asd::Engine::Terminate();
}
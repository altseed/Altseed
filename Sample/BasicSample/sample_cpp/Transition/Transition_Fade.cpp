#include <Altseed.h>

// フェードアウト・フェードインでシーン遷移をするサンプルです。
void Transition_Fade()
{
	asd::Engine::Initialize(asd::ToAString("Transition_Fade").c_str(), 640, 480, asd::EngineOption());

	// シーン(1)、レイヤー、オブジェクトを生成する。
	auto scene1 = std::make_shared<asd::Scene>();
	auto layer1 = std::make_shared<asd::Layer2D>();
	auto object1 = std::make_shared<asd::TextureObject2D>();

	// テクスチャを生成し、オブジェクトに設定する。
	auto texture1 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Scene1.png").c_str());
	object1->SetTexture(texture1);

	// シーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
	scene1->AddLayer(layer1);
	layer1->AddObject(object1);


	// 上と同じものをもう１セット作る。
	auto scene2 = std::make_shared<asd::Scene>();
	auto layer2 = std::make_shared<asd::Layer2D>();
	auto object2 = std::make_shared<asd::TextureObject2D>();

	auto texture2 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Scene2.png").c_str());
	object2->SetTexture(texture2);

	scene2->AddLayer(layer2);
	layer2->AddObject(object2);


	// シーンをシーン1に設定する。
	asd::Engine::ChangeScene(scene1);

	while (asd::Engine::DoEvents())
	{
		asd::Engine::Update();

		// マウスの左ボタンが押されるのを待つ。
		if (asd::Engine::GetMouse()->GetLeftButton()->GetButtonState() == asd::MouseButtonState::Push)
		{
			// フェードアウト・インによるシーン遷移を開始する。
			// 1秒かけてフェードアウトし、1.5秒かけてフェードイン。
			asd::Engine::ChangeSceneWithTransition(scene2, std::make_shared<asd::TransitionFade>(1.0f, 1.5f));
		}
	}
	
	asd::Engine::Terminate();
}
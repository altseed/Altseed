
#include <ace.h>

// 様々なポストエフェクトを適用するサンプルです。
void PostEffect()
{
	// 説明
	printf("Z : グレースケール追加\n");
	printf("X : セピア調追加\n");
	printf("C : ガウスぼかし追加\n");
	printf("V : ライトブルーム追加\n");
	printf("A : 全てのポストエフェクト消去\n");

	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("PostEffect").c_str(), 640, 480, ace::EngineOption());

	// シーン等を初期化する
	auto scene = std::make_shared<ace::Scene>();
	auto layer = std::make_shared<ace::Layer2D>();
	auto effectObj = std::make_shared<ace::EffectObject2D>();
	auto effect = ace::Engine::GetGraphics()->CreateEffect(ace::ToAString("Data/Effect/magic.efk").c_str());

	scene->SetHDRMode(true);
	scene->AddLayer(layer);
	layer->AddObject(effectObj);
	ace::Engine::ChangeScene(scene);

	effectObj->SetScale(ace::Vector2DF(50, 50));
	effectObj->SetPosition(ace::Vector2DF(320, 240));
	effectObj->SetEffect(effect);
	effectObj->Play();

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		// AC-Engineを更新する。
		ace::Engine::Update();

		// Aを押したらポストエフェクトを全て破棄する。
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::A) == ace::KeyState::Push)
		{
			layer->ClearPostEffects();
		}

		// Zを押したらグレースケールを追加する。
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::Z) == ace::KeyState::Push)
		{
			auto posteffect = std::make_shared<ace::PostEffectGrayScale>();
			layer->AddPostEffect(posteffect);
		}

		// Xを押したらセピアを追加する。
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::X) == ace::KeyState::Push)
		{
			auto posteffect = std::make_shared<ace::PostEffectSepia>();
			layer->AddPostEffect(posteffect);
		}

		// Cを押したらガウスぼかしを追加する。
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::C) == ace::KeyState::Push)
		{
			auto posteffect = std::make_shared<ace::PostEffectGaussianBlur>();
			posteffect->SetIntensity(5.0f);
			layer->AddPostEffect(posteffect);
		}

		// Vを押したらライトブルームを追加する。
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::V) == ace::KeyState::Push)
		{
			auto posteffect = std::make_shared<ace::PostEffectLightBloom>();
			posteffect->SetIntensity(10.0f);
			posteffect->SetExposure(1.0f);
			posteffect->SetThreshold(0.3f);
			layer->AddPostEffect(posteffect);
		}
	}

	// AC-Engineの終了処理をする。
	ace::Engine::Terminate();

	return;
}

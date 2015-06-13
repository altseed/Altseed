
#include <Base.h>

/**
@brief	ポストエフェクトを適用する。
*/
void PostEffect()
{
	// 説明
	printf("Z : グレースケール追加\n");
	printf("X : セピア調追加\n");
	printf("C : ガウスぼかし追加\n");
	printf("V : ライトブルーム追加\n");
	printf("A : 全てのポストエフェクト消去\n");

	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("PostEffect").c_str(), 640, 480, asd::EngineOption());

	// シーン等を初期化する
	auto scene = std::make_shared<asd::Scene>();
	auto layer = std::make_shared<asd::Layer2D>();
	auto effectObj = std::make_shared<asd::EffectObject2D>();
	auto effect = asd::Engine::GetGraphics()->CreateEffect(asd::ToAString("Data/Effect/magic.efk").c_str());

	scene->SetHDRMode(true);
	scene->AddLayer(layer);
	layer->AddObject(effectObj);
	asd::Engine::ChangeScene(scene);

	effectObj->SetScale(asd::Vector2DF(50, 50));
	effectObj->SetPosition(asd::Vector2DF(320, 240));
	effectObj->SetEffect(effect);
	effectObj->Play();

	// aceが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		// aceを更新する。
		asd::Engine::Update();

		// Aを押したらポストエフェクトを全て破棄する。
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::A) == asd::KeyState::Push)
		{
			layer->ClearPostEffects();
		}

		// Zを押したらグレースケールを追加する。
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::Z) == asd::KeyState::Push)
		{
			auto posteffect = std::make_shared<asd::PostEffectGrayScale>();
			layer->AddPostEffect(posteffect);
		}

		// Xを押したらセピアを追加する。
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::X) == asd::KeyState::Push)
		{
			auto posteffect = std::make_shared<asd::PostEffectSepia>();
			layer->AddPostEffect(posteffect);
		}

		// Cを押したらガウスぼかしを追加する。
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::C) == asd::KeyState::Push)
		{
			auto posteffect = std::make_shared<asd::PostEffectGaussianBlur>();
			posteffect->SetIntensity(5.0f);
			layer->AddPostEffect(posteffect);
		}

		// Vを押したらライトブルームを追加する。
		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::V) == asd::KeyState::Push)
		{
			auto posteffect = std::make_shared<asd::PostEffectLightBloom>();
			posteffect->SetIntensity(10.0f);
			posteffect->SetExposure(1.0f);
			posteffect->SetThreshold(0.3f);
			layer->AddPostEffect(posteffect);
		}
	}

	// aceを終了する。
	asd::Engine::Terminate();
}

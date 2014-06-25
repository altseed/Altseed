
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
	ace::Engine::Initialize(L"PostEffect", 640, 480, ace::EngineOption());

	// シーン等を初期化する(本サンプルのメインの内容には関係なし)
	auto scene = std::make_shared<ace::Scene>();
	auto layer = std::make_shared<ace::Layer3D>();
	auto effectObj = std::make_shared<ace::EffectObject3D>();
	auto cameraObj = std::make_shared<ace::CameraObject3D>();
	auto effect = ace::Engine::GetGraphics()->CreateEffect(ace::ToAString("Data/Effect/magic.efk").c_str());

	scene->SetHDRMode(true);
	scene->AddLayer(layer);
	layer->AddObject(effectObj);
	layer->AddObject(cameraObj);
	ace::Engine::ChangeScene(scene);

	cameraObj->SetPosition(ace::Vector3DF(2, 2, 20));
	cameraObj->SetFieldOfView(20);
	cameraObj->SetZNear(1);
	cameraObj->SetZFar(40);
	cameraObj->SetWindowSize(ace::Vector2DI(640, 480));

	effectObj->SetEffect(effect);
	effectObj->Play();

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		// aceを更新する。
		ace::Engine::Update();

		// Aを押したらポストエフェクトを全て破棄する。
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::A) == ace::KeyboardButtonState::Push)
		{
			layer->ClearPostEffects();
		}

		// Zを押したらグレースケールを追加する。
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::Z) == ace::KeyboardButtonState::Push)
		{
			auto posteffect = std::make_shared<ace::PostEffectGrayScale>();
			layer->AddPostEffect(posteffect);
		}

		// Xを押したらセピアを追加する。
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::X) == ace::KeyboardButtonState::Push)
		{
			auto posteffect = std::make_shared<ace::PostEffectSepia>();
			layer->AddPostEffect(posteffect);
		}

		// Cを押したらガウスぼかしを追加する。
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::C) == ace::KeyboardButtonState::Push)
		{
			auto posteffect = std::make_shared<ace::PostEffectGaussianBlur>();
			posteffect->SetIntensity(5.0f);
			layer->AddPostEffect(posteffect);
		}

		// Vを押したらライトブルームを追加する。
		if (ace::Engine::GetKeyboard()->GetKeyState(ace::Keys::V) == ace::KeyboardButtonState::Push)
		{
			auto posteffect = std::make_shared<ace::PostEffectLightBloom>();
			posteffect->SetIntensity(10.0f);
			posteffect->SetPower(1.0f);
			posteffect->SetThreshold(0.3f);
			layer->AddPostEffect(posteffect);
		}
	}

	// aceを終了する。
	ace::Engine::Terminate();
}

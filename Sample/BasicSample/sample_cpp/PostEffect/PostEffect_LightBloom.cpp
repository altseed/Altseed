#include <Altseed.h>

// レイヤーにライトブルームのポストエフェクトを適用する。
void PostEffect_LightBloom()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("PostEffect_LightBloom").c_str(), 640, 480, asd::EngineOption());

	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

	// シーンクラスのインスタンスを生成する。
	auto scene = std::make_shared<asd::Scene>();

	// レイヤークラスのインスタンスを生成する。
	auto layer = std::make_shared<asd::Layer2D>();

	// 画像描画オブジェクトのインスタンスを生成する。
	auto obj = std::make_shared<asd::TextureObject2D>();

	// オブジェクトの位置とテクスチャを設定。
	obj->SetPosition(asd::Vector2DF(50, 50));
	obj->SetTexture(texture);

	// シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
	asd::Engine::ChangeScene(scene);
	scene->AddLayer(layer);
	layer->AddObject(obj);

	// ライトブルームクラスのインスタンスを生成する。
	auto posteffect = std::make_shared<asd::PostEffectLightBloom>();

	// ライトブルームのぼかしの強さを設定する。
	posteffect->SetIntensity(10.0f);

	// ライトブルームの露光の強さを設定する。
	posteffect->SetExposure(1.0f);

	// ライトブルームで光らせる明るさのしきい値を設定する。
	posteffect->SetThreshold(0.3f);

	// レイヤーにライトブルームのポストエフェクトを適用。
	layer->AddPostEffect(posteffect);

	// HDRモードをONにする（ライトブルームの効果が見えるようにするため）
	scene->SetHDRMode(true);

	// Altseedのウインドウが閉じられていないか確認する。 
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}
	
	// Altseedを終了する。
	asd::Engine::Terminate();
}
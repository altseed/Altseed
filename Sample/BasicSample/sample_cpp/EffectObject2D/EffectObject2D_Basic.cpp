
#include <Altseed.h>

// EffectObject2Dのサンプル。エフェクトを表示しています。
void EffectObject2D_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("EffectObject2D_Basic").c_str(), 640, 480, asd::EngineOption());

	// エフェクトを読み込む
	auto effect = asd::Engine::GetGraphics()->CreateEffect(asd::ToAString("Data/Effect/magic.efk").c_str());

	// エフェクトオブジェクトを生成する
	auto effectObj = std::make_shared<asd::EffectObject2D>();

	// エフェクトオブジェクトをエンジンに追加する
	asd::Engine::AddObject2D(effectObj);
	
	// エフェクトを設定する
	effectObj->SetEffect(effect);

	// エフェクトの位置、大きさを指定する
	effectObj->SetPosition(asd::Vector2DF(320, 240));
	effectObj->SetScale(asd::Vector2DF(50, 50));

	// エフェクトを再生する
	effectObj->Play();

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();
}
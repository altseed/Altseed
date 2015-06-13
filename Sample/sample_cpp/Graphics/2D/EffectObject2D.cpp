
#include <Base.h>

/**
@brief	エフェクトを表示する。
*/
void EffectObject2D()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("EffectObject2D").c_str(), 640, 480, asd::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<asd::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	asd::Engine::ChangeScene(scene);

	// エフェクトを読み込む
	auto effect = asd::Engine::GetGraphics()->CreateEffect(asd::ToAString("Data/Effect/magic.efk").c_str());

	// エフェクトオブジェクトを生成する
	auto effectObj = std::make_shared<asd::EffectObject2D>();

	// エフェクトをレイヤーに追加する
	layer->AddObject(effectObj);

	// エフェクトの位置、大きさを指定する
	effectObj->SetPosition(asd::Vector2DF(320, 240));
	effectObj->SetScale(asd::Vector2DF(50, 50));
	
	// エフェクトを設定する
	effectObj->SetEffect(effect);

	// エフェクトを再生する
	effectObj->Play();

	// aceが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		// aceを更新する。
		asd::Engine::Update();
	}

	// aceを終了する。
	asd::Engine::Terminate();
}
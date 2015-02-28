#include <Base.h>

/**
@brief	エフェクトを表示する。
*/
void CameraObject2D()
{
	// aceを初期化する
	ace::Engine::Initialize(ace::ToAString("CameraObject2D").c_str(), 640, 480, ace::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<ace::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<ace::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	ace::Engine::ChangeScene(scene);


	{
		// テクスチャを生成する
		auto tex0 = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample2.png").c_str());

		//TextureObject2Dを生成する
		auto obj0 = std::make_shared<ace::TextureObject2D>();

		//描画に使うテクスチャを設定する
		obj0->SetTexture(tex0);

		//描画位置を指定する
		obj0->SetPosition(ace::Vector2DF(100, 100));

		//レイヤーへ追加する
		layer->AddObject(obj0);
	}

	auto camera1 = std::make_shared<ace::CameraObject2D>();
	camera1->SetSrc(ace::RectI(0, 0, 200, 200));
	camera1->SetDst(ace::RectI(0, 0, 200, 200));

	layer->AddObject(camera1);


	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		// aceを更新する。
		ace::Engine::Update();
	}

	// aceを終了する。
	ace::Engine::Terminate();
}
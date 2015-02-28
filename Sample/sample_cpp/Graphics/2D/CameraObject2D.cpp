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
		auto tex0 = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());

		//TextureObject2Dを生成する
		auto obj0 = std::make_shared<ace::TextureObject2D>();

		//描画に使うテクスチャを設定する
		obj0->SetTexture(tex0);

		//描画位置を指定する
		obj0->SetCenterPosition(ace::Vector2DF(256, 256));
		obj0->SetPosition(ace::Vector2DF(320, 240));
		obj0->SetScale(ace::Vector2DF(0.5f, 0.5f));

		//レイヤーへ追加する
		layer->AddObject(obj0);
	}

	//画面全体を写すカメラ(オブジェクトをそのまま描画)
	auto camera1 = std::make_shared<ace::CameraObject2D>();
	camera1->SetSrc(ace::RectI(0, 0, 640, 480));
	camera1->SetDst(ace::RectI(0, 0, 640, 480));

	layer->AddObject(camera1);

	//ポインタ周りを拡大して表示するカメラ
	auto camera2 = std::make_shared<ace::CameraObject2D>();

	layer->AddObject(camera2);

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		auto pos = ace::Engine::GetMouse()->GetPosition();

		camera2->SetDst(ace::RectI(pos.X - 50, pos.Y - 50, 100, 100));
		camera2->SetSrc(ace::RectI(pos.X - 25, pos.Y - 25, 50, 50));

		// aceを更新する。
		ace::Engine::Update();
	}

	// aceを終了する。
	ace::Engine::Terminate();
}
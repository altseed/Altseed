
#include <Altseed.h>

// CameraObject2Dのサンプル。マウスポインタの先を拡大表示しています。
void CameraObject2D()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("CameraObject2D").c_str(), 640, 480, asd::EngineOption());

	// シーンを生成する。
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーを生成する。
	auto layer = std::make_shared<asd::Layer2D>();

	// シーンにレイヤーを追加する。
	scene->AddLayer(layer);

	// シーンを切り替える。
	asd::Engine::ChangeScene(scene);

	// 画面にテクスチャを表示するオブジェクトを生成する。
	{
		auto tex0 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());
		auto obj0 = std::make_shared<asd::TextureObject2D>();
		layer->AddObject(obj0);
	}

	// 画面にテクスチャを表示するオブジェクトを生成する。
	{
		auto baseCameraObj = std::make_shared<asd::CameraObject2D>();
		baseCameraObj->SetSrc(asd::RectI(0, 0, 640, 480));
		baseCameraObj->SetDst(asd::RectI(0, 0, 640, 480));
		layer->AddObject(baseCameraObj);
	}

	// マウスポインタの周辺を拡大表示するカメラを生成する。
	auto cameraObj = std::make_shared<asd::CameraObject2D>();
	layer->AddObject(cameraObj);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// マウスポインタの位置を取得する。
		auto pos = asd::Engine::GetMouse()->GetPosition();

		// 拡大用カメラの描画元を指定する。
		cameraObj->SetSrc(asd::RectI(pos.X - 25, pos.Y - 25, 50, 50));

		// ポインタを中心に100x100の拡大画像を表示する。
		cameraObj->SetDst(asd::RectI(pos.X - 50, pos.Y - 50, 100, 100));

		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
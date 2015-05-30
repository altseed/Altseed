
#include <ace.h>

// CameraObject2Dのサンプル。マウスポインタの先を拡大表示しています。
void CameraObject2D()
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("CameraObject2D").c_str(), 640, 480, ace::EngineOption());

	// シーンを生成する。
	auto scene = std::make_shared<ace::Scene>();

	// レイヤーを生成する。
	auto layer = std::make_shared<ace::Layer2D>();

	// シーンにレイヤーを追加する。
	scene->AddLayer(layer);

	// シーンを切り替える。
	ace::Engine::ChangeScene(scene);

	// 画面にテクスチャを表示するオブジェクトを生成する。
	{
		auto tex0 = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		auto obj0 = std::make_shared<ace::TextureObject2D>();
		layer->AddObject(obj0);
	}

	// 画面にテクスチャを表示するオブジェクトを生成する。
	{
		auto baseCameraObj = std::make_shared<ace::CameraObject2D>();
		baseCameraObj->SetSrc(ace::RectI(0, 0, 640, 480));
		baseCameraObj->SetDst(ace::RectI(0, 0, 640, 480));
		layer->AddObject(baseCameraObj);
	}

	// マウスポインタの周辺を拡大表示するカメラを生成する。
	auto cameraObj = std::make_shared<ace::CameraObject2D>();
	layer->AddObject(cameraObj);

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		// マウスポインタの位置を取得する。
		auto pos = ace::Engine::GetMouse()->GetPosition();

		// 拡大用カメラの描画元を指定する。
		cameraObj->SetSrc(ace::RectI(pos.X - 25, pos.Y - 25, 50, 50));

		// ポインタを中心に100x100の拡大画像を表示する。
		cameraObj->SetDst(ace::RectI(pos.X - 50, pos.Y - 50, 100, 100));

		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	// AC-Engineの終了処理をする。
	ace::Engine::Terminate();

	return;
}
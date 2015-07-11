#include <Base.h>

/**
@brief	カメラを用いた描画空間の一部を切り取って描画するサンプルを表示する。
*/
void CameraObject2D_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("CameraObject2D_Basic").c_str(), 640, 480, asd::EngineOption());

	// 画像を読み込む。
	auto tex0 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

	// テクスチャを描画するオブジェクトを設定する。
	auto obj0 = std::make_shared<asd::TextureObject2D>();
	obj0->SetTexture(tex0);
	obj0->SetPosition(asd::Vector2DF(10, 10));
	obj0->SetScale(asd::Vector2DF(0.7f, 0.7f));
	asd::Engine::AddObject2D(obj0);

	//画面全体を写すカメラを設定する。(オブジェクトをそのまま描画する。)
	auto entityCamera = std::make_shared<asd::CameraObject2D>();
	entityCamera->SetSrc(asd::RectI(0, 0, 640, 480));
	entityCamera->SetDst(asd::RectI(0, 0, 640, 480));
	asd::Engine::AddObject2D(entityCamera);

	//テクスチャの左上から縦横150ピクセルを切り取って描画するカメラを設定する。
	auto camera = std::make_shared<asd::CameraObject2D>();
	camera->SetSrc(asd::RectI(10, 10, 150, 150));
	camera->SetDst(asd::RectI(450, 10, 150, 150));
	asd::Engine::AddObject2D(camera);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
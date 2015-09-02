#include <Base.h>

/**
@brief	カメラに特定のオブジェクトのみを表示する。
*/
void CameraObject2D_Group()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("CameraObject2D_Group").c_str(), 640, 480, asd::EngineOption());

	// 画像を読み込む。
	auto tex = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

	// 画像を描画するオブジェクトを設定する。
	auto obj1 = std::make_shared<asd::TextureObject2D>();
	obj1->SetTexture(tex);
	obj1->SetPosition(asd::Vector2DF(10, 10));
	obj1->SetScale(asd::Vector2DF(0.5f, 0.5f));

	// グループを設定する。(描画されない)
	obj1->SetCameraGroup(1);
	asd::Engine::AddObject2D(obj1);

	// 画像を描画するオブジェクトを設定する。
	auto obj2 = std::make_shared<asd::TextureObject2D>();
	obj2->SetTexture(tex);
	obj2->SetPosition(asd::Vector2DF(310, 10));
	obj2->SetScale(asd::Vector2DF(0.5f, 0.5f));

	// グループを設定する。
	obj2->SetCameraGroup(2);
	asd::Engine::AddObject2D(obj2);

	// カメラを設定する。
	auto camera = std::make_shared<asd::CameraObject2D>();
	camera->SetSrc(asd::RectI(0, 0, 640, 480));
	camera->SetDst(asd::RectI(0, 0, 640, 480));

	// グループを設定する。
	camera->SetCameraGroup(2);

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
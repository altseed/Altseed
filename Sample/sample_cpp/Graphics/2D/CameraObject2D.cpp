#include <Base.h>

/**
@brief	エフェクトを表示する。
*/
void CameraObject2D()
{
	// aceを初期化する。
	asd::Engine::Initialize(asd::ToAString("CameraObject2D").c_str(), 640, 480, asd::EngineOption());

	// テクスチャ画像を準備する。
	{
		auto tex0 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());
		auto obj0 = std::make_shared<asd::TextureObject2D>();
		obj0->SetTexture(tex0);
		obj0->SetCenterPosition(asd::Vector2DF(256, 256));
		obj0->SetPosition(asd::Vector2DF(320, 240));
		obj0->SetScale(asd::Vector2DF(0.5f, 0.5f));

		asd::Engine::AddObject2D(obj0);
	}

	//一つ目の画面全体を写すカメラを設定する。(オブジェクトをそのまま描画する。)
	{
		auto camera1 = std::make_shared<asd::CameraObject2D>();
		camera1->SetSrc(asd::RectI(0, 0, 640, 480));
		camera1->SetDst(asd::RectI(0, 0, 640, 480));
		asd::Engine::AddObject2D(camera1);
	}

	//二つ目のマウスポインタの周辺を拡大して表示するカメラを設定する。
	auto camera2 = std::make_shared<asd::CameraObject2D>();

	asd::Engine::AddObject2D(camera2);

	//フレーム用テクスチャ画像を準備する。
	auto frame = std::make_shared<asd::TextureObject2D>();
	{
		auto tex = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Frame.png").c_str());
		frame->SetTexture(tex);
		frame->SetCenterPosition(asd::Vector2DF(55.0f, 55.0f));

		asd::Engine::AddObject2D(frame);
	}

	// aceが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		//マウスポインタの位置を取得する。
		auto pos = asd::Engine::GetMouse()->GetPosition();

		//拡大用カメラの描画元を指定する。
		camera2->SetSrc(asd::RectI(pos.X - 25, pos.Y - 25, 50, 50));

		//ポインタを中心に100x100の拡大画像を表示する。
		camera2->SetDst(asd::RectI(pos.X - 50, pos.Y - 50, 100, 100));

		//フレーム画像の描画中心をマウスポインタの位置に合わせる。
		frame->SetPosition(pos);

		// aceを更新する。
		asd::Engine::Update();
	}

	// aceを終了する。
	asd::Engine::Terminate();
}
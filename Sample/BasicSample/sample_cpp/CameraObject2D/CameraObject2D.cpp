#include <Base.h>

/**
@brief	カメラサンプルを表示する。
*/

const int SOURCE_SIZE = 50;
const int DESTINATION_SIZE = 100;

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

	//二つ目のマウスポインタの周辺を拡大して表示するカメラを設定する。
	auto camera = std::make_shared<asd::CameraObject2D>();

	asd::Engine::AddObject2D(camera);

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
		camera->SetSrc(asd::RectI(pos.X - SOURCE_SIZE / 2, pos.Y - SOURCE_SIZE / 2, SOURCE_SIZE, SOURCE_SIZE));

		//ポインタを中心に100x100の拡大画像を表示する。
		camera->SetDst(asd::RectI(pos.X - DESTINATION_SIZE / 2, pos.Y - DESTINATION_SIZE / 2, DESTINATION_SIZE, DESTINATION_SIZE));

		//フレーム画像の描画中心をマウスポインタの位置に合わせる。
		frame->SetPosition(pos);

		// aceを更新する。
		asd::Engine::Update();
	}

	// aceを終了する。
	asd::Engine::Terminate();
}
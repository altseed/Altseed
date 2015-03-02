#include <Base.h>

/**
@brief	エフェクトを表示する。
*/
void CameraObject2D()
{
	// aceを初期化する。
	ace::Engine::Initialize(ace::ToAString("CameraObject2D").c_str(), 640, 480, ace::EngineOption());

	// シーンを生成する。
	auto scene = std::make_shared<ace::Scene>();

	// レイヤーを生成する。
	auto layer = std::make_shared<ace::Layer2D>();

	// シーンにレイヤーを追加する。
	scene->AddLayer(layer);

	// シーンを切り替える。
	ace::Engine::ChangeScene(scene);

	// テクスチャ画像を準備する。
	{
		auto tex0 = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		auto obj0 = std::make_shared<ace::TextureObject2D>();
		obj0->SetTexture(tex0);
		obj0->SetCenterPosition(ace::Vector2DF(256, 256));
		obj0->SetPosition(ace::Vector2DF(320, 240));
		obj0->SetScale(ace::Vector2DF(0.5f, 0.5f));

		layer->AddObject(obj0);
	}

	//一つ目の画面全体を写すカメラを設定する。(オブジェクトをそのまま描画する。)
	{
		auto camera1 = std::make_shared<ace::CameraObject2D>();
		camera1->SetSrc(ace::RectI(0, 0, 640, 480));
		camera1->SetDst(ace::RectI(0, 0, 640, 480));
		layer->AddObject(camera1);
	}

	//二つ目のマウスポインタの周辺を拡大して表示するカメラを設定する。
	auto camera2 = std::make_shared<ace::CameraObject2D>();

	layer->AddObject(camera2);

	//フレーム用テクスチャ画像を準備する。
	auto frame = std::make_shared<ace::TextureObject2D>();
	{
		auto tex = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Frame.png").c_str());
		frame->SetTexture(tex);
		frame->SetCenterPosition(ace::Vector2DF(55.0f, 55.0f));

		layer->AddObject(frame);
	}

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		//マウスポインタの位置を取得する。
		auto pos = ace::Engine::GetMouse()->GetPosition();

		//拡大用カメラの描画元を指定する。
		camera2->SetSrc(ace::RectI(pos.X - 25, pos.Y - 25, 50, 50));

		//ポインタを中心に100x100の拡大画像を表示する。
		camera2->SetDst(ace::RectI(pos.X - 50, pos.Y - 50, 100, 100));

		//フレーム画像の描画中心をマウスポインタの位置に合わせる。
		frame->SetPosition(pos);

		// aceを更新する。
		ace::Engine::Update();
	}

	// aceを終了する。
	ace::Engine::Terminate();
}
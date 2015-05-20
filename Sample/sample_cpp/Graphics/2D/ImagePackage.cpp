
#include <Base.h>

/**
@brief	エフェクトを表示する。
*/
void ImagePackage()
{
	// aceを初期化する
	ace::Engine::Initialize(ace::ToAString("ImagePackage").c_str(), 640, 480, ace::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<ace::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<ace::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	ace::Engine::ChangeScene(scene);

	// イメージパッケージを読み込む
	auto imagePackage = ace::Engine::GetGraphics()->CreateImagePackage(ace::ToAString("Data/ImagePackage/Game.aip").c_str());

	for (int i = 0; i < imagePackage->GetImageCount(); i++)
	{
		// テクスチャを取り出す
		auto texture = imagePackage->GetImage(i);
		auto area = imagePackage->GetImageArea(i);

		// テクスチャをオブジェクトとして追加する
		auto textureObject2D = std::make_shared<ace::TextureObject2D>();
		textureObject2D->SetTexture(texture);
		textureObject2D->SetPosition(ace::Vector2DF(area.X, area.Y));
		layer->AddObject(textureObject2D);
	}

	// aceが進行可能かチェックする。
	while (ace::Engine::DoEvents())
	{
		// aceを更新する。
		ace::Engine::Update();
	}

	// aceを終了する。
	ace::Engine::Terminate();
}
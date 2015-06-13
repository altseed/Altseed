
#include <Base.h>

/**
@brief	エフェクトを表示する。
*/
void ImagePackage()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("ImagePackage").c_str(), 640, 480, asd::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<asd::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	asd::Engine::ChangeScene(scene);

	// イメージパッケージを読み込む
	auto imagePackage = asd::Engine::GetGraphics()->CreateImagePackage(asd::ToAString("Data/ImagePackage/Game.aip").c_str());

	for (int i = 0; i < imagePackage->GetImageCount(); i++)
	{
		// テクスチャを取り出す
		auto texture = imagePackage->GetImage(i);
		auto area = imagePackage->GetImageArea(i);

		// テクスチャをオブジェクトとして追加する
		auto textureObject2D = std::make_shared<asd::TextureObject2D>();
		textureObject2D->SetTexture(texture);
		textureObject2D->SetPosition(asd::Vector2DF(area.X, area.Y));
		layer->AddObject(textureObject2D);
	}

	// aceが進行可能かチェックする。
	while (asd::Engine::DoEvents())
	{
		// aceを更新する。
		asd::Engine::Update();
	}

	// aceを終了する。
	asd::Engine::Terminate();
}
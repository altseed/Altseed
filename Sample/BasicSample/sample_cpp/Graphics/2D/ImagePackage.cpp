
#include <ace.h>

/**
@brief	ImagePackageのサンプル。画像パッケージを元に画像を配置する。
*/
void ImagePackage()
{
	// AC-Engineを初期化する。
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

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	// AC-Engineの終了処理をする。
	ace::Engine::Terminate();
}
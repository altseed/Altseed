
#include <Altseed.h>

// ImagePackageを用いてUIを配置するサンプル。
void ImagePackageUI_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("ImagePackageUI_Basic").c_str(), 640, 480, asd::EngineOption());

	// イメージパッケージを読み込む
	auto imagePackage = asd::Engine::GetGraphics()->CreateImagePackage(asd::ToAString("Data/ImagePackage/UI.aip").c_str());

	for (int i = 0; i < imagePackage->GetImageCount(); i++)
	{
		// テクスチャを取り出す
		auto texture = imagePackage->GetImage(i);
		auto area = imagePackage->GetImageArea(i);

		// テクスチャをオブジェクトとして追加する
		auto textureObject2D = std::make_shared<asd::TextureObject2D>();
		textureObject2D->SetTexture(texture);
		textureObject2D->SetPosition(asd::Vector2DF(area.X, area.Y));
		asd::Engine::AddObject2D(textureObject2D);
	}

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}
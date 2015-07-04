
#include <Altseed.h>

// ImagePackageを用いてUIを配置して、特定の名前の画像のアルファブレンドの方法を変更するサンプル。
void ImagePackageUI_AlphaBlend()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("ImagePackageUI_AlphaBlend").c_str(), 640, 480, asd::EngineOption());

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

		// Background_Lightという名称の画像のアルファブレンドの方法を変更する。
		auto name = imagePackage->GetImageName(i);
		if (asd::astring(imagePackage->GetImageName(i)) == asd::ToAString("Background_Light"))
		{
			textureObject2D->SetAlphaBlendMode(asd::AlphaBlendMode::Add);
		}
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

#include <Altseed.h>

// ImagePackageを用いてUIを配置して、特定の名前の画像をアニメーションさせるサンプル。

class AlphaAnimationComponent 
	: public asd::Object2DComponent
{
	int time = 0;
	void OnUpdate() override
	{
		// アルファ値をsinカーブでアニメーションするようにする。
		auto alpha = (uint8_t) ((float) (sin(time / 20.0f) + 1.0f) / 2.0f * 255);
		auto owner = (asd::TextureObject2D*)GetOwner();
		owner->SetColor(asd::Color(255, 255, 255, alpha));
		time++;
	}
};

void ImagePackageUI_Component()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("ImagePackageUI_Component").c_str(), 640, 480, asd::EngineOption());

	// シーンを生成する
	auto scene = std::make_shared<asd::Scene>();

	// レイヤーを生成する
	auto layer = std::make_shared<asd::Layer2D>();

	// シーンにレイヤーを追加する
	scene->AddLayer(layer);

	// シーンを切り替える
	asd::Engine::ChangeScene(scene);

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
		layer->AddObject(textureObject2D);

		// Background_Lightという名称の画像にコンポーネントを適用する。
		auto name = imagePackage->GetImageName(i);
		if (asd::astring(imagePackage->GetImageName(i)) == asd::ToAString("Background_Light"))
		{
			textureObject2D->SetAlphaBlendMode(asd::AlphaBlendMode::Add);
			textureObject2D->AddComponent(std::make_shared<AlphaAnimationComponent>(), asd::ToAString("AlphaAnimation"));
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
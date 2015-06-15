
#include <Base.h>

/**
@brief	テクスチャを上下反転して描画するサンプル。
*/
void TextureObject2D_Turn()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("TextureObject2D_Turn").c_str(), 640, 480, asd::EngineOption());

	{
		auto tex1 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample2.png").c_str());

		auto obj1 = std::make_shared<asd::TextureObject2D>();

		obj1->SetTexture(tex1);

		obj1->SetPosition(asd::Vector2DF(500, 400));

		//画像を上下に反転する
		obj1->SetTurnUL(true);

		asd::Engine::AddObject2D(obj1);
	}


	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
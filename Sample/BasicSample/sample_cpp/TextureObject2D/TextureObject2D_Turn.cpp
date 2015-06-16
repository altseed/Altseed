
#include <Altseed.h>

/**
@brief	画像を上下反転して描画するサンプル。
*/
void TextureObject2D_Turn()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("TextureObject2D_Turn").c_str(), 640, 480, asd::EngineOption());

	// 画像を読み込む。
	std::shared_ptr<asd::Texture2D> texture =
		asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

	// TextureObject2Dのインスタンスを生成する。
	std::shared_ptr<asd::TextureObject2D> obj = std::make_shared<asd::TextureObject2D>();

	// 描画される画像を設定する。
	obj->SetTexture(texture);

	// 描画位置を指定する。
	obj->SetPosition(asd::Vector2DF(50, 50));

	// 画像を上下に反転する。
	obj->SetTurnUL(true);

	// オブジェクトのインスタンスをエンジンに追加する。
	asd::Engine::AddObject2D(obj);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
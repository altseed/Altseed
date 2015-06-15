
#include <Base.h>

/**
@brief	テクスチャを表示するサンプル。
*/
void TextureObject2D_Simple()
{
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("TextureObject2D_Simple").c_str(), 640, 480, asd::EngineOption());

	{
		// テクスチャを生成する
		auto tex0 = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample2.png").c_str());

		//TextureObject2Dを生成する
		auto obj0 = std::make_shared<asd::TextureObject2D>();

		//描画に使うテクスチャを設定する
		obj0->SetTexture(tex0);

		//描画位置を指定する
		obj0->SetPosition(asd::Vector2DF(100, 400));

		//レイヤーへ追加する
		asd::Engine::AddObject2D(obj0);
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
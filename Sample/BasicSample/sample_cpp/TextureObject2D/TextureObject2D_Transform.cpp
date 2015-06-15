
#include <Base.h>

/**
@brief	エフェクトを変形させて表示するサンプル。
*/
void TextureObject2D_Transform()
{
	// Altseedを初期化する
	asd::Engine::Initialize(asd::ToAString("TextureObject2D_Transform").c_str(), 640, 480, asd::EngineOption());

	auto obj = std::make_shared<asd::TextureObject2D>();
	{

		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());

		//画像(サイズ 512 x 512)の中心(256 x 256 の地点)を描画の起点とする
		obj->SetCenterPosition(asd::Vector2DF(256, 256));
		obj->SetTexture(texture);

		obj->SetPosition(asd::Vector2DF(320, 240));

		//画像を中心を軸に45度回転する
		obj->SetAngle(45);

		//画像をX,Y方向に0.4倍に縮小する
		obj->SetScale(asd::Vector2DF(0.4f, 0.4f));

		asd::Engine::AddObject2D(obj);
	}

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		//オブジェクトを0.5度回転させる。
		float angle = obj->GetAngle();
		angle += 0.5f;
		obj->SetAngle(angle);

		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
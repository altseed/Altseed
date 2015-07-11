
#include <Altseed.h>

/**
@brief	画像を変形させて表示するサンプル。
*/
void TextureObject2D_Transform()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("TextureObject2D_Transform").c_str(), 640, 480, asd::EngineOption());

	// 画像を読み込む。
	std::shared_ptr<asd::Texture2D> texture =
		asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

	// 画像描画オブジェクトのインスタンスを生成する。
	std::shared_ptr<asd::TextureObject2D> obj = std::make_shared<asd::TextureObject2D>();

	// 描画される画像を設定する。
	obj->SetTexture(texture);

	// 描画位置を指定する。
	obj->SetPosition(asd::Vector2DF(320, 240));

	// 画像(サイズ 512 x 512)の中心(256 x 256 の地点)を描画の起点とする。
	obj->SetCenterPosition(asd::Vector2DF(256, 256));

	// 中心を軸に画像を45度回転する。
	obj->SetAngle(45);

	// 画像をX,Y方向に0.4倍に縮小する。
	obj->SetScale(asd::Vector2DF(0.4f, 0.4f));
	
	// 画像描画オブジェクトのインスタンスをエンジンに追加する。
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
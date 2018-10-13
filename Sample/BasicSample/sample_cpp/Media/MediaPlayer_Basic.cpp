
#include <Altseed.h>

/**
@brief	映像を表示するサンプル
*/
void MediaPlayer_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(u"MediaPlayer_Basic", 640, 480, asd::EngineOption());

	// 空の画像を生成する。
	std::shared_ptr<asd::Texture2D> texture =
		asd::Engine::GetGraphics()->CreateEmptyTexture2D(640, 480, asd::TextureFormat::R8G8B8A8_UNORM_SRGB);

	// 画像描画オブジェクトのインスタンスを生成する。
	std::shared_ptr<asd::TextureObject2D> obj = std::make_shared<asd::TextureObject2D>();

	// 描画される画像を設定する。
	obj->SetTexture(texture);

	// 画像描画オブジェクトのインスタンスをエンジンに追加する。
	asd::Engine::AddObject2D(obj);

	// メディアプレイヤーを生成する。
	std::shared_ptr<asd::MediaPlayer> mp = asd::Engine::GetGraphics()->CreateMediaPlayer();

	// 映像を読み込む。
	mp->Load(u"Data/Movie/Sample1.mp4");

	// 映像を再生する。
	mp->Play();

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// 映像を画像にコピーする。
		mp->WriteToTexture2D(texture);

		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
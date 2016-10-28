
#include <Altseed.h>

/**
@brief	読み込まれた画像を編集するサンプル
*/
void Texture_Edit()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(u"Texture_Edit", 640, 480, asd::EngineOption());

	// 画像を編集可能な状態で読み込む。
	std::shared_ptr<asd::Texture2D> texture =
		asd::Engine::GetGraphics()->CreateEditableTexture2D(u"Data/Texture/Picture1.png");

	// 画像を編集する。
	asd::TextureLockInfomation lockInfo;
	
	// ロックして編集可能な状態にする。
	if (texture->Lock(&lockInfo))
	{
		for (int32_t y = 0; y < lockInfo.GetSize().Y; y++)
		{
			for (int32_t x = 0; x < lockInfo.GetSize().X; x++)
			{
				auto pixel = &((uint8_t*) lockInfo.GetPixels())[(x + y *  lockInfo.GetSize().X) * lockInfo.GetPitch()];
				pixel[1] = y / 2;
			}
		}

		// Unlockして編集結果を適用する。
		texture->Unlock();
	}

	// 画像描画オブジェクトのインスタンスを生成する。
	std::shared_ptr<asd::TextureObject2D> obj = std::make_shared<asd::TextureObject2D>();

	// 描画される画像を設定する。
	obj->SetTexture(texture);

	// 描画位置を指定する。
	obj->SetPosition(asd::Vector2DF(50, 50));

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
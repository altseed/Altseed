
#include <Altseed.h>

/**
@brief	空の画像を編集するサンプル
*/
void Texture_Basic()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Texture_Basic").c_str(), 640, 480, asd::EngineOption());

	// 空の画像を生成する。
	std::shared_ptr<asd::Texture2D> texture =
		asd::Engine::GetGraphics()->CreateEmptyTexture2D(256, 256, asd::TextureFormat::R8G8B8A8_UNORM_SRGB);

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
				pixel[0] = x;
				pixel[1] = y;
				pixel[2] = 0;
				pixel[3] = 255;
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
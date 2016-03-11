
#include <Altseed.h>

// Disposeが呼ばれた時や破棄されたときにコンソールに出力するオブジェクト。
class Dispose_MessageObject : public asd::TextureObject2D
{
protected:
	// Disposeが呼ばれたときに呼ばれる。
	void OnDispose()
	{
		printf("Dispose_MessageObject.OnDispose\n");
	}
};

void Dispose_Object2D()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Dispose_Object2D").c_str(), 640, 480, asd::EngineOption());

	// 画像を読み込む。
	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

	// 画像描画オブジェクトのインスタンスを生成する。
	auto obj = std::make_shared<Dispose_MessageObject>();

	// 描画される画像を設定する。
	obj->SetTexture(texture);

	// 描画位置を指定する。
	obj->SetPosition(asd::Vector2DF(50, 50));

	// 画像描画オブジェクトのインスタンスをエンジンに追加する。
	asd::Engine::AddObject2D(obj);

	// フレーム数を数えるための変数。
	int count = 0;

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();

		// フレームを数える。
		++count;

		// 10フレーム目になったら
		if (count == 10)
		{
			printf("オブジェクトを破棄します\n");
			// オブジェクトを破棄する。
			obj->Dispose();
		}
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
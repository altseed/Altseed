
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

// Disposeが呼ばれた時や破棄されたときにコンソールに出力するレイヤー。
class Dispose_MessageLayer : public asd::Layer2D
{
protected:
	// Disposeが呼ばれたときに呼ばれる。
	void OnDispose()
	{
		printf("Dispose_MessageLayer.OnDispose\n");
	}
};

void Dispose_Layer2D()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("SceneAndLayer_Basic").c_str(), 640, 480, asd::EngineOption());

	// 画像を読み込む。
	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

	// シーンクラスのインスタンスを生成する。
	auto scene = std::make_shared<asd::Scene>();

	// カスタマイズしたレイヤーのインスタンスを生成する。
	auto layer = std::make_shared<Dispose_MessageLayer>();

	// カスタマイズしたオブジェクトのインスタンスを生成する。
	auto obj = std::make_shared<Dispose_MessageObject>();

	// オブジェクトの位置とテクスチャを設定する。
	obj->SetPosition(asd::Vector2DF(50, 50));
	obj->SetTexture(texture);

	// 描画するシーンを指定する。
	asd::Engine::ChangeScene(scene);

	// 描画するレイヤーをシーンに追加する。
	scene->AddLayer(layer);

	// 描画するオブジェクトをレイヤーに追加する。
	layer->AddObject(obj);

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
			printf("レイヤーを破棄します\n");
			// レイヤーを破棄する。
			layer->Dispose();
		}
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
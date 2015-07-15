#include <Altseed.h>

// このサンプルで使用するカスタマイズしたレイヤー。
class SceneAndLayer_SampleLayer : public asd::Layer2D
{
private:
	// フレーム数を数えるカウンタ。
	int count;

public:
	SceneAndLayer_SampleLayer()
	{
		// カウンタを初期化
		count = 0;
	}

	// 毎フレーム実行される関数をオーバーライド。
	void OnUpdated()
	{
		// フレーム数を数える
		count++;

		// 60フレームごとに
		if (count % 60 == 0)
		{
			// オブジェクトを生成
			auto obj = std::make_shared<asd::TextureObject2D>();

			// オブジェクトのテクスチャを設定
			auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());
			obj->SetTexture(texture);

			// オブジェクトの位置を設定
			auto p = count / 60 * 20;
			obj->SetPosition(asd::Vector2DF(p, p));

			// オブジェクトをこのレイヤーに追加
			AddObject(obj);
		}
	}
};

// Layer2Dを継承することでカスタマイズしたレイヤーを作り、一定時間ごとにオブジェクトを追加するサンプル。
void SceneAndLayer_CustomLayer()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("SceneAndLayer_CustomLayer").c_str(), 640, 480, asd::EngineOption());

	// シーンクラスのインスタンスを生成する。
	auto scene = std::make_shared<asd::Scene>();

	// 描画するシーンを指定する。
	asd::Engine::ChangeScene(scene);

	// レイヤークラスのインスタンスを生成する。
	auto layer = std::make_shared<SceneAndLayer_SampleLayer>();

	// 描画するレイヤーをシーンに追加する。
	scene->AddLayer(layer);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
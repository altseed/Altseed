#include <Altseed.h>

// このサンプルで使用するカスタマイズされたシーン。
class SceneAndLayer_SampleScene : public asd::Scene
{
private:
	// フレーム数を数えるカウンタ。
	int count;

public:
	SceneAndLayer_SampleScene()
	{
		// カウンタを初期化
		count = 0;
	}

	void OnUpdated()
	{
		// フレーム数を数える。
		count++;

		// 60フレームごとにレイヤーを追加する処理
		if (count % 60 == 0)
		{
			// レイヤークラスのインスタンスを生成する。
			auto layer = std::make_shared<asd::Layer2D>();

			// 描画するレイヤーをシーンに追加する。
			AddLayer(layer);

			// オブジェクトを生成する。
			auto obj = std::make_shared<asd::TextureObject2D>();

			// オブジェクトに画像を設定する。
			auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());
			obj->SetTexture(texture);

			// オブジェクトの位置を設定する。
			auto p = count / 60 * 20;
			obj->SetPosition(asd::Vector2DF(p, p));

			// オブジェクトをレイヤーに追加する。
			layer->AddObject(obj);
		}
	}
};

// Sceneクラスを継承することでカスタマイズされたシーンを作り、一定時間ごとにレイヤーを追加していくサンプル。
void SceneAndLayer_CustomScene()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("SceneAndLayer_CustomScene").c_str(), 640, 480, asd::EngineOption());

	// シーンクラスのインスタンスを生成する。
	auto scene = std::make_shared<SceneAndLayer_SampleScene>();

	// 描画するシーンを指定する。
	asd::Engine::ChangeScene(scene);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	// Altseedを終了する。
	asd::Engine::Terminate();
}
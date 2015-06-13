
#include <Altseed.h>

// Transitionを用いて、シーンの変更の見栄えを変更するサンプルです。

// Scene1の画像を表示するシーン
class Scene1 : public asd::Scene
{
protected:
	virtual void OnUpdated() override;
public:
	Scene1();
};

// Scene2の画像を表示するシーン
class Scene2 : public asd::Scene
{
protected:
	virtual void OnUpdated() override;
public:
	Scene2();
};

Scene1::Scene1()
{
	// レイヤーを追加する。
	auto layer = std::make_shared<asd::Layer2D>();
	AddLayer(layer);

	// Scene1を描画するインスタンスを生成してレイヤーに追加する。
	auto obj = std::make_shared<asd::TextureObject2D>();
	obj->SetTexture(asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Scene1.png").c_str()));
	layer->AddObject(obj);
}

void Scene1::OnUpdated()
{
	// マウスが左クリックされた時にフェード付きでScene2に遷移する。
	if (asd::Engine::GetMouse()->GetLeftButton()->GetButtonState() == asd::MouseButtonState::Push)
	{
		auto scene2 = std::make_shared<Scene2>();
		asd::Engine::ChangeSceneWithTransition(scene2, std::make_shared<asd::TransitionFade>(1.0f, 1.0f));
	}
}

Scene2::Scene2()
{
	// レイヤーを追加する。
	auto layer = std::make_shared<asd::Layer2D>();
	AddLayer(layer);

	// Scene2を描画するインスタンスを生成してレイヤーに追加する。
	auto obj = std::make_shared<asd::TextureObject2D>();
	obj->SetTexture(asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Scene2.png").c_str()));
	layer->AddObject(obj);
}

void Scene2::OnUpdated()
{
	// マウスが左クリックされた時にScene1に遷移する。
	if (asd::Engine::GetMouse()->GetLeftButton()->GetButtonState() == asd::MouseButtonState::Push)
	{
		auto& scene1 = std::make_shared<Scene1>();
		asd::Engine::ChangeScene(scene1);
	}
}

void Transition()
{
	//Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Transition").c_str(), 640, 480, asd::EngineOption());

	// シーンをScene1に変更する。
	auto scene = std::make_shared<Scene1>();
	asd::Engine::ChangeScene(scene);

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}

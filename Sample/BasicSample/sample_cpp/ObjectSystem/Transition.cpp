
#include <ace.h>

// Transitionを用いて、シーンの変更の見栄えを変更するサンプルです。

// Scene1の画像を表示するシーン
class Scene1 : public ace::Scene
{
protected:
	virtual void OnUpdated() override;
public:
	Scene1();
};

// Scene2の画像を表示するシーン
class Scene2 : public ace::Scene
{
protected:
	virtual void OnUpdated() override;
public:
	Scene2();
};

Scene1::Scene1()
{
	// レイヤーを追加する。
	auto layer = std::make_shared<ace::Layer2D>();
	AddLayer(layer);

	// Scene1を描画するインスタンスを生成してレイヤーに追加する。
	auto obj = std::make_shared<ace::TextureObject2D>();
	obj->SetTexture(ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Scene1.png").c_str()));
	layer->AddObject(obj);
}

void Scene1::OnUpdated()
{
	// マウスが左クリックされた時にフェード付きでScene2に遷移する。
	if (ace::Engine::GetMouse()->GetLeftButton()->GetButtonState() == ace::MouseButtonState::Push)
	{
		auto scene2 = std::make_shared<Scene2>();
		ace::Engine::ChangeSceneWithTransition(scene2, std::make_shared<ace::TransitionFade>(1.0f, 1.0f));
	}
}

Scene2::Scene2()
{
	// レイヤーを追加する。
	auto layer = std::make_shared<ace::Layer2D>();
	AddLayer(layer);

	// Scene2を描画するインスタンスを生成してレイヤーに追加する。
	auto obj = std::make_shared<ace::TextureObject2D>();
	obj->SetTexture(ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Scene2.png").c_str()));
	layer->AddObject(obj);
}

void Scene2::OnUpdated()
{
	// マウスが左クリックされた時にScene1に遷移する。
	if (ace::Engine::GetMouse()->GetLeftButton()->GetButtonState() == ace::MouseButtonState::Push)
	{
		auto& scene1 = std::make_shared<Scene1>();
		ace::Engine::ChangeScene(scene1);
	}
}

void Transition()
{
	//AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("Transition").c_str(), 640, 480, ace::EngineOption());

	// シーンをScene1に変更する。
	auto scene = std::make_shared<Scene1>();
	ace::Engine::ChangeScene(scene);

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		// AC-Engineを更新する。
		ace::Engine::Update();
	}

	//AC-Engineの終了処理をする。
	ace::Engine::Terminate();

	return;
}

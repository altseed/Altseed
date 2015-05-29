#include <Base.h>

//Sample1.pngを右回転させながら表示するクラス。
class Scene1 : public ace::Scene
{
	std::shared_ptr<ace::TextureObject2D> obj;
protected:
	virtual void OnUpdated() override;
public:
	Scene1();
};

//Block1.pngを左回転させながら表示するクラス。
class Scene2 : public ace::Scene
{
	std::shared_ptr<ace::TextureObject2D> obj;
protected:
	virtual void OnUpdated() override;
public:
	Scene2();
};

Scene1::Scene1()
{
	//レイヤーを追加する。
	auto layer = std::make_shared<ace::Layer2D>();
	AddLayer(layer);

	//Sample1.pngを描画するテクスチャオブジェクトを設定する。
	obj = std::make_shared<ace::TextureObject2D>();
	obj->SetTexture(ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str()));
	obj->SetCenterPosition(ace::Vector2DF(256, 256));
	obj->SetPosition(ace::Vector2DF(320, 240));
	layer->AddObject(obj);
}

void Scene1::OnUpdated()
{
	//Sample1.pngを右方向に2度回転する。
	auto v = obj->GetAngle();
	v += 2.0;
	obj->SetAngle(v);

	//マウスが左クリックされた時にScene2へ遷移する。
	if (ace::Engine::GetMouse()->GetLeftButton()->GetButtonState() == ace::MouseButtonState::Push)
	{
		auto scene2 = std::make_shared<Scene2>();
		ace::Engine::ChangeScene(scene2);
	}
}

Scene2::Scene2()
{
	//レイヤーを追加する。
	auto layer = std::make_shared<ace::Layer2D>();
	AddLayer(layer);

	//Block1.pngを描画するテクスチャオブジェクトを設定する。
	obj = std::make_shared<ace::TextureObject2D>();
	obj->SetTexture(ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Block1.png").c_str()));
	obj->SetPosition(ace::Vector2DF(100, 100));
	layer->AddObject(obj);
}

void Scene2::OnUpdated()
{
	//Block1.pngを左方向に2度回転する。
	auto v = obj->GetAngle();
	v -= 2.0;
	obj->SetAngle(v);

	//マウスが左クリックされた時にフェード付きでScene1へ遷移する。
	//（フェードアウトとフェードインはそれぞれ1秒間かけて行う。）
	if (ace::Engine::GetMouse()->GetLeftButton()->GetButtonState() == ace::MouseButtonState::Push)
	{
		auto& scene1 = std::make_shared<Scene1>();
		ace::Engine::ChangeSceneWithTransition(scene1, std::make_shared<ace::TransitionFade>(1.0f, 1.0f));
	}
}

void Transition()
{
	//AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("Transition").c_str(), 640, 480, ace::EngineOption());

	// シーンをScene1に設定する。
	auto scene = std::make_shared<Scene1>();
	ace::Engine::ChangeScene(scene);

	// AC-Engineのウインドウが閉じられていないか確認する。
	while (ace::Engine::DoEvents())
	{
		//aceを更新する。
		ace::Engine::Update();
	}

	//AC-Engineの終了処理をする。
	ace::Engine::Terminate();
}

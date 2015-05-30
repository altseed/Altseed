
#include <ace.h>

// Object2DComponentを用いて、オブジェクトのパラメーターを変更するサンプルです。

// オブジェクトを回転させるコンポーネント
class RotatingComponent : public ace::Object2DComponent
{
public:
	void OnUpdate()
	{
		// 毎フレーム、オブジェクトの角度を回転させる
		GetOwner()->SetAngle(GetOwner()->GetAngle() + 2);
	}
};

void Object2DComponent()
{
	// AC-Engineを初期化する。
	ace::Engine::Initialize(ace::ToAString("Object2DComponent").c_str(), 640, 480, ace::EngineOption());

	// 使用するシーン、レイヤー、オブジェクト、コンポーネントを生成する。
	auto scene = std::make_shared<ace::Scene>();
	auto layer = std::make_shared<ace::Layer2D>();
	auto obj = std::make_shared<ace::TextureObject2D>();
	auto component = std::make_shared<RotatingComponent>();

	// シーンを変更する。
	ace::Engine::ChangeScene(scene);

	// シーンにレイヤーを追加する。
	scene->AddLayer(layer);

	// レイヤーにオブジェクトを追加する。
	layer->AddObject(obj);

	// オブジェクトに回転コンポーネントをコンポーネント名"Rotate"で追加する。
	obj->AddComponent(component, ace::ToAString("Rotate"));

	// 画像を読み込む。
	auto texture = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Block1.png").c_str());

	// オブジェクトに画像を設定する。
	obj->SetTexture(texture);

	// オブジェクトの位置を設定する。
	obj->SetPosition(ace::Vector2DF(320, 240));

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


#include <ace.h>

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

	// 使用するシーン、レイヤー、オブジェクト、コンポーネントを生成
	auto scene = std::make_shared<ace::Scene>();
	auto layer = std::make_shared<ace::Layer2D>();
	auto object = std::make_shared<ace::TextureObject2D>();
	auto component = std::make_shared<RotatingComponent>();


	// シーンを変更
	ace::Engine::ChangeScene(scene);

	// シーンにレイヤーを追加
	scene->AddLayer(layer);

	// レイヤーにオブジェクトを追加
	layer->AddObject(object);

	// オブジェクトに回転コンポーネントをコンポーネント名"Rotate"で追加
	object->AddComponent(component, ace::ToAString("Rotate"));


	// グラフィックスオブジェクトを取得
	auto graphics = ace::Engine::GetGraphics();

	// テクスチャをロード
	auto texture = graphics->CreateTexture2D(ace::ToAString("Data/Texture/Block1.png").c_str());

	// オブジェクトに画像をセット
	object->SetTexture(texture);

	// オブジェクトの位置を指定
	object->SetPosition(ace::Vector2DF(320, 240));


	while (ace::Engine::DoEvents())
	{
		ace::Engine::Update();
	}

	ace::Engine::Terminate();
}
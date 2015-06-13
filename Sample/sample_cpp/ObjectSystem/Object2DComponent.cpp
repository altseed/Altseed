#include <Base.h>

class RotatingComponent : public asd::Object2DComponent
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
	// aceを初期化する
	asd::Engine::Initialize(asd::ToAString("Object2DComponent").c_str(), 640, 480, asd::EngineOption());

	// 使用するシーン、レイヤー、オブジェクト、コンポーネントを生成
	auto scene = std::make_shared<asd::Scene>();
	auto layer = std::make_shared<asd::Layer2D>();
	auto object = std::make_shared<asd::TextureObject2D>();
	auto component = std::make_shared<RotatingComponent>();


	// シーンを変更
	asd::Engine::ChangeScene(scene);

	// シーンにレイヤーを追加
	scene->AddLayer(layer);

	// レイヤーにオブジェクトを追加
	layer->AddObject(object);

	// オブジェクトに回転コンポーネントをコンポーネント名"Rotate"で追加
	object->AddComponent(component, asd::ToAString("Rotate"));


	// グラフィックスオブジェクトを取得
	auto graphics = asd::Engine::GetGraphics();

	// テクスチャをロード
	auto texture = graphics->CreateTexture2D(asd::ToAString("Data/Texture/Block1.png").c_str());

	// オブジェクトに画像をセット
	object->SetTexture(texture);

	// オブジェクトの位置を指定
	object->SetPosition(asd::Vector2DF(320, 240));


	while (asd::Engine::DoEvents())
	{
		asd::Engine::Update();
	}

	asd::Engine::Terminate();
}
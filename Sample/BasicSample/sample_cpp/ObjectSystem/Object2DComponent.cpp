
#include <Altseed.h>

// Object2DComponentを用いて、オブジェクトのパラメーターを変更するサンプルです。

// オブジェクトを回転させるコンポーネント
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
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Object2DComponent").c_str(), 640, 480, asd::EngineOption());

	// 使用するシーン、レイヤー、オブジェクト、コンポーネントを生成する。
	auto scene = std::make_shared<asd::Scene>();
	auto layer = std::make_shared<asd::Layer2D>();
	auto obj = std::make_shared<asd::TextureObject2D>();
	auto component = std::make_shared<RotatingComponent>();

	// シーンを変更する。
	asd::Engine::ChangeScene(scene);

	// シーンにレイヤーを追加する。
	scene->AddLayer(layer);

	// レイヤーにオブジェクトを追加する。
	layer->AddObject(obj);

	// オブジェクトに回転コンポーネントをコンポーネント名"Rotate"で追加する。
	obj->AddComponent(component, asd::ToAString("Rotate"));

	// 画像を読み込む。
	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Block1.png").c_str());

	// オブジェクトに画像を設定する。
	obj->SetTexture(texture);

	// オブジェクトの位置を設定する。
	obj->SetPosition(asd::Vector2DF(320, 240));

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

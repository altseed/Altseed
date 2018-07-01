
#include <Altseed.h>

// Object2DComponentを用いて、オブジェクトのパラメーターを変更するサンプル。

// オブジェクトを回転させるコンポーネント
class Object2DComponent_Custom_RotatingComponent : public asd::Object2DComponent
{
public:
	void OnUpdate()
	{
		// 毎フレーム、オブジェクトの角度を回転させる
		GetOwner()->SetAngle(GetOwner()->GetAngle() + 2);
	}
};

void Object2DComponent_Custom()
{
	// Altseedを初期化する。
	asd::Engine::Initialize(asd::ToAString("Object2DComponent").c_str(), 640, 480, asd::EngineOption());

	// オブジェクトを生成する。
	auto obj = std::make_shared<asd::TextureObject2D>();

	// 画像を読み込む。
	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Picture1.png").c_str());

	// オブジェクトに画像を設定する。
	obj->SetTexture(texture);

	// オブジェクトの位置を設定する。
	obj->SetPosition(asd::Vector2DF(320, 240));

	// エンジンにオブジェクトを追加する。
	asd::Engine::AddObject2D(obj);

	// 回転コンポーネントを生成する。
	auto component = std::make_shared<Object2DComponent_Custom_RotatingComponent>();

	// オブジェクトに回転コンポーネントをコンポーネント名"Rotate"で追加する。
	obj->AddComponent(component, asd::ToAString("Rotate"));

	// Altseedのウインドウが閉じられていないか確認する。
	while (asd::Engine::DoEvents())
	{
		// Altseedを更新する。
		asd::Engine::Update();

		if (asd::Engine::GetKeyboard()->GetKeyState(asd::Keys::P) == asd::ButtonState::Push)
		{
			asd::Engine::TakeScreenshot(asd::ToAString("Object2DComponent.jpg").c_str());
		}
	}

	//Altseedの終了処理をする。
	asd::Engine::Terminate();

	return;
}

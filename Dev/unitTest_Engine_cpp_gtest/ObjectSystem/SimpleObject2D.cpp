#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_Simple : public EngineTest
{
public:
	ObjectSystem_Simple(bool isOpenGLMode)
		: EngineTest(asd::ToAString("Simple"), isOpenGLMode, 30)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextureObject2D>();
		auto object2 = make_shared<TextureObject2D>();

		ASSERT_NE(scene, nullptr);
		ASSERT_NE(layer, nullptr);
		ASSERT_NE(object, nullptr);
		ASSERT_NE(object2, nullptr);

		asd::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(object);
		layer->AddObject(object2);

		auto g = asd::Engine::GetGraphics();
		auto texture = g->CreateTexture2D(asd::ToAString("Data/Texture/Cloud1.png").c_str());
		object->SetTexture(texture);
		object2->SetTexture(texture);

		object->SetCenterPosition(Vector2DF(128, 128));
		object->SetAngle(5);
		object->SetPosition(Vector2DF(320, 240));
		object->SetScale(object->GetScale() + Vector2DF(0.5f, 0.5f));
	}
};

ENGINE_TEST(ObjectSystem, Simple)


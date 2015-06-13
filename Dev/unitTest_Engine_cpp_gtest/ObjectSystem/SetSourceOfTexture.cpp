#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_SetSourceOfTexture : public EngineTest
{
public:
	ObjectSystem_SetSourceOfTexture(bool isOpenGLMode)
		: EngineTest(asd::ToAString("SetSourceOfTexture"), isOpenGLMode, 30)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto obj = make_shared<TextureObject2D>();
		obj->SetPosition(Vector2DF(0, 0));
		obj->SetTexture(Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Sample1.png").c_str()));
		obj->SetSrc(RectF(50, 50, 300, 300));

		Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(obj);
	}
};

ENGINE_TEST(ObjectSystem, SetSourceOfTexture)

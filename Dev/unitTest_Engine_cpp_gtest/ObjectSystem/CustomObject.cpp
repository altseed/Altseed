#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_CustomObject : public EngineTest
{
	class CustomObject : public TextureObject2D
	{
	protected:
		void OnStart()
		{
			SetTexture(
				ace::Engine::GetGraphics()->CreateTexture2D(
				ToAString("Data/Texture/Cloud1.png").c_str()));
		}

		void OnUpdate()
		{
			SetPosition(GetPosition() + Vector2DF(4, 3));
			SetAngle(GetAngle() + 1);
			SetScale(GetScale() - Vector2DF(0.001f, 0.001f));
		}
	};

public:
	ObjectSystem_CustomObject(bool isOpenGLMode)
		: EngineTest(ace::ToAString("CustomObject"), isOpenGLMode, 50)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<CustomObject>();
		ace::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(object);
	}
};

ENGINE_TEST(ObjectSystem, CustomObject)
#include <gtest/gtest.h>
#include <ace.h>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_ObjectsFlag : public EngineTest
{
	class MyObject : public TextureObject2D
	{
	public:
		MyObject(float x, float y)
		{
			auto texture = ace::Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str());
			SetTexture(texture);
			SetPosition(Vector2DF(x, y));
			SetCenterPosition(Vector2DF(128, 128));
		}

	protected:
		void OnUpdate()
		{
			SetAngle(GetAngle() + 2);
		}
	};

public:
	ObjectSystem_ObjectsFlag(bool isOpenGLMode)
		: EngineTest(ace::ToAString("ObjectsFlag"), isOpenGLMode, 30)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<ObjectSystem_ObjectsFlag::MyObject>(100, 128);
		auto isntDrawn = make_shared<ObjectSystem_ObjectsFlag::MyObject>(300, 128);
		auto isntUpdated = make_shared<ObjectSystem_ObjectsFlag::MyObject>(500, 128);

		isntDrawn->SetIsDrawn(false);
		isntUpdated->SetIsUpdated(false);

		layer->AddObject(object);
		layer->AddObject(isntDrawn);
		layer->AddObject(isntUpdated);

		scene->AddLayer(layer);
		ace::Engine::ChangeScene(scene);
	}
};

ENGINE_TEST(ObjectSystem, ObjectsFlag)

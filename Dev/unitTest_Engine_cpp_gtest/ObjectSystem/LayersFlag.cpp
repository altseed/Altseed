#include <gtest/gtest.h>
#include <ace.h>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_LayersFlag : public EngineTest
{
	std::shared_ptr<Scene>	scene;
	std::shared_ptr<Layer2D> layer;

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
	ObjectSystem_LayersFlag(bool isOpenGLMode)
		: EngineTest(ace::ToAString("LayersFlag"), isOpenGLMode, 30)
	{
	}

protected:
	void OnStart()
	{
		scene = make_shared<Scene>();
		layer = make_shared<Layer2D>();
		auto notDrawnLayer = make_shared<Layer2D>();
		auto notUpdatedLauer = make_shared<Layer2D>();
		auto object1 = make_shared<ObjectSystem_LayersFlag::MyObject>(100, 128);
		auto object2 = make_shared<ObjectSystem_LayersFlag::MyObject>(300, 128);
		auto object3 = make_shared<ObjectSystem_LayersFlag::MyObject>(500, 128);

		notDrawnLayer->SetIsDrawn(false);
		notUpdatedLauer->SetIsUpdated(false);

		layer->AddObject(object1);
		notDrawnLayer->AddObject(object2);
		notUpdatedLauer->AddObject(object3);
		scene->AddLayer(layer);
		scene->AddLayer(notDrawnLayer);
		scene->AddLayer(notUpdatedLauer);
		ace::Engine::ChangeScene(scene);
	}

	void OnUpdating()
	{
		// レイヤー削除テスト
		if (GetTime() == GetExitTime())
		{
			scene->RemoveLayer(layer);
		}
	}
};

ENGINE_TEST(ObjectSystem, LayersFlag)
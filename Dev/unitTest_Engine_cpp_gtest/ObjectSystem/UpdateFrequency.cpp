#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_UpdateFrequency : public EngineTest
{
	class MovingObject : public TextureObject2D
	{
	protected:
		void OnUpdate()
		{
			SetPosition(GetPosition() + Vector2DF(3, 0));
		}
	};

public:
	ObjectSystem_UpdateFrequency(bool isOpenGLMode)
		: EngineTest(asd::ToAString("UpdateFrequency"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		Engine::ChangeScene(scene);

		AddHastedLayerTo(scene, 2, Vector2DF(10, 10));
		AddHastedLayerTo(scene, 1, Vector2DF(10, 110));
		AddHastedLayerTo(scene, 0.2f, Vector2DF(10, 210));
		AddHastedLayerTo(scene, 0, Vector2DF(10, 310));
	}

private:
	void AddHastedLayerTo(Scene::Ptr scene, float frequency, Vector2DF position)
	{
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<MovingObject>();

		layer->SetUpdateFrequency(frequency);
		object->SetTexture(asd::Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str()));
		object->SetScale(Vector2DF(0.5f, 0.5f));
		object->SetPosition(position);

		scene->AddLayer(layer);
		layer->AddObject(object);
	}
};

ENGINE_TEST(ObjectSystem, UpdateFrequency)
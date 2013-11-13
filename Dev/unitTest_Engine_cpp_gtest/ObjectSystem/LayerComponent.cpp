#include <gtest/gtest.h>
#include <ace.h>

using namespace std;
using namespace ace;

class MyComponent : public Layer2DComponent
{
private:
	int time;

public:
	MyComponent()
		: time(0)
	{
	}

protected:
	void OnUpdate()
	{
		if (time % 60 == 0)
		{
			//*
			auto object = make_shared<TextureObject2D>();
			object->SetPosition(Vector2DF(time % 640, time % 480));
			GetOwner()->AddObject(object);
			//*/
		}
	}
};

void ObjectSystem_LayerComponent(bool isOpenGLMode)
{
	int time = 0;
	ace::EngineOption option;
	option.GraphicsType = isOpenGLMode ? ace::GRAPHICS_TYPE_GL : ace::GRAPHICS_TYPE_DX11;

	auto engine = ace::GetEngine();
	engine->Initialize(ace::ToAString("LayerComponent").c_str(), 640, 480, option);

	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		engine->ChangeScene(scene);
		scene->AddLayer(layer);

		layer->AddComponent(make_shared<MyComponent>(), ToAString("generator").c_str());
		ASSERT_NE(layer->GetComponent( ToAString("generator").c_str() ), nullptr);

		while (engine->DoEvents())
		{
			engine->Update();
			++time;
			if (time == 120)
			{
				break;
			}
		}
	}

	engine->Terminate();
}

/*
TEST(ObjectSystem, LayerComponent_GL)
{
	ObjectSystem_LayerComponent(true);
}

#if _WIN32
TEST(ObjectSystem, LayerComponent_DX)
{
	ObjectSystem_LayerComponent(false);
}
#endif

//*/
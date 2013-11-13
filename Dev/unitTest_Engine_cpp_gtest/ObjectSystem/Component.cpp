#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#pragma comment(lib,"Debug/ace_core.lib")
#pragma comment(lib,"Debug/ace_engine.lib")

using namespace std;
using namespace ace;

class MyComponent : public ace::Object2DComponent
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
		if (time % 30 == 0)
		{
			GetOwner()->SetAngle(GetOwner()->GetAngle() + 13);
		}
		++time;
	}
};

void ObjectSystem_Component(bool isOpenGLMode)
{
	int time = 0;
	ace::EngineOption option;
	option.GraphicsType = isOpenGLMode ? ace::GRAPHICS_TYPE_GL : ace::GRAPHICS_TYPE_DX11;

	auto engine = ace::GetEngine();
	engine->Initialize(ace::ToAString("Component").c_str(), 640, 480, option);

	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextureObject2D>();
		engine->ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(object);

		object->SetPosition(Vector2DF(320, 240));
		object->AddComponent(make_shared<MyComponent>(), ToAString("rotation"));
		ASSERT_NE(object->GetComponent(ToAString("rotation").c_str()), nullptr);

		auto g = ace::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str());
		object->SetTexture(g);

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

TEST(ObjectSystem, Component_GL)
{
	ObjectSystem_Component(true);
}

#if _WIN32
TEST(ObjectSystem, Component_DX)
{
	ObjectSystem_Component(false);
}
#endif


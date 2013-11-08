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
	Object2D* m_owner;
	int time;

public:
	MyComponent(Object2D& owner)
		: time(0)
		, m_owner(&owner)
	{
	}

protected:
	void OnUpdate()
	{
		if (time % 30 == 0)
		{
			m_owner->SetAngle(m_owner->GetAngle() + 30);
		}
		++time;
	}
};

void ObjectSystem_Component(bool isOpenGLMode)
{
	ace::EngineOption option;
	option.GraphicsType = isOpenGLMode ? ace::GRAPHICS_TYPE_GL : ace::GRAPHICS_TYPE_DX11;

	auto engine = ace::GetEngine();
	engine->Initialize(ace::ToAString("CustomObject2D").c_str(), 640, 480, option);

	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextureObject2D>();
		engine->ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(object);

		object->SetPosition(Vector2DF(320, 240));
		object->AddComponent(make_shared<MyComponent>(*object), ToAString("rotation"));

		auto g = ace::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str());
		object->SetTexture(g);

		while (engine->DoEvents())
		{
			engine->Update();
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


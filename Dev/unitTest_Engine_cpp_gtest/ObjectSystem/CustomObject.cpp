#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#pragma comment(lib,"Debug/ace_core.lib")
#pragma comment(lib,"Debug/ace_engine.lib")
using namespace std;
using namespace ace;

class CustomObject : public TextureObject2D
{
protected:
	void OnStart()
	{
		SetTexture(
			GetGraphics()->CreateTexture2D(
				ToAString("Data/Texture/Cloud1.png").c_str()));
	}

	void OnUpdate()
	{
		SetPosition(GetPosition() + Vector2DF(0.4f, 0.3f));
		SetAngle(GetAngle() + 1);
		SetScale(GetScale() - Vector2DF(0.001f, 0.001f));
	}
};

void ObjectSystem_Custom(bool isOpenGLMode)
{
	ace::EngineOption option;
	option.GraphicsType = isOpenGLMode ? ace::GRAPHICS_TYPE_GL : ace::GRAPHICS_TYPE_DX11;

	auto engine = ace::GetEngine();
	engine->Initialize(ace::ToAString("CustomObject2D").c_str(), 640, 480, option);

	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<CustomObject>();
		engine->ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(object);

		while (engine->DoEvents())
		{
			engine->Update();
		}
	}

	engine->Terminate();
}

TEST(ObjectSystem, Custom_GL)
{
	ObjectSystem_Custom(true);
}


TEST(ObjectSystem, Custom_DX)
{
	ObjectSystem_Custom(false);
}
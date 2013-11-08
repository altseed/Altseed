#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#pragma comment(lib,"Debug/ace_core.lib")
#pragma comment(lib,"Debug/ace_engine.lib")

using namespace std;
using namespace ace;

void ObjectSystem_Normal(bool isOpenGLMode)
{
	ace::EngineOption option;
	option.GraphicsType = isOpenGLMode ? ace::GRAPHICS_TYPE_GL : ace::GRAPHICS_TYPE_DX11;

	{
		auto engine = ace::GetEngine();
		engine->Initialize(ace::ToAString("Object2D").c_str(), 640, 480, option);

		{
			auto scene = make_shared<Scene>();
			auto layer = make_shared<Layer2D>();
			auto object = make_shared<TextureObject2D>();
			auto object2 = make_shared<TextureObject2D>();
			scene->AddLayer(layer);
			layer->AddObject(object);
			engine->ChangeScene(scene);
			layer->AddObject(object2);

			auto g = ace::GetGraphics();
			auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Cloud1.png").c_str());
			object->SetTexture(texture);
			object2->SetTexture(texture);
			
			object->SetCenterPosition(Vector2DF(128, 128));
			object->SetAngle(5);
			object->SetPosition(Vector2DF(320, 240));
			object->SetScale(object->GetScale() + Vector2DF(0.5f, 0.5f));

			while (engine->DoEvents())
			{
				engine->Update();
			}
		}

		engine->Terminate();
	}

	auto ref = GetGlobalReferenceCount();
	ASSERT_TRUE(ref == 0);
}

TEST(ObjectSystem, Normal_GL)
{
	ObjectSystem_Normal(true);
}

#if _WIN32
TEST(ObjectSystem, Normal_DX)
{
	ObjectSystem_Normal(false);
}
#endif


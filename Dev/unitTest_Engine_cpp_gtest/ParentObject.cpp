#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#pragma comment(lib,"Debug/ace_core.lib")
#pragma comment(lib,"Debug/ace_engine.lib")

using namespace std;
using namespace ace;

void ObjectSystem_Parent()
{
	auto engine = ace::GetEngine();
	engine->Initialize(ace::ToAString("ParentObject2D").c_str(), 640, 480, false);

	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto parent = make_shared<TextureObject2D>();
		auto child = make_shared<TextureObject2D>();

		engine->ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(parent);
		layer->AddObject(child);
		parent->AddChild(child, eChildMode::CHILD_MODE_ALL);

		auto g = ace::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Cloud1.png").c_str());
		parent->SetTexture(texture);
		child->SetTexture(texture);

		parent->SetPosition(Vector2DF(320, 240));
		parent->SetCenterPosition(Vector2DF(128,128));
		parent->SetColor(Color(255,255,128,255));
		child->SetPosition(Vector2DF(50, 50));

		while (engine->DoEvents())
		{
			parent->SetAngle(parent->GetAngle() + 0.2f);
			parent->SetScale(parent->GetScale() + Vector2DF(0.001f, 0.001f));
			engine->Update();
		}
	}

	engine->Terminate();
}

TEST(ObjectSystem, Parent)
{
	ObjectSystem_Parent();
}
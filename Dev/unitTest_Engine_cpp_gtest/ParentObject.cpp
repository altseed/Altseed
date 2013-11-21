#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_ParentObject : public EngineTest
{
public:
	ObjectSystem_ParentObject(bool isOpenGLMode)
		: EngineTest(ace::ToAString("ParentObject"), isOpenGLMode, 120)
		, m_parent(nullptr)
	{
	}

protected:
	shared_ptr<TextureObject2D> m_parent;

	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto child = make_shared<TextureObject2D>();
		m_parent = make_shared<TextureObject2D>();

		GetEngine()->ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(m_parent);
		layer->AddObject(child);
		m_parent->AddChild(child, eChildMode::CHILD_MODE_ALL);

		auto g = ace::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Cloud1.png").c_str());
		m_parent->SetTexture(texture);
		child->SetTexture(texture);

		m_parent->SetPosition(Vector2DF(320, 240));
		m_parent->SetCenterPosition(Vector2DF(128, 128));
		m_parent->SetColor(Color(255, 255, 128, 255));
		child->SetPosition(Vector2DF(50, 50));
	}

	void OnUpdating()
	{
		m_parent->SetAngle(m_parent->GetAngle() + 0.2f);
		m_parent->SetScale(m_parent->GetScale() + Vector2DF(0.001f, 0.001f));
	}
};

void Test_ParentObject()
{
	RunTest<ObjectSystem_ParentObject>(true);
	RunTest<ObjectSystem_ParentObject>(false);
}

TEST(ObjectSystem, ParentObject_GL)
{
	RunTest<ObjectSystem_ParentObject>(true);
}

#if _WIN32
TEST(ObjectSystem, ParentObject_DX)
{
	RunTest<ObjectSystem_ParentObject>(false);
}
#endif
#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class BugFix_DisposeAndChangeParent : public EngineTest
{
public:
	BugFix_DisposeAndChangeParent(bool isOpenGLMode)
		: EngineTest(asd::ToAString("DisposeAndChangeParent"), isOpenGLMode, 120)
	{
	}

protected:
	TextureObject2D::Ptr child, parent1, parent2, parent3;

	void OnStart()
	{
		auto texture = GetTexture();

		child = make_shared<TextureObject2D>();
		child->SetTexture(texture);
		child->SetPosition(Vector2DF(300, 0));
		child->SetScale(Vector2DF(0.4f, 0.4f));

		parent1 = make_shared<TextureObject2D>();
		parent1->SetTexture(texture);
		parent1->SetPosition(Vector2DF(0, 0));
		parent1->SetScale(Vector2DF(0.4f, 0.4f));
		parent1->SetColor(Color(255, 0, 0, 255));

		parent2 = make_shared<TextureObject2D>();
		parent2->SetTexture(texture);
		parent2->SetPosition(Vector2DF(0, 128));
		parent2->SetScale(Vector2DF(0.4f, 0.4f));
		parent2->SetColor(Color(0, 255, 0, 255));

		parent3 = make_shared<TextureObject2D>();
		parent3->SetTexture(texture);
		parent3->SetPosition(Vector2DF(0, 256));
		parent3->SetScale(Vector2DF(0.4f, 0.4f));
		parent3->SetColor(Color(0, 0, 255, 255));

		parent1->AddChild(child, ChildManagementMode::Nothing, ChildTransformingMode::Position);

		Engine::AddObject2D(child);
		Engine::AddObject2D(parent1);
		Engine::AddObject2D(parent2);
		Engine::AddObject2D(parent3);
	}

	void OnUpdating()
	{
		if (m_currentTime == 30)
		{
			parent1->Dispose();
			parent2->AddChild(child, ChildManagementMode::Nothing, ChildTransformingMode::Position);
		}
		if (m_currentTime == 60)
		{
			if (child->GetParent() != nullptr)
			{
				child->GetParent()->RemoveChild(child);
			}
			parent3->AddChild(child, ChildManagementMode::Nothing, ChildTransformingMode::Position);
		}
		if (m_currentTime == 90)
		{
			if (child->GetParent() != nullptr)
			{
				child->GetParent()->RemoveChild(child);
			}
			parent2->AddChild(child, ChildManagementMode::Nothing, ChildTransformingMode::Position);
		}
	}

	void OnUpdated()
	{
	}

	void OnFinish()
	{
	}
};

ENGINE_TEST(BugFix, DisposeAndChangeParent)
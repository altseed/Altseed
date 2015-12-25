#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class FamilySystem_DrawnParent : public EngineTest
{
public:
	FamilySystem_DrawnParent(bool isOpenGLMode)
		: EngineTest(asd::ToAString("DrawnParent"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto other = make_shared<TextureObject2D>();
		auto parent = make_shared<TextureObject2D>();
		auto child1 = make_shared<TextureObject2D>();
		auto child2 = make_shared<TextureObject2D>();

		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Cloud1.png").c_str());
		other->SetTexture(texture);
		parent->SetTexture(texture);
		child1->SetTexture(texture);
		child2->SetTexture(texture);

		other->SetColor(Color(255, 128, 128, 255));
		parent->SetColor(Color(128, 255, 128, 255));
		child1->SetColor(Color(0, 255, 255, 255));
		child2->SetColor(Color(255, 255, 0, 255));

		other->SetPosition(Vector2DF(0, 0));
		parent->SetPosition(Vector2DF(100, 0));
		child1->SetPosition(Vector2DF(0, 100));
		child2->SetPosition(Vector2DF(-100, 100));

		other->SetDrawingPriority(2);
		parent->SetDrawingPriority(3);

		parent->AddDrawnChild(child1,
			ChildManagementMode::Nothing,
			ChildTransformingMode::Position,
			ChildDrawingMode::Color);
		parent->AddDrawnChild(child2,
			ChildManagementMode::Nothing,
			ChildTransformingMode::Position,
			ChildDrawingMode::Color | ChildDrawingMode::DrawingPriority);

		Engine::AddObject2D(other);
		Engine::AddObject2D(parent);
		Engine::AddObject2D(child1);
		Engine::AddObject2D(child2);
	}

	void OnUpdating()
	{
	}

	void OnUpdated()
	{
	}

	void OnFinish()
	{
	}
};

ENGINE_TEST(FamilySystem, DrawnParent)
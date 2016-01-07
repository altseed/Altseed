#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class FamilySystem_AbsoluteParameter : public EngineTest
{
public:
	FamilySystem_AbsoluteParameter(bool isOpenGLMode)
		: EngineTest(asd::ToAString("AbsoluteParameter"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto shape = make_shared<CircleShape>();
		shape->SetOuterDiameter(100);

		auto parent = make_shared<TextureObject2D>();
		parent->SetPosition(Vector2DF(40, 40));
		parent->SetColor(Color(128, 255, 255, 255));
		parent->SetDrawingPriority(2);
		parent->SetIsUpdated(false);
		parent->SetIsDrawn(false);

		auto child = make_shared<TextureObject2D>();
		child->SetPosition(Vector2DF(100, 0));
		child->SetColor(Color(255, 255, 128, 255));
		child->SetDrawingPriority(1);
		child->SetIsUpdated(true);
		child->SetIsDrawn(true);

		parent->AddDrawnChild(child,
			ChildManagementMode::IsDrawn | ChildManagementMode::IsUpdated,
			ChildTransformingMode::All,
			ChildDrawingMode::Color | ChildDrawingMode::DrawingPriority);

		Engine::AddObject2D(parent);
		Engine::AddObject2D(child);

		ASSERT_TRUE(child->GetGlobalPosition() == Vector2DF(140, 40));
		ASSERT_EQ(child->GetAbsoluteBeingUpdated(), false);
		ASSERT_EQ(child->GetAbsoluteBeingDrawn(), false);
		ASSERT_TRUE(child->GetAbsoluteColor() == Color(128, 255, 128, 255));
		ASSERT_EQ(child->GetAbsoluteDrawingPriority(), 3);
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

ENGINE_TEST(FamilySystem, AbsoluteParameter)
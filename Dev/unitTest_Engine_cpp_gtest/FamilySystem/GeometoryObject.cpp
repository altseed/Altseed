#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class FamilySystem_GeometoryObject : public EngineTest
{
public:
	FamilySystem_GeometoryObject(bool isOpenGLMode)
		: EngineTest(asd::ToAString("GeometoryObject"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto shape = make_shared<CircleShape>();
		shape->SetOuterDiameter(100);

		auto parent = make_shared<GeometryObject2D>();
		parent->SetShape(shape);
		parent->SetPosition(Vector2DF(70, 70));
		parent->SetColor(Color(255, 128, 128, 255));
		parent->SetDrawingPriority(2);

		auto child1 = make_shared<GeometryObject2D>();
		child1->SetShape(shape);
		child1->SetPosition(Vector2DF(0, 50));
		child1->SetColor(Color(128, 128, 255, 255));
		child1->SetDrawingPriority(1);

		auto child2 = make_shared<GeometryObject2D>();
		child2->SetShape(shape);
		child2->SetPosition(Vector2DF(50, 0));
		child2->SetColor(Color(128, 128, 255, 255));
		child2->SetDrawingPriority(1);

		parent->AddDrawnChild(child1,
			ChildManagementMode::Nothing,
			ChildTransformingMode::All,
			ChildDrawingMode::Color | ChildDrawingMode::DrawingPriority);

		parent->AddDrawnChild(child2,
			ChildManagementMode::Nothing,
			ChildTransformingMode::All,
			ChildDrawingMode::Nothing);

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

ENGINE_TEST(FamilySystem, GeometoryObject)
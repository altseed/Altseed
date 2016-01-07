#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class FamilySystem_TextObject : public EngineTest
{
public:
	FamilySystem_TextObject(bool isOpenGLMode)
		: EngineTest(asd::ToAString("TextObject"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto font = Engine::GetGraphics()->CreateFont(ToAString("Data/Font/MeiryoTest.aff").c_str());

		auto parent = make_shared<TextObject2D>();
		parent->SetFont(font);
		parent->SetText(ToAString("Meiryo\nMeiryo").c_str());
		parent->SetPosition(Vector2DF(20, 20));
		parent->SetColor(Color(255, 128, 128, 255));
		parent->SetDrawingPriority(2);

		auto child1 = make_shared<TextObject2D>();
		child1->SetFont(font);
		child1->SetText(ToAString("Meiryo\nMeiryo").c_str());
		child1->SetPosition(Vector2DF(0, 20));
		child1->SetColor(Color(128, 128, 255, 255));
		child1->SetDrawingPriority(1);

		auto child2 = make_shared<TextObject2D>();
		child2->SetFont(font);
		child2->SetText(ToAString("Meiryo\nMeiryo").c_str());
		child2->SetPosition(Vector2DF(20, 0));
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

ENGINE_TEST(FamilySystem, TextObject)
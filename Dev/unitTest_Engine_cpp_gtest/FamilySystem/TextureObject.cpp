#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class FamilySystem_TextureObject : public EngineTest
{
public:
	FamilySystem_TextureObject(bool isOpenGLMode)
		: EngineTest(asd::ToAString("TextureObject"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto texture = Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str());

		auto parent = make_shared<TextureObject2D>();
		parent->SetTexture(texture);
		parent->SetPosition(Vector2DF(20, 20));
		parent->SetColor(Color(255, 128, 128, 255));
		parent->SetDrawingPriority(2);

		auto child1 = make_shared<TextureObject2D>();
		child1->SetTexture(texture);
		child1->SetPosition(Vector2DF(0, 100));
		child1->SetColor(Color(128, 128, 255, 255));
		child1->SetDrawingPriority(1);

		auto child2 = make_shared<TextureObject2D>();
		child2->SetTexture(texture);
		child2->SetPosition(Vector2DF(100, 0));
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

ENGINE_TEST(FamilySystem, TextureObject)
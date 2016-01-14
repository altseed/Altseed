#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class FamilySystem_VanishParent : public EngineTest
{
public:
	FamilySystem_VanishParent(bool isOpenGLMode)
		: EngineTest(asd::ToAString("VanishParent"), isOpenGLMode, 60)
	{
	}

private:
	TextureObject2D::Ptr child;
	TextureObject2D::Ptr parent;
	int count = 0;

protected:
	void OnStart()
	{
		parent = make_shared<TextureObject2D>();
		child = make_shared<TextureObject2D>();

		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Cloud1.png").c_str());
		parent->SetTexture(texture);
		child->SetTexture(texture);

		parent->SetPosition(Vector2DF(100, 100));
		child->SetPosition(Vector2DF(30, 30));

		parent->AddChild(child, ChildManagementMode::Nothing, ChildTransformingMode::Position);

		Engine::AddObject2D(parent);
		Engine::AddObject2D(child);
	}

	void OnUpdating()
	{
		++count;
		if (count >= 30)
		{
			parent->Dispose();
		}
	}

	void OnUpdated()
	{
	}

	void OnFinish()
	{
	}
};

ENGINE_TEST(FamilySystem, VanishParent)
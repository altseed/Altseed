#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_TransformOutOfUpdate : public EngineTest
{
public:
	ObjectSystem_TransformOutOfUpdate(bool isOpenGLMode)
		: EngineTest(asd::ToAString("TransformOutOfUpdate"), isOpenGLMode, 60)
	{
	}

private:
	shared_ptr<TextureObject2D> obj;
	shared_ptr<TextureObject2D> child;
	int loop = 0;

protected:
	void OnStart()
	{
		obj = make_shared<TextureObject2D>();
		obj->SetPosition(Vector2DF(200, 200));
		obj->SetTexture(Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str()));

		Engine::AddObject2D(obj);

		child = make_shared<TextureObject2D>();
		child->SetPosition(Vector2DF(10, 10));
		child->SetTexture(Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str()));
		
		obj->AddChild(child, ChildManagementMode::Nothing, ChildTransformingMode::Position);
		Engine::AddObject2D(child);
	}

	void OnUpdating()
	{
		obj->SetPosition(obj->GetPosition() + Vector2DF(1, 0));

		loop++;

		if (loop < 9)
		{
			child->SetPosition(obj->GetPosition() + Vector2DF(1, 0));
		}
		if (loop == 10)
		{
			child->Dispose();
		}
	}
};

ENGINE_TEST(ObjectSystem, TransformOutOfUpdate)
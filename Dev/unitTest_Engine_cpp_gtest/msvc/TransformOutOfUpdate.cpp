#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_TransformOutOfUpdate : public EngineTest
{
public:
	ObjectSystem_TransformOutOfUpdate(bool isOpenGLMode)
		: EngineTest(ace::ToAString("TransformOutOfUpdate"), isOpenGLMode, 60)
	{
	}

private:
	shared_ptr<TextureObject2D> obj;

protected:
	void OnStart()
	{
		obj = make_shared<TextureObject2D>();
		obj->SetPosition(Vector2DF(200, 200));
		obj->SetTexture(Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str()));

		Engine::AddObject2D(obj);
	}

	void OnUpdating()
	{
		obj->SetPosition(obj->GetPosition() + Vector2DF(1, 0));
	}
};

ENGINE_TEST(ObjectSystem, TransformOutOfUpdate)
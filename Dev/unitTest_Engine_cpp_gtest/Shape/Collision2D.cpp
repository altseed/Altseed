#include <gtest/gtest.h>
#include <ace.h>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class Shape_Collision2D : public EngineTest
{
public:
	Shape_Collision2D(bool isOpenGLMode)
		: EngineTest(ace::ToAString("Collision2D"), isOpenGLMode, 60)
	{
	}

private:
	std::shared_ptr<CircleShape> cir1;
	std::shared_ptr<CircleShape> cir2;
protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();

		cir1 = make_shared<CircleShape>();
		cir1->SetOuterDiameter(100);
		cir1->SetInnerDiameter(0);
		cir1->SetPosition(Vector2DF(100, 200));


		cir2 = make_shared<CircleShape>();
		cir2->SetOuterDiameter(100);
		cir2->SetInnerDiameter(0);
		cir2->SetPosition(Vector2DF(150, 200));

		scene->AddLayer(layer);
		ace::Engine::ChangeScene(scene);
	}

	void OnUpdating()
	{
		printf("%d", cir1->GetIsCollidedWith(cir2) ? 1 : 0);
	}
};

ENGINE_TEST(Shape, Collision2D)

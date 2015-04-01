
#include <gtest/gtest.h>
#include <ace.h>
#include "../../EngineTest.h"

using namespace std;
using namespace ace;

class Graphics_GeometryObject2D : public EngineTest
{
public:
	Graphics_GeometryObject2D(bool isOpenGLMode)
		: EngineTest(ace::ToAString("GeometryObject2D"), isOpenGLMode, 20)
	{
	}

private:

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto texture = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		auto geometryObj1 = make_shared<GeometryObject2D>();

		layer->AddObject(geometryObj1);
		scene->AddLayer(layer);
		ace::Engine::ChangeScene(scene);

		auto circle = make_shared<Circle>();
		circle->SetOuterDiamater(400);
		circle->SetInnerDiamater(0);
		circle->SetNumberOfCorners(96);
		circle->SetPosition(Vector2DF(320, 240));

		geometryObj1->SetShape(circle);
		geometryObj1->SetTexture(texture);
		geometryObj1->SetPosition(ace::Vector2DF(0, 0));
	}

	void OnUpdating()
	{
	}
};


ENGINE_TEST(Graphics, GeometryObject2D)

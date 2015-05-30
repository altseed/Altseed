#include <gtest/gtest.h>
#include <ace.h>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class Shape_Collision2D : public EngineTest
{
public:
	Shape_Collision2D(bool isOpenGLMode)
		: EngineTest(ace::ToAString("Collision2D"), isOpenGLMode, 300)
	{
	}

private:
	std::shared_ptr<GeometryObject2D> geometryObj0;
	std::shared_ptr<GeometryObject2D> geometryObj1;
	std::shared_ptr<GeometryObject2D> geometryObj2;
	std::shared_ptr<GeometryObject2D> geometryObj3;

	std::shared_ptr<CircleShape> selfCircle;

	std::shared_ptr<CircleShape> circle;
	std::shared_ptr<TriangleShape> triangle;
	std::shared_ptr<RectangleShape> rect;
protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		geometryObj0 = make_shared<GeometryObject2D>();
		geometryObj1 = make_shared<GeometryObject2D>();
		geometryObj2 = make_shared<GeometryObject2D>();
		geometryObj3 = make_shared<GeometryObject2D>();

		layer->AddObject(geometryObj0);
		layer->AddObject(geometryObj1);
		layer->AddObject(geometryObj2);
		layer->AddObject(geometryObj3);

		scene->AddLayer(layer);
		ace::Engine::ChangeScene(scene);

		{
			selfCircle = make_shared<CircleShape>();
			selfCircle->SetOuterDiameter(100);
			selfCircle->SetInnerDiameter(0);
			selfCircle->SetNumberOfCorners(96);
			selfCircle->SetPosition(Vector2DF(100, 50));

			geometryObj0->SetShape(selfCircle);
			geometryObj0->SetPosition(ace::Vector2DF(0, 0));
			geometryObj0->SetDrawingPriority(1);
		}

		{
			circle = make_shared<CircleShape>();
			circle->SetOuterDiameter(100);
			circle->SetInnerDiameter(0);
			circle->SetNumberOfCorners(96);
			circle->SetPosition(Vector2DF(100, 50));

			geometryObj1->SetShape(circle);
			geometryObj1->SetPosition(ace::Vector2DF(0, 0));
		}

		{
			triangle = make_shared<TriangleShape>();
			triangle->SetPointByIndex(ace::Vector2DF(400, 350), 0);
			triangle->SetPointByIndex(ace::Vector2DF(300, 450), 1);
			triangle->SetPointByIndex(ace::Vector2DF(540, 450), 2);

			geometryObj2->SetShape(triangle);
			geometryObj2->SetPosition(ace::Vector2DF(0, 0));
		}

		{
			rect = make_shared<RectangleShape>();
			rect->SetDrawingArea(ace::RectF());
			rect->SetDrawingArea(ace::RectF(10, 110, 300, 200));
			rect->SetAngle(15);

			geometryObj3->SetShape(rect);
			geometryObj3->SetPosition(ace::Vector2DF(0, 0));
		}
	}

	void OnUpdating()
	{
		selfCircle->SetPosition(ace::Engine::GetMouse()->GetPosition());

		triangle->SetPointByIndex(triangle->GetPointByIndex(0) + Vector2DF(1, 0), 0);

		if (selfCircle->GetIsCollidedWith(circle)
			|| selfCircle->GetIsCollidedWith(triangle)
			|| selfCircle->GetIsCollidedWith(rect))
		{
			geometryObj0->SetColor(ace::Color(255, 0, 0, 255));
		}
		else
		{
			geometryObj0->SetColor(ace::Color(255, 255, 255, 255));
		}

		rect->SetAngle(rect->GetAngle() + 1);
	}
};

ENGINE_TEST(Shape, Collision2D)

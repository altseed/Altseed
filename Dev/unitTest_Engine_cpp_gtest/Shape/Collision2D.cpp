#include "../EngineTest.h"
#include <Altseed.h>
#include <gtest/gtest.h>

using namespace std;
using namespace asd;

class Shape_Collision2D : public EngineTest
{
public:
	Shape_Collision2D(bool isOpenGLMode)
		: EngineTest(asd::ToAString("Collision2D"), isOpenGLMode, 300)
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
		asd::Engine::ChangeScene(scene);

		{
			selfCircle = make_shared<CircleShape>();
			selfCircle->SetOuterDiameter(100);
			selfCircle->SetInnerDiameter(0);
			selfCircle->SetNumberOfCorners(96);
			selfCircle->SetPosition(Vector2DF(100, 50));

			geometryObj0->SetShape(selfCircle);
			geometryObj0->SetPosition(asd::Vector2DF(0, 0));
			geometryObj0->SetDrawingPriority(2);
		}

		{
			circle = make_shared<CircleShape>();
			circle->SetOuterDiameter(100);
			circle->SetInnerDiameter(0);
			circle->SetNumberOfCorners(96);
			circle->SetPosition(Vector2DF(100, 50));

			geometryObj1->SetShape(circle);
			geometryObj1->SetPosition(asd::Vector2DF(0, 0));
			geometryObj1->SetDrawingPriority(1);
		}

		{
			triangle = make_shared<TriangleShape>();
			triangle->SetPointByIndex(asd::Vector2DF(400, 350), 0);
			triangle->SetPointByIndex(asd::Vector2DF(300, 450), 1);
			triangle->SetPointByIndex(asd::Vector2DF(540, 450), 2);

			geometryObj2->SetShape(triangle);
			geometryObj2->SetPosition(asd::Vector2DF(0, 0));
		}

		{
			rect = make_shared<RectangleShape>();
			rect->SetDrawingArea(asd::RectF());
			rect->SetDrawingArea(asd::RectF(10, 110, 300, 200));
			rect->SetAngle(15);

			geometryObj3->SetShape(rect);
			geometryObj3->SetPosition(asd::Vector2DF(0, 0));
		}
	}

	void OnUpdating()
	{
		selfCircle->SetPosition(asd::Engine::GetMouse()->GetPosition());

		triangle->SetPointByIndex(
			triangle->GetPointByIndex(0) + Vector2DF(1, 0), 0);

		if (selfCircle->GetIsCollidedWith(circle) ||
			selfCircle->GetIsCollidedWith(triangle) ||
			selfCircle->GetIsCollidedWith(rect))
		{
			geometryObj0->SetColor(asd::Color(255, 0, 0, 255));
		}
		else
		{
			geometryObj0->SetColor(asd::Color(255, 255, 255, 255));
		}

		if (circle->GetIsCollidedWith(selfCircle) ||
			circle->GetIsCollidedWith(triangle) ||
			circle->GetIsCollidedWith(rect))
		{
			geometryObj1->SetColor(asd::Color(255, 127, 0, 255));
		}
		else
		{
			geometryObj1->SetColor(asd::Color(255, 255, 255, 255));
		}

		rect->SetAngle(rect->GetAngle() + 1);
	}
};

ENGINE_TEST(Shape, Collision2D)

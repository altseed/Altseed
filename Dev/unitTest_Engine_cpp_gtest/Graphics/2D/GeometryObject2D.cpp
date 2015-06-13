
#include <gtest/gtest.h>
#include <Altseed.h>
#include "../../EngineTest.h"

using namespace std;
using namespace asd;

class Graphics_GeometryObject2D : public EngineTest
{
public:
	Graphics_GeometryObject2D(bool isOpenGLMode)
		: EngineTest(asd::ToAString("GeometryObject2D"), isOpenGLMode, 20)
	{
	}

private:

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());
		auto geometryObj1 = make_shared<GeometryObject2D>();
		auto geometryObj2 = make_shared<GeometryObject2D>();
		auto geometryObj3 = make_shared<GeometryObject2D>();
		auto geometryObj4 = make_shared<GeometryObject2D>();
		auto geometryObj5 = make_shared<GeometryObject2D>();
		auto geometryObj6 = make_shared<GeometryObject2D>();

		layer->AddObject(geometryObj1);
		layer->AddObject(geometryObj2);
		layer->AddObject(geometryObj3);
		layer->AddObject(geometryObj4);
		layer->AddObject(geometryObj5);
		layer->AddObject(geometryObj6);

		scene->AddLayer(layer);
		asd::Engine::ChangeScene(scene);

		{
			auto circle = make_shared<CircleShape>();
			circle->SetOuterDiameter(100);
			circle->SetInnerDiameter(10);
			circle->SetNumberOfCorners(96);
			circle->SetPosition(Vector2DF(100, 50));

			geometryObj1->SetShape(circle);
			geometryObj1->SetTexture(texture);
			geometryObj1->SetPosition(asd::Vector2DF(0, 0));
		}

		{
			auto arc = make_shared<asd::ArcShape>();
			arc->SetOuterDiameter(100);
			arc->SetInnerDiameter(10);
			arc->SetNumberOfCorners(96);
			arc->SetPosition(Vector2DF(300, 50));
			arc->SetStartingCorner(90);
			arc->SetEndingCorner(5);

			geometryObj2->SetShape(arc);
			geometryObj2->SetTexture(texture);
			geometryObj2->SetPosition(asd::Vector2DF(0, 0));
		}

		{
			auto line = make_shared<LineShape>();
			line->SetStartingPosition(Vector2DF(410, 50));
			line->SetEndingPosition(Vector2DF(630, 50));
			line->SetThickness(5.0f);

			geometryObj3->SetShape(line);
			geometryObj3->SetPosition(asd::Vector2DF(0, 0));
		}

		{
			auto rect = make_shared<asd::RectangleShape>();
			rect->SetDrawingArea(asd::RectF(10, 110, 300, 200));
			rect->SetUV(asd::RectF(0, 0, 0.5, 0.5));

			geometryObj4->SetShape(rect);
			geometryObj4->SetTexture(texture);
			geometryObj4->SetPosition(asd::Vector2DF(0, 0));
		}

		{
			auto triangle = make_shared<asd::TriangleShape>();
			triangle->SetPointByIndex(asd::Vector2DF(320, 350), 0);
			triangle->SetPointByIndex(asd::Vector2DF(100, 450), 1);
			triangle->SetPointByIndex(asd::Vector2DF(540, 450), 2);

			triangle->SetUVByIndex(asd::Vector2DF(0.5, 0.2), 0);
			triangle->SetUVByIndex(asd::Vector2DF(0.1, 0.5), 1);
			triangle->SetUVByIndex(asd::Vector2DF(0.9, 0.7), 2);

			geometryObj5->SetShape(triangle);
			geometryObj5->SetTexture(texture);
			geometryObj5->SetPosition(asd::Vector2DF(0, 0));
		}

		{
			auto polygon = make_shared<asd::PolygonShape>();

			for (int i = 0; i < 10; ++i)
			{
				Vector2DF vec = Vector2DF(1, 0);
				vec.SetDegree(i * 36);
				vec.SetLength((i % 2 == 0) ? 100 : 55);
				polygon->AddVertex(vec + Vector2DF(500, 250));

			}

			geometryObj6->SetShape(polygon);
			geometryObj6->SetTexture(texture);
			geometryObj6->SetPosition(asd::Vector2DF(0, 0));
		}
	}

	void OnUpdating()
	{
	}
};


ENGINE_TEST(Graphics, GeometryObject2D)

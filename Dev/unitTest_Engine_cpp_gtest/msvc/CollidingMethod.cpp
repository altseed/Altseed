#include <gtest/gtest.h>
#include <Altseed.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class ObjectSystem_CollidingMethod : public EngineTest
{

	class CollidingObject1 : public TextureObject2D
	{
		Vector2DF velocity;
	protected:
		void OnAdded()
		{

			auto rectCollider = std::make_shared<RectangleCollider>();
			rectCollider->SetArea(RectF(0, 0, 100, 100));
			rectCollider->SetVisible(true);
			rectCollider->SetTag(asd::ToAString("A").c_str());
			AddCollider(rectCollider);

			auto lineCollider = std::make_shared<LineCollider>();
			lineCollider->SetStartingPosition(Vector2DF(110, 110));
			lineCollider->SetEndingPosition(Vector2DF(200, 200));
			lineCollider->SetThickness(10);
			lineCollider->SetTag(asd::ToAString("B").c_str());
			lineCollider->SetVisible(true);
			AddCollider(lineCollider);

			auto circleCollider = std::make_shared<CircleCollider>();
			circleCollider->SetCenter(Vector2DF(300, 300));
			circleCollider->SetRadius(90);
			circleCollider->SetTag(asd::ToAString("C").c_str());
			circleCollider->SetVisible(true);
			AddCollider(circleCollider);

			SetScale(asd::Vector2DF(0.5, 0.5));
			velocity = Vector2DF(4, 0);
		}

		void OnUpdate()
		{
			if (GetPosition().X < 0) {
				velocity = Vector2DF(4, 0);
			}
			else if (GetPosition().X > 600) {
				velocity = Vector2DF(-4, 0);
			}
			SetAngle(GetAngle() + 1);

			auto collisionsInfo = GetCollisionsInfo();

			for (auto collisionInfo : collisionsInfo) {
				switch (collisionInfo->GetCollisionType())
				{
				case CollisionType::Enter:
					printf("Enter:%s\n", asd::ToUtf8String(collisionInfo->GetSelfCollider()->GetTag()).c_str());
						break;
				case CollisionType::Stay:
					printf("Stay:%s\n", asd::ToUtf8String(collisionInfo->GetSelfCollider()->GetTag()).c_str());
					break;
				case CollisionType::Exit:
					printf("Exit:%s\n", asd::ToUtf8String(collisionInfo->GetSelfCollider()->GetTag()).c_str());
					break;
				default:
					break;
				}
			}

		}
	};

	class CollidingObject2 : public TextureObject2D
	{
		Vector2DF velocity;
	protected:
		void OnAdded()
		{
			auto rectCollider = std::make_shared<RectangleCollider>();
			rectCollider->SetArea(asd::RectF(0, 0, 100, 100));
			rectCollider->SetTag(asd::ToAString("D").c_str());
			rectCollider->SetVisible(true);
			AddCollider(rectCollider);
			SetScale(asd::Vector2DF(0.5, 0.5));
		}

		void OnUpdate()
		{
			SetPosition(Engine::GetMouse()->GetPosition());
		}
	};
public:
	ObjectSystem_CollidingMethod(bool isOpenGLMode)
		: EngineTest(asd::ToAString("CollidingMethod"), isOpenGLMode, 600)
	{
	}

protected:
	void OnStart()
	{
		auto obj1 = make_shared<CollidingObject1>();
		obj1->SetPosition(asd::Vector2DF(320, 240));
		auto obj2 = make_shared<CollidingObject2>();
		obj2->SetPosition(asd::Vector2DF(600, 20));

		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		asd::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(obj1);
		layer->AddObject(obj2);
	}
};

ENGINE_TEST(ObjectSystem, CollidingMethod)

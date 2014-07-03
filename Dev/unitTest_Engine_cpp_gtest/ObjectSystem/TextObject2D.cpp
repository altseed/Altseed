﻿#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../EngineTest.h"

using namespace std;
using namespace ace;

class ObjectSystem_TextObject2D : public EngineTest
{
public:
	ObjectSystem_TextObject2D(bool isOpenGLMode)
		: EngineTest(ace::ToAString("TextObject2D"), isOpenGLMode, 60)
	{
	}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto object = make_shared<TextObject2D>();

		ASSERT_NE(scene, nullptr);
		ASSERT_NE(layer, nullptr);
		ASSERT_NE(object, nullptr);

		ace::Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		layer->AddObject(object);

		auto g = ace::Engine::GetGraphics();
		
		auto font = g->CreateFont(ace::ToAString("Data/Text/Nac0703.aff").c_str());

		object->SetFont(font);
		object->SetText(ToAString("Font"));

		//object->SetCenterPosition(Vector2DF(128, 128));
		object->SetPosition(Vector2DF(0, 0));
	}
};


ENGINE_TEST(ObjectSystem, TextObject2D)
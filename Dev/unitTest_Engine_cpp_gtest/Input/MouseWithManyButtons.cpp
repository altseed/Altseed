#include "../EngineTest.h"
#include <Altseed.h>
#include <gtest/gtest.h>

using namespace std;
using namespace asd;

class Input_MouseWithManyButtons : public EngineTest
{
	vector<shared_ptr<TextObject2D>> textObjects;

public:
	Input_MouseWithManyButtons(bool isOpenGLMode) : EngineTest(u"MouseWithManyButtons", isOpenGLMode, 300) {}

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();

		for (int i = 0; i < 8; ++i)
		{
			auto text = make_shared<TextObject2D>();
			text->SetFont(
				Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 32, Color(255, 0, 0, 255), 0, Color(0, 0, 0, 0)));
			text->SetPosition(Vector2DF(40, 40 + 50 * i));
			textObjects.push_back(text);
			layer->AddObject(text);
		}

		scene->AddLayer(layer);
		asd::Engine::ChangeScene(scene);
	}

	void OnUpdating()
	{
		for (int i = 0; i < 8; ++i)
		{
			auto inputState = Engine::GetMouse()->GetButtonInputState((MouseButtons)i);

			if (inputState == ButtonState::Push)
			{
				textObjects[i]->SetText((ToAString("ボタン") + ToAString(to_string(i).c_str()) + ToAString("を押しました。")).c_str());
			}
			else if (inputState == ButtonState::Hold)
			{
				textObjects[i]->SetText(
					(ToAString("ボタン") + ToAString(to_string(i).c_str()) + ToAString("を押し続けています。")).c_str());
			}
			else if (inputState == ButtonState::Free)
			{
				textObjects[i]->SetText(
					(ToAString("ボタン") + ToAString(to_string(i).c_str()) + ToAString("を離し続けています。")).c_str());
			}
			else if (inputState == ButtonState::Release)
			{
				textObjects[i]->SetText((ToAString("ボタン") + ToAString(to_string(i).c_str()) + ToAString("を離しました。")).c_str());
			}
		}
	}
};

ENGINE_TEST(Input, MouseWithManyButtons)

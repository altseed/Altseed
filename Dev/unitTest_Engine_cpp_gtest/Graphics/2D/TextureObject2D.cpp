
#include <gtest/gtest.h>
#include <Altseed.h>
#include "../../EngineTest.h"

using namespace std;
using namespace asd;

class Graphics_TextureObject2D : public EngineTest
{
public:
	Graphics_TextureObject2D(bool isOpenGLMode)
		: EngineTest(asd::ToAString("TextureObject2D"), isOpenGLMode, 20)
	{
	}

private:

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());
		auto textureObj1 = make_shared<TextureObject2D>();
		auto textureObj2 = make_shared<TextureObject2D>();

		layer->AddObject(textureObj1);
		layer->AddObject(textureObj2);
		scene->AddLayer(layer);
		asd::Engine::ChangeScene(scene);

		textureObj1->SetTexture(texture);
		textureObj1->SetSrc(asd::RectF(256, 256, 256, 256));
		textureObj1->SetPosition(asd::Vector2DF(320, 240));

		textureObj2->SetTexture(texture);
		textureObj2->SetSrc(asd::RectF(0, 0, 256, 256));
		textureObj2->SetPosition(asd::Vector2DF(400, 320));
		textureObj2->SetTurnLR(true);
	}

	void OnUpdating()
	{
	}
};


ENGINE_TEST(Graphics, TextureObject2D)

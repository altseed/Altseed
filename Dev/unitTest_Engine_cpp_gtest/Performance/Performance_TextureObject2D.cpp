
#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class Performance_TextureObject2D : public EngineTest
{
public:
	Performance_TextureObject2D(bool isOpenGLMode)
		: EngineTest(asd::ToAString("TextureObject2D"), isOpenGLMode, 200000)
	{
	}

private:

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());

		for (int32_t y = 0; y < 50; y++)
		{
			for (int32_t x = 0; x < 50; x++)
			{
				auto textureObj = make_shared<TextureObject2D>();
				layer->AddObject(textureObj);

				textureObj->SetTexture(texture);
				textureObj->SetSrc(asd::RectF(256, 256, 256, 256));
				textureObj->SetPosition(asd::Vector2DF(x * 20,  y * 15));

				textureObj->SetScale(asd::Vector2DF(0.05f, 0.05f));
			}
		}

		scene->AddLayer(layer);
		asd::Engine::ChangeScene(scene);
	}

	void OnUpdating()
	{
	}
};

#if defined(PERFORMANCE_MODE)
ENGINE_TEST(Performance, TextureObject2D)
#endif

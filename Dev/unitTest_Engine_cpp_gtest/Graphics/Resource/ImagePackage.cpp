
#include <gtest/gtest.h>
#include <Altseed.h>
#include "../../EngineTest.h"

using namespace std;
using namespace asd;

class Graphics_ImagePackage : public EngineTest
{
public:
	Graphics_ImagePackage(bool isOpenGLMode)
		: EngineTest(asd::ToAString("ImagePackage"), isOpenGLMode, 10)
	{
	}

private:

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto imagePackage = asd::Engine::GetGraphics()->CreateImagePackage(asd::ToAString("Data/ImagePackage/test.aip").c_str());

		for (int32_t i = 0; i < imagePackage->GetImageCount(); i++)
		{
			auto obj = make_shared<TextureObject2D>();

			obj->SetTexture(imagePackage->GetImage(i));
			obj->SetPosition(asd::Vector2DF(imagePackage->GetImageArea(i).X, imagePackage->GetImageArea(i).Y));
			layer->AddObject(obj);
		}

		scene->AddLayer(layer);
		asd::Engine::ChangeScene(scene);
	}

	void OnUpdating()
	{
	}
};


ENGINE_TEST(Graphics, ImagePackage)

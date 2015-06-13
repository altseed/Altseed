#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"


class Graphics_PostEffectLightBloom : public EngineTest
{
public:
	Graphics_PostEffectLightBloom(bool isOpenGLMode) :
		EngineTest(asd::ToAString("PostEffectLightBloom"), isOpenGLMode, 10)
	{}
protected:

	void OnStart() override
	{
		auto scene = std::make_shared<asd::Scene>();
		auto layer = std::make_shared<asd::Layer2D>();
		auto obj= std::make_shared<asd::TextureObject2D>();
		scene->SetHDRMode(true);
		scene->AddLayer(layer);
		layer->AddObject(obj);
		asd::Engine::ChangeScene(scene);

		auto g = asd::Engine::GetGraphics();
		auto texture = g->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());
		obj->SetTexture(texture);
		obj->SetScale(asd::Vector2DF(1, 1));

		auto pe = std::make_shared<asd::PostEffectLightBloom>();
		pe->SetIntensity(5.0f);
		pe->SetThreshold(0.2f);
		pe->SetExposure(3.0f);
		layer->AddPostEffect(pe);
	}
};

ENGINE_TEST(Graphics, PostEffectLightBloom)
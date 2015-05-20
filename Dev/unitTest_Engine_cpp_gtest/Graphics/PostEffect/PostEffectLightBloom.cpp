#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"


class Graphics_PostEffectLightBloom : public EngineTest
{
public:
	Graphics_PostEffectLightBloom(bool isOpenGLMode) :
		EngineTest(ace::ToAString("PostEffectLightBloom"), isOpenGLMode, 10)
	{}
protected:

	void OnStart() override
	{
		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer2D>();
		auto obj= std::make_shared<ace::TextureObject2D>();
		scene->SetHDRMode(true);
		scene->AddLayer(layer);
		layer->AddObject(obj);
		ace::Engine::ChangeScene(scene);

		auto g = ace::Engine::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		obj->SetTexture(texture);
		obj->SetScale(ace::Vector2DF(1, 1));

		auto pe = std::make_shared<ace::PostEffectLightBloom>();
		pe->SetIntensity(5.0f);
		pe->SetThreshold(0.2f);
		pe->SetExposure(3.0f);
		layer->AddPostEffect(pe);
	}
};

ENGINE_TEST(Graphics, PostEffectLightBloom)
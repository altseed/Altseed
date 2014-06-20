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
	std::shared_ptr<ace::PostEffectLightBloom> pe;
	float intensity = 10.0f;
	void OnStart() override
	{
		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer2D>();
		auto object = std::make_shared<ace::TextureObject2D>();
		scene->SetHDRMode(true);
		scene->AddLayer(layer);
		layer->AddObject(object);
		ace::Engine::ChangeScene(scene);

		auto g = ace::Engine::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		object->SetTexture(texture);
		object->SetScale(ace::Vector2DF(1, 1));

		pe = std::make_shared<ace::PostEffectLightBloom>(g);
		pe->SetIntensity(intensity);
		layer->AddPostEffect(pe);

	}
	void OnUpdating() override
	{
		pe->SetThreshold(0.3f);
		pe->SetPower(2.0f);
		pe->SetIntensity(intensity);
		intensity += 0.1f;
	}
};

ENGINE_TEST(Graphics, PostEffectLightBloom)
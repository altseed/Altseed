#include <Altseed.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"


class Graphics_PostEffectGaussianBlur : public EngineTest
{
public:
	Graphics_PostEffectGaussianBlur(bool isOpenGLMode) :
		EngineTest(asd::ToAString("PostEffectGaussianBlur"), isOpenGLMode, 10)
	{}
protected:
	std::shared_ptr<asd::PostEffectGaussianBlur> pe;
	float intensity = 0.0f;
	void OnStart() override
	{
		auto scene = std::make_shared<asd::Scene>();
		auto layer = std::make_shared<asd::Layer2D>();
		auto obj = std::make_shared<asd::TextureObject2D>();
		scene->AddLayer(layer);
		layer->AddObject(obj);
		asd::Engine::ChangeScene(scene);

		auto g = asd::Engine::GetGraphics();
		auto texture = g->CreateTexture2D(asd::ToAString("Data/Texture/Sample1.png").c_str());
		obj->SetTexture(texture);
		obj->SetScale(asd::Vector2DF(1, 1));

		pe = std::make_shared<asd::PostEffectGaussianBlur>();
		pe->SetIntensity(intensity);
		layer->AddPostEffect(pe);

	}
	void OnUpdating() override
	{
		pe->SetIntensity(intensity);
		intensity += 0.5f;
	}
};

ENGINE_TEST(Graphics, PostEffectGaussianBlur)




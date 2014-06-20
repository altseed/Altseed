#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"


class Graphics_PostEffectGaussianBlur : public EngineTest
{
public:
	Graphics_PostEffectGaussianBlur(bool isOpenGLMode) :
		EngineTest(ace::ToAString("PostEffectGaussianBlur"), isOpenGLMode, 60)
	{}
protected:
	std::shared_ptr<ace::PostEffectGaussianBlur> pe;
	float intensity = 0.0f;
	void OnStart() override
	{
		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer2D>();
		auto object = std::make_shared<ace::TextureObject2D>();
		scene->AddLayer(layer);
		layer->AddObject(object);
		ace::Engine::ChangeScene(scene);

		auto g = ace::Engine::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		object->SetTexture(texture);
		object->SetScale(ace::Vector2DF(1, 1));

		pe = std::make_shared<ace::PostEffectGaussianBlur>(g);
		pe->SetIntensity(intensity);
		layer->AddPostEffect(pe);

	}
	void OnUpdating() override
	{
		pe->SetIntensity(intensity);
		intensity += 0.1f;
	}

	
};


ENGINE_TEST(Graphics, PostEffectGaussianBlur)




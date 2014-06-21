#include <ace.h>
#include <gtest/gtest.h>
#include <memory>
#include "../../EngineTest.h"


class Graphics_PostEffectSepia : public EngineTest
{
public:
	float intensity;
	Graphics_PostEffectSepia(bool isOpenGLMode) :
		EngineTest(ace::ToAString("PostEffectSepia"), isOpenGLMode, 10)
	{}
protected:
	void OnStart() override
	{
		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer2D>();
		auto obj = std::make_shared<ace::TextureObject2D>();
		scene->AddLayer(layer);
		layer->AddObject(obj);
		ace::Engine::ChangeScene(scene);

		auto g = ace::Engine::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		obj->SetTexture(texture);
		obj->SetScale(ace::Vector2DF(2, 2));

		auto pe = std::make_shared<ace::PostEffectSepia>();
		layer->AddPostEffect(pe);
	}
};


ENGINE_TEST(Graphics, PostEffectSepia)

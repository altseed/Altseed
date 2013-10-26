#include <ace.h>
#include <gtest/gtest.h>
#include <memory>


void Graphics_PostEffectGrayScale()
{
	auto engine = ace::GetEngine();
	engine->Initialize(ace::ToAString("PostEffectGrayScale").c_str(), 640, 480, false);

	{
		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer2D>();
		auto object = std::make_shared<ace::TextureObject2D>();
		scene->AddLayer(layer);
		layer->AddObject(object);
		engine->ChangeScene(scene);

		auto g = ace::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Cloud1.png").c_str());
		object->SetTexture(texture);
		object->SetScale(ace::Vector2DF(2, 2));

		auto pe = std::make_shared<ace::PostEffectGrayScale>(g);
		layer->AddPostEffect(pe);

		while (engine->DoEvents())
		{
			engine->Update();
		}
	}

	engine->Terminate();

	auto ref = ace::GetGlobalReferenceCount();
	ASSERT_TRUE(ref == 0);

}


TEST(Graphics, PostEffectGrayScale)
{
	Graphics_PostEffectGrayScale();
}
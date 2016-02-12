#include "SampleBrowser.h"
#include "SampleBrowserLayer.h"
#include <memory>
#include <Altseed.h>

using namespace std;
using namespace asd;

SampleBrowser::SampleBrowser(std::vector<SampleInfo>& samples)
	: m_samples(samples)
{
}

void SampleBrowser::Run()
{
	while (true)
	{
		SampleInfo selected = SampleInfo();
		selected.isAvailable = false;

		Engine::Initialize(ToAString("サンプルブラウザ").c_str(), 640, 480, EngineOption());

		auto scene = make_shared<Scene>();
		auto layer = make_shared<SampleBrowserLayer>(m_samples);
		layer->SetOnDecideEventHandler([&selected](SampleInfo s){ selected = s; });

		Engine::ChangeScene(scene);
		scene->AddLayer(layer);

		auto hintLayer = make_shared<Layer2D>();
		auto hintObject = make_shared<TextureObject2D>();
		hintObject->SetTexture(Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Browser/Hint.png").c_str()));
		hintLayer->AddObject(hintObject);

		scene->AddLayer(hintLayer);

		while (Engine::DoEvents() && !selected.isAvailable)
		{
			Engine::Update();
		}

		Engine::Terminate();

		if (!selected.isAvailable)
		{
			break;
		}
		selected.func();
	}
}
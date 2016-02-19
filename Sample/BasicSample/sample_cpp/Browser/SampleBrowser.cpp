#include "SampleBrowser.h"
#include "SampleBrowserLayer.h"
#include "SampleInfoLayer.h"
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

		auto size = (480 - 85) * (480 - 85) / layer->GetTotalHeight();
		auto infoLayer = make_shared<SampleInfoLayer>(size, layer->GetTotalHeight());
		infoLayer->SetDrawingPriority(2);
		layer->SetOnSelectionChangedEventHandler([&infoLayer](SampleInfo s){ infoLayer->Show(s); });

		Engine::ChangeScene(scene);
		scene->AddLayer(layer);
		scene->AddLayer(infoLayer);

		auto hintLayer = make_shared<Layer2D>();
		auto hintObject = make_shared<TextureObject2D>();
		hintObject->SetTexture(Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Browser/Hint.png").c_str()));
		hintLayer->AddObject(hintObject);

		scene->AddLayer(hintLayer);

		while (Engine::DoEvents() && !selected.isAvailable)
		{
			Engine::Update();
			infoLayer->MoveScrollBar(layer->GetCameraArea().Y);
		}

		Engine::Terminate();

		if (!selected.isAvailable)
		{
			break;
		}
		selected.func();
	}
}
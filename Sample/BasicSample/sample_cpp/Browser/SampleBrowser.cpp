#include "SampleBrowser.h"
#include "SampleBrowserLayer.h"
#include "SampleInfoLayer.h"
#include <memory>
#include <Altseed.h>

using namespace std;

SampleBrowser::SampleBrowser(std::vector<SampleInfo>& samples)
	: m_samples(samples)
{
}

void SampleBrowser::Run()
{
	while (true)
	{
		Selected.isAvailable = false;
		
		{

			asd::Engine::Initialize(u"サンプルブラウザ", 640, 480, asd::EngineOption());

			auto scene = make_shared<asd::Scene>();
			auto browserLayer = make_shared<SampleBrowserLayer>(this, m_samples);

			auto size = (480 - 85) * (480 - 85) / browserLayer->GetTotalHeight();
			auto infoLayer = make_shared<SampleInfoLayer>(size, browserLayer->GetTotalHeight());
			infoLayer->SetDrawingPriority(2);

			asd::Engine::ChangeScene(scene);
			scene->AddLayer(browserLayer);
			scene->AddLayer(infoLayer);

			auto hintLayer = make_shared<asd::Layer2D>();
			auto hintObject = make_shared<asd::TextureObject2D>();
			hintObject->SetTexture(asd::Engine::GetGraphics()->CreateTexture2D(u"Data/Browser/Hint.png"));
			hintLayer->AddObject(hintObject);

			scene->AddLayer(hintLayer);

			while (asd::Engine::DoEvents() && !Selected.isAvailable)
			{
				asd::Engine::Update();
				infoLayer->MoveScrollBar(browserLayer->GetCameraArea().Y);
			}

			asd::Engine::Terminate();
		}

		if (!Selected.isAvailable)
		{
			break;
		}
		Selected.func();
		Selected.isAvailable = false;
	}

}

void SampleBrowser::ShowInfo(SampleInfo sample)
{
	auto infoLayer = infoLayer_weak.lock();
	if (infoLayer != nullptr)
	{
		infoLayer->Show(sample);
	}
}
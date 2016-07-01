#pragma once
#include <vector>
#include "SampleGuide.h"
#include "SampleBrowserLayer.h"
#include "SampleInfoLayer.h"

class SampleBrowser
{
private:
	std::vector<SampleInfo> m_samples;
	std::shared_ptr<SampleBrowserLayer> browserLayer;
	std::shared_ptr<SampleInfoLayer> infoLayer;

public:
	static const int Margin = 16;

	SampleBrowser(std::vector<SampleInfo>& samples);

	SampleInfo	Selected;
	void ShowInfo(SampleInfo sample);
	void Run();
};
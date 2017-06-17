#pragma once
#include <vector>
#include "SampleGuide.h"
#include "SampleBrowserLayer.h"
#include "SampleInfoLayer.h"

class SampleBrowser
{
private:
	std::vector<SampleInfo> m_samples;
	std::weak_ptr<SampleBrowserLayer> browserLayer_weak;
	std::weak_ptr<SampleInfoLayer> infoLayer_weak;

public:
	static const int Margin = 16;

	SampleBrowser(std::vector<SampleInfo>& samples);

	SampleInfo	Selected;
	void ShowInfo(SampleInfo sample);
	void Run();
};
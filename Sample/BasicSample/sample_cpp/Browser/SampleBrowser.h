#pragma once
#include <vector>
#include "SampleGuide.h"

class SampleBrowser
{
private:
	std::vector<SampleInfo> m_samples;

public:
	static const int Margin = 16;

	SampleBrowser(std::vector<SampleInfo>& samples);

	void Run();
};
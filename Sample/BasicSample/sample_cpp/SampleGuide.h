#pragma once
#include <functional>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <Altseed.h>

struct SampleInfo
{
	std::function<void()> func;
	asd::astring title;
	asd::astring readableTitle;
	asd::astring description;
	bool isAvailable;
};

class SampleGuide
{
public:
	SampleGuide(std::vector<SampleInfo> samples)
		: m_samples(samples)
	{
		m_exitIndex = (int)samples.size();
	}

	void Show();

private:
	std::vector<SampleInfo> m_samples;
	int m_exitIndex;

	void ShowTitles();
	int InputSampleNumber();
	bool EnsureToRun( int number );
};
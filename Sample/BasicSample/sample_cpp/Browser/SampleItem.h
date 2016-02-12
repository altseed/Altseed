#pragma once
#include <Altseed.h>
#include <memory>
#include "SampleGuide.h"

class SampleItem : public asd::TextureObject2D
{
private:
	asd::RectangleShape m_shape;
	asd::TextureObject2D::Ptr m_frame;
	asd::TextObject2D::Ptr m_title;
	SampleInfo m_sample;

	asd::astring GetWrappedString(std::shared_ptr<asd::Font>& font, asd::astring title);

public:
	static const int SizeX = 150;
	static const int SizeY = 112;

	SampleItem(SampleInfo sample, std::shared_ptr<asd::Font>& font);

	asd::RectangleShape GetArea();
	SampleInfo GetSample() const;

	void Activate();
	void Disactivate();
};
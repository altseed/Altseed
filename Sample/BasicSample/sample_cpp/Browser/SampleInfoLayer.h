#pragma once
#include <Altseed.h>
#include "../SampleGuide.h"

class SampleInfoLayer : public asd::Layer2D
{
private:
	static const int PanelHeight = 65;
	static const int ScrollBarWidth = 16;

	asd::TextObject2D::Ptr title, className, description;
	asd::GeometryObject2D::Ptr scrollBar;
	float totalHeight;

public:
	SampleInfoLayer(float scrollBarHeight, float totalHeight);

	void Show(SampleInfo info);

	void MoveScrollBar(float pos);
};
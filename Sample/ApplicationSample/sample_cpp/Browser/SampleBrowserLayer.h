#include <vector>
#pragma once
#include "SampleGuide.h"
#include <Altseed.h>
#include <functional>
#include "SampleItem.h"

class SampleBrowser;

class SampleBrowserLayer : public asd::Layer2D
{
private:
	std::vector<std::shared_ptr<SampleItem>> m_items;
	asd::CameraObject2D::Ptr m_camera;
	asd::RectF m_cameraArea;

	SampleBrowser*	browser = nullptr;

	float m_totalHeight;
	std::shared_ptr<SampleItem> m_activeItem;

protected:
	void OnUpdated() override;

public:
	const static int Columns = 3;
	const static int ItemOffsetX = 632 / Columns;
	const static int ItemOffsetY = 150;

	SampleBrowserLayer(SampleBrowser* browser, std::vector<SampleInfo>& samples);

	asd::RectF& GetCameraArea() const;
	float GetTotalHeight() const;
};
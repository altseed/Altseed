#include "SampleBrowserLayer.h"
#include "SampleItem.h"

using namespace std;
using namespace asd;

SampleBrowserLayer::SampleBrowserLayer(std::vector<SampleInfo>& samples)
	: m_items(std::vector<shared_ptr<SampleItem>>())
{
	SetName(ToAString("BrowserLayer"));

	auto font = Engine::GetGraphics()->CreateDynamicFont(
		ToAString("").c_str(),
		12, Color(255, 255, 255, 255),
		1, Color(0, 0, 0, 255));
	int index = 0;
	for (auto& sample : samples)
	{
		auto item = make_shared<SampleItem>(sample, font);
		auto x = ItemOffsetX * (index % Columns) + 8;
		auto y = ItemOffsetY * (index / Columns) + 8 + 20;
		item->SetPosition(Vector2DF(x, y));
		AddObject(item);
		m_items.push_back(item);
		++index;
	}

	int haveJut = index % Columns == 0 ? 0 : 1;
	m_totalHeight = ItemOffsetY * (index / Columns + haveJut) + 8 + 20;

	m_camera = make_shared<CameraObject2D>();
	m_camera->SetSrc(RectI(0, 0, 640, 480));
	m_camera->SetDst(RectI(0, 0, 640, 480));
	AddObject(m_camera);
}

void SampleBrowserLayer::OnUpdated()
{
	auto rows = (m_items.size() / Columns) + (m_items.size() % Columns == 0 ? 0 : 1);
	auto y = m_camera->GetSrc().Y - Engine::GetMouse()->GetMiddleButton()->GetRotation() * 30;
	y = max(0, y);
	y = min(rows * ItemOffsetY - 480 + 24 + 60, y);
	m_camera->SetSrc(RectI(m_camera->GetSrc().X, (int)y, m_camera->GetSrc().Width, m_camera->GetSrc().Height));

	auto mouse = make_shared<CircleShape>();
	mouse->SetPosition(Engine::GetMouse()->GetPosition() + Vector2DF(0, m_camera->GetSrc().Y));
	mouse->SetOuterDiameter(2);

	if (m_activeItem != nullptr && !m_activeItem->GetArea().GetIsCollidedWith(mouse))
	{
		m_activeItem->Disactivate();
		m_activeItem = nullptr;

		auto disabled = SampleInfo();
		disabled.isAvailable = false;
		if (m_onSelectionChanged != nullptr)
		{
			m_onSelectionChanged(disabled);
		}
	}

	for(auto& item : m_items)
	{
		if (item->GetArea().GetIsCollidedWith(mouse))
		{
			if (item != m_activeItem)
			{
				item->Activate();
				m_activeItem = item;
				if (m_onSelectionChanged != nullptr)
				{
					m_onSelectionChanged(item->GetSample());
				}
			}
			if (m_onDecide != nullptr && Engine::GetMouse()->GetLeftButton()->GetButtonState() == MouseButtonState::Push)
			{
				m_onDecide(item->GetSample());
			}
			break;
		}
	}
}

RectF& SampleBrowserLayer::GetCameraArea() const
{
	return m_camera->GetSrc().ToF();
}

float SampleBrowserLayer::GetTotalHeight() const
{
	return m_totalHeight;
}

void SampleBrowserLayer::SetOnSelectionChangedEventHandler(std::function<void(SampleInfo)> eventHandler)
{
	m_onSelectionChanged = eventHandler;
}

void SampleBrowserLayer::SetOnDecideEventHandler(std::function<void(SampleInfo)> eventHandler)
{
	m_onDecide = eventHandler;
}
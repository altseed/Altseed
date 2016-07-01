#include "SampleInfoLayer.h"
#include "SampleBrowser.h"

using namespace std;

SampleInfoLayer::SampleInfoLayer(float scrollBarHeight, float totalHeight)
	: totalHeight(totalHeight)
{
	scrollBar = make_shared<asd::GeometryObject2D>();
	{
		auto rect = asd::RectF(0, 0, ScrollBarWidth, scrollBarHeight);
		auto shape = make_shared<asd::RectangleShape>();
		shape->SetDrawingArea(rect);
		scrollBar->SetShape(shape);
	}
	scrollBar->SetColor(asd::Color(64, 64, 64, 255));
	scrollBar->SetPosition(asd::Vector2DF(640 - ScrollBarWidth - SampleBrowser::Margin, 20 + SampleBrowser::Margin));
	AddObject(scrollBar);

	SetName(asd::ToAString("Info"));

	auto panel = make_shared<asd::GeometryObject2D>();
	{
		auto rect = asd::RectF(0, 0, 640, PanelHeight);
		auto shape = make_shared<asd::RectangleShape>();
		shape->SetDrawingArea(rect);
		panel->SetShape(shape);
	}
	panel->SetColor(asd::Color(16, 16, 16, 255));
	panel->SetPosition(asd::Vector2DF(0, 480 - PanelHeight));
	AddObject(panel);

	auto font = asd::Engine::GetGraphics()->CreateDynamicFont(asd::ToAString("").c_str(), 12, asd::Color(255, 255, 255, 255), 1, asd::Color(0, 0, 0, 255));

	title = make_shared<asd::TextObject2D>();
	title->SetFont(font);
	title->SetText(asd::ToAString("").c_str());
	title->SetColor(asd::Color(255, 255, 0, 255));
	title->SetPosition(asd::Vector2DF(2, 2));
	title->SetDrawingPriority(1);

	className = make_shared<asd::TextObject2D>();
	className->SetFont(font);
	className->SetText(asd::ToAString("").c_str());
	className->SetColor(asd::Color(128, 255, 225, 255));
	className->SetPosition(asd::Vector2DF(2, 2));
	className->SetDrawingPriority(1);

	description = make_shared<asd::TextObject2D>();
	description->SetFont(font);
	description->SetText(asd::ToAString("").c_str());
	description->SetColor(asd::Color(255, 255, 255, 255));
	description->SetPosition(asd::Vector2DF(6, 22));
	description->SetDrawingPriority(1);

	panel->AddChild(title, asd::ChildManagementMode::RegistrationToLayer, asd::ChildTransformingMode::Position);
	panel->AddChild(className, asd::ChildManagementMode::RegistrationToLayer, asd::ChildTransformingMode::Position);
	panel->AddChild(description, asd::ChildManagementMode::RegistrationToLayer, asd::ChildTransformingMode::Position);
}

void SampleInfoLayer::Show(SampleInfo info)
{
	if (info.isAvailable)
	{
		title->SetText(info.readableTitle.c_str());
		className->SetText(info.title.c_str());
		description->SetText(info.description.c_str());
		className->SetPosition(asd::Vector2DF(
			title->GetFont()->CalcTextureSize(title->GetText(), asd::WritingDirection::Horizontal).X + 8,
			2));
	}
	else
	{
		title->SetText(asd::ToAString("").c_str());
		className->SetText(asd::ToAString("").c_str());
		description->SetText(asd::ToAString("").c_str());
	}
}

void SampleInfoLayer::MoveScrollBar(float pos)
{
	float yOffset = pos / totalHeight * (480 - 20 - PanelHeight - SampleBrowser::Margin);
	scrollBar->SetPosition(asd::Vector2DF(
		640 - ScrollBarWidth - SampleBrowser::Margin,
		20 + SampleBrowser::Margin / 2 + yOffset));
}
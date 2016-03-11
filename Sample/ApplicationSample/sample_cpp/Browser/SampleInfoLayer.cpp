#include "SampleInfoLayer.h"
#include "SampleBrowser.h"

using namespace std;
using namespace asd;

SampleInfoLayer::SampleInfoLayer(float scrollBarHeight, float totalHeight)
	: totalHeight(totalHeight)
{
	scrollBar = make_shared<GeometryObject2D>();
	{
		auto rect = RectF(0, 0, ScrollBarWidth, scrollBarHeight);
		auto shape = make_shared<RectangleShape>();
		shape->SetDrawingArea(rect);
		scrollBar->SetShape(shape);
	}
	scrollBar->SetColor(Color(64, 64, 64, 255));
	scrollBar->SetPosition(Vector2DF(640 - ScrollBarWidth - SampleBrowser::Margin, 20 + SampleBrowser::Margin));
	AddObject(scrollBar);

	SetName(ToAString("Info"));

	auto panel = make_shared<GeometryObject2D>();
	{
		auto rect = RectF(0, 0, 640, PanelHeight);
		auto shape = make_shared<RectangleShape>();
		shape->SetDrawingArea(rect);
		panel->SetShape(shape);
	}
	panel->SetColor(Color(16, 16, 16, 255));
	panel->SetPosition(Vector2DF(0, 480 - PanelHeight));
	AddObject(panel);

	auto font = Engine::GetGraphics()->CreateDynamicFont(ToAString("").c_str(), 12, Color(255, 255, 255, 255), 1, Color(0, 0, 0, 255));

	title = make_shared<TextObject2D>();
	title->SetFont(font);
	title->SetText(ToAString("").c_str());
	title->SetColor(Color(255, 255, 0, 255));
	title->SetPosition(Vector2DF(2, 2));
	title->SetDrawingPriority(1);

	className = make_shared<TextObject2D>();
	className->SetFont(font);
	className->SetText(ToAString("").c_str());
	className->SetColor(Color(128, 255, 225, 255));
	className->SetPosition(Vector2DF(2, 2));
	className->SetDrawingPriority(1);

	description = make_shared<TextObject2D>();
	description->SetFont(font);
	description->SetText(ToAString("").c_str());
	description->SetColor(Color(255, 255, 255, 255));
	description->SetPosition(Vector2DF(6, 22));
	description->SetDrawingPriority(1);

	panel->AddChild(title, ChildManagementMode::RegistrationToLayer, ChildTransformingMode::Position);
	panel->AddChild(className, ChildManagementMode::RegistrationToLayer, ChildTransformingMode::Position);
	panel->AddChild(description, ChildManagementMode::RegistrationToLayer, ChildTransformingMode::Position);
}

void SampleInfoLayer::Show(SampleInfo info)
{
	if (info.isAvailable)
	{
		title->SetText(info.readableTitle.c_str());
		className->SetText(info.title.c_str());
		description->SetText(info.description.c_str());
		className->SetPosition(Vector2DF(
			title->GetFont()->CalcTextureSize(title->GetText(), WritingDirection::Horizontal).X + 8,
			2));
	}
	else
	{
		title->SetText(ToAString("").c_str());
		className->SetText(ToAString("").c_str());
		description->SetText(ToAString("").c_str());
	}
}

void SampleInfoLayer::MoveScrollBar(float pos)
{
	float yOffset = pos / totalHeight * (480 - 20 - PanelHeight - SampleBrowser::Margin);
	scrollBar->SetPosition(Vector2DF(
		640 - ScrollBarWidth - SampleBrowser::Margin,
		20 + SampleBrowser::Margin / 2 + yOffset));
}
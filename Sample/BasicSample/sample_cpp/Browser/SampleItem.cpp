#include "SampleItem.h"

using namespace std;
using namespace asd;

SampleItem::SampleItem(SampleInfo sample, std::shared_ptr<asd::Font>& font)
	: m_sample(sample)
{
	auto texturePath = asd::ToAString("Data/Browser/") + sample.title + asd::ToAString(".png");
	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(texturePath.c_str());
	if (texture == nullptr)
	{
		texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Browser/Default.png").c_str());
	}
	SetTexture(texture);
	SetScale(Vector2DF(SizeX, SizeY) / texture->GetSize().To2DF());

	asd::astring text;
	if (!sample.readableTitle.empty())
	{
		text = sample.readableTitle;
	}
	else
	{
		text = sample.title;
	}

	m_title = make_shared<TextObject2D>();
	m_title->SetFont(font);
	m_title->SetText(GetWrappedString(font, text).c_str());
	m_title->SetPosition(Vector2DF(0, 115));
	m_title->SetDrawingPriority(1);

	AddChild(m_title,
		ChildManagementMode::RegistrationToLayer | ChildManagementMode::Disposal,
		ChildTransformingMode::Position);

	auto frameTexture = Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Browser/Frame.png").c_str());
	m_frame = make_shared<TextureObject2D>();
	m_frame->SetTexture(frameTexture);
	m_frame->SetPosition(Vector2DF(-3, -3));

	AddChild(m_frame,
		ChildManagementMode::RegistrationToLayer | ChildManagementMode::Disposal,
		ChildTransformingMode::Position);
}

RectangleShape SampleItem::GetArea()
{
	auto result = RectangleShape();
	result.SetDrawingArea(RectF(GetPosition(), Vector2DF(SizeX, SizeY)));
	return result;
}

SampleInfo SampleItem::GetSample() const
{
	return m_sample;
}

void SampleItem::Activate()
{
	auto texture = Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Browser/FrameActive.png").c_str());
	m_frame->SetTexture(texture);
}

void SampleItem::Disactivate()
{
	auto texture = Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Browser/Frame.png").c_str());
	m_frame->SetTexture(texture);
}

astring SampleItem::GetWrappedString(shared_ptr<Font>& font, astring title)
{
	astring result = title;
	if (font->CalcTextureSize(result.c_str(), WritingDirection::Horizontal).X <= SizeX)
	{
		return result;
	}
	while (font->CalcTextureSize(result.c_str(), WritingDirection::Horizontal).X > SizeX)
	{
		result = result.substr(0, result.length() - 1);
	}
	return (result + ToAString("Åc")).c_str();
}
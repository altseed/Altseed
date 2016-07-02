#include "SampleItem.h"

using namespace std;

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
	SetScale(asd::Vector2DF(SizeX, SizeY) / texture->GetSize().To2DF());

	asd::astring text;
	if (!sample.readableTitle.empty())
	{
		text = sample.readableTitle;
	}
	else
	{
		text = sample.title;
	}

	m_title = make_shared<asd::TextObject2D>();
	m_title->SetFont(font);
	m_title->SetText(GetWrappedString(font, text).c_str());
	m_title->SetPosition(asd::Vector2DF(0, 115));
	m_title->SetDrawingPriority(1);

	auto frameTexture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Browser/Frame.png").c_str());
	m_frame = make_shared<TextureObject2D>();
	m_frame->SetTexture(frameTexture);
	m_frame->SetPosition(asd::Vector2DF(-3, -3));
}

void SampleItem::OnAdded()
{
	AddChild(m_title,
		asd::ChildManagementMode::RegistrationToLayer | asd::ChildManagementMode::Disposal,
		asd::ChildTransformingMode::Position);

	AddChild(m_frame,
		asd::ChildManagementMode::RegistrationToLayer | asd::ChildManagementMode::Disposal,
		asd::ChildTransformingMode::Position);
}

asd::RectangleShape SampleItem::GetArea()
{
	auto result = asd::RectangleShape();
	result.SetDrawingArea(asd::RectF(GetPosition(), asd::Vector2DF(SizeX, SizeY)));
	return result;
}

SampleInfo SampleItem::GetSample() const
{
	return m_sample;
}

void SampleItem::Activate()
{
	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Browser/FrameActive.png").c_str());
	m_frame->SetTexture(texture);
}

void SampleItem::Disactivate()
{
	auto texture = asd::Engine::GetGraphics()->CreateTexture2D(asd::ToAString("Data/Browser/Frame.png").c_str());
	m_frame->SetTexture(texture);
}

asd::astring SampleItem::GetWrappedString(shared_ptr<asd::Font>& font, asd::astring title)
{
	asd::astring result = title;
	if (font->CalcTextureSize(result.c_str(), asd::WritingDirection::Horizontal).X <= SizeX)
	{
		return result;
	}
	while (font->CalcTextureSize(result.c_str(), asd::WritingDirection::Horizontal).X > SizeX)
	{
		result = result.substr(0, result.length() - 1);
	}
	return (result + asd::ToAString("Åc")).c_str();
}
#include "ace.TextObject2D.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	TextObject2D::TextObject2D()
		: m_coreObject(nullptr)
		, m_font(nullptr)
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateTextObject2D());
	}

	TextObject2D::~TextObject2D()
	{
	}

	CoreObject2D* TextObject2D::GetCoreObject() const
	{
		return m_coreObject.get();
	}

	std::shared_ptr<Font> TextObject2D::GetFont() const
	{
		return m_font;
	}

	void TextObject2D::SetFont(std::shared_ptr<Font> font)
	{
		m_coreObject->SetFont(font.get());
		m_font = font;
	}

	Vector2DF TextObject2D::GetCenterPosition() const
	{
		return m_coreObject->GetCenterPosition();
	}

	void TextObject2D::SetCenterPosition(Vector2DF position)
	{
		m_coreObject->SetCenterPosition(position);
	}

	Color TextObject2D::GetColor() const
	{
		return m_coreObject->GetColor();
	}

	void TextObject2D::SetColor(Color color)
	{
		m_coreObject->SetColor(color);
	}

	bool TextObject2D::GetTurnLR() const
	{
		return m_coreObject->GetTurnLR();
	}

	void TextObject2D::SetTurnLR(bool turnLR)
	{
		m_coreObject->SetTurnLR(turnLR);
	}

	bool TextObject2D::GetTurnUL() const
	{
		return m_coreObject->GetTurnUL();
	}

	void TextObject2D::SetTurnUL(bool turnUL)
	{
		m_coreObject->SetTurnUL(turnUL);
	}

	int TextObject2D::GetDrawingPriority() const
	{
		return m_coreObject->GetDrawingPriority();
	}

	void TextObject2D::SetDrawingPriority(int priority)
	{
		m_coreObject->SetDrawingPriority(priority);
	}

	AlphaBlendMode TextObject2D::GetAlphaBlendMode() const
	{
		return m_coreObject->GetAlphaBlendMode();
	}

	void TextObject2D::SetAlphaBlendMode(AlphaBlendMode alphaBlend)
	{
		m_coreObject->SetAlphaBlendMode(alphaBlend);
	}

	WritingDirection TextObject2D::GetWritingDirection() const
	{
		return m_coreObject->GetWritingDirection();
	}

	void TextObject2D::SetWritingDirection(WritingDirection writingDirection)
	{
		m_coreObject->SetWritingDirection(writingDirection);
	}

	const achar* TextObject2D::GetText()
	{
		return m_coreObject->GetText();
	}

	void TextObject2D::SetText(const achar* text)
	{
		m_coreObject->SetText(text);
	}

	void TextObject2D::SetTextureFilterType(TextureFilterType textureFilterType)
	{
		m_coreObject->SetTextureFilterType(textureFilterType);
	}

	TextureFilterType TextObject2D::GetTextureFilterType() const
	{
		return m_coreObject->GetTextureFilterType();
	}
}
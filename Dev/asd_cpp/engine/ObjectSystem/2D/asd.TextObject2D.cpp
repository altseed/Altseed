#include "asd.TextObject2D.h"

namespace asd
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

	CoreDrawnObject2D* TextObject2D::GetCoreDrawnObject() const
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

	void TextObject2D::SetLetterSpacing(float letterSpacing)
	{
		m_coreObject->SetLetterSpacing(letterSpacing);
	}

	float TextObject2D::GetLetterSpacing() const
	{
		return m_coreObject->GetLetterSpacing();
	}

	void TextObject2D::SetLineSpacing(float lineSpacing)
	{
		m_coreObject->SetLineSpacing(lineSpacing);
	}

	float TextObject2D::GetLineSpacing() const
	{
		return m_coreObject->GetLineSpacing();
	}

	bool TextObject2D::GetIsRichTextMode() const
	{
		return m_coreObject->GetIsRichTextMode();
	}

	void TextObject2D::SetIsRichTextMode(bool value)
	{
		m_coreObject->SetIsRichTextMode(value);
	}

	std::shared_ptr<Material2D> TextObject2D::GetMaterial()
	{
		auto mat = m_coreObject->GetMaterial();
		SafeAddRef(mat);
		return CreateSharedPtrWithReleaseDLL(mat);
	}

	void TextObject2D::SetMaterial(const std::shared_ptr<Material2D>& material)
	{
		m_coreObject->SetMaterial(material.get());
	}
}
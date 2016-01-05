
#include "../common/asd.common.Base.h"
#include "asd.CoreTextObject2D_Imp.h"
#include "../../Graphics/Resource/asd.Font_Imp.h"
#include "../../Graphics/Resource/asd.Texture2D_Imp.h"

#ifdef WIN32
#ifdef min
#undef min
#endif 
#ifdef max
#undef max
#endif
#endif

namespace asd
{
	CoreTextObject2D_Imp::CoreTextObject2D_Imp(Graphics_Imp* graphics)
		: CoreDrawnObject2D_Imp(graphics)
		, m_font(nullptr)
		, m_centerPosition(Vector2DF())
		, m_turnLR(false)
		, m_turnUL(false)
		, m_text(asd::ToAString(""))
		, m_alphablend(AlphaBlendMode::Blend)
		, m_writingDirection(WritingDirection::Horizontal)
		, m_textureFilterType(TextureFilterType::Nearest)
		, m_lineSpacing(0)
		, m_letterSpacing(0)
	{

	}

	CoreTextObject2D_Imp::~CoreTextObject2D_Imp()
	{
		SafeRelease(m_font);
	}


#pragma region Parameter
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Font* CoreTextObject2D_Imp::GetFont() const
	{
		return m_font;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetFont(Font* font)
	{
		SafeSubstitute(m_font, font);
		SetCullingUpdate(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	WritingDirection CoreTextObject2D_Imp::GetWritingDirection() const
	{
		return m_writingDirection;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetWritingDirection(WritingDirection writingDirection)
	{
		m_writingDirection = writingDirection;
		SetCullingUpdate(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	const achar* CoreTextObject2D_Imp::GetText() const
	{
		return m_text.c_str();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetText(const achar* text)
	{
		m_text = astring(text);
		SetCullingUpdate(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector2DF CoreTextObject2D_Imp::GetCenterPosition() const
	{
		return m_centerPosition;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetCenterPosition(Vector2DF position)
	{
		m_centerPosition = position;
		SetCullingUpdate(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool CoreTextObject2D_Imp::GetTurnLR() const
	{
		return m_turnLR;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetTurnLR(bool turnLR)
	{
		m_turnLR = turnLR;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool CoreTextObject2D_Imp::GetTurnUL() const
	{
		return m_turnUL;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetTurnUL(bool turnUL)
	{
		m_turnUL = turnUL;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	AlphaBlendMode CoreTextObject2D_Imp::GetAlphaBlendMode() const
	{
		return m_alphablend;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetAlphaBlendMode(AlphaBlendMode alphaBlend)
	{
		m_alphablend = alphaBlend;
	}
#pragma endregion

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::CalculateBoundingCircle()
	{
		Vector2DF drawPosition = Vector2DF(0, 0);

		float offset = 0;
		Font_Imp* font_Imp = (Font_Imp*)m_font;

		if (font_Imp == nullptr) return;

		auto parentMatrix = GetParentsMatrix();
		auto matrix = GetMatrixToTransform();

		Vector2DF min = Vector2DF(FLT_MAX, FLT_MAX);
		Vector2DF max = Vector2DF(-FLT_MAX, -FLT_MAX);

		font_Imp->AddCharactorsDynamically(m_text.c_str());
		font_Imp->UpdateTextureDynamically();

		for (int textIndex = 0;; ++textIndex)
		{
			if (m_text[textIndex] == 0) break;

			if (m_text[textIndex] != '\n' && !font_Imp->HasGlyphData(m_text[textIndex]))
			{
				continue;
			}
			else if (m_text[textIndex] == '\n')
			{
				if (m_writingDirection == WritingDirection::Horizontal)
				{
					drawPosition.X = 0;
					drawPosition.Y += (offset + m_lineSpacing);
				}
				else
				{
					drawPosition.X += (offset + m_lineSpacing);
					drawPosition.Y = 0;
				}
				offset = 0;

				continue;
			}

			const GlyphData glyphData = font_Imp->GetGlyphData(m_text[textIndex]);
			auto texture = font_Imp->GetTexture(glyphData.GetSheetNum());

			if (texture == nullptr)
			{
				continue;
			}

			const auto glyphSrc = glyphData.GetSrc();

			std::array<Vector2DF, 4> position;

			{
				position.at(0) = Vector2DF(0, 0);
				position.at(1) = Vector2DF(glyphSrc.Width, 0);
				position.at(2) = Vector2DF(glyphSrc.Width, glyphSrc.Height);
				position.at(3) = Vector2DF(0, glyphSrc.Height);

				for (auto& pos : position)
				{
					pos += drawPosition;
					pos -= m_centerPosition;
					auto v3 = Vector3DF(pos.X, pos.Y, 1);
					auto result = parentMatrix * matrix * v3;
					pos = Vector2DF(result.X, result.Y);
				}

				if (min.X >= position.at(0).X && min.Y >= position.at(0).Y)
				{
					min = position.at(0);
				}

				if (max.X <= position.at(2).X && min.Y <= position.at(2).Y)
				{
					max = position.at(2);
				}

			}

			if (m_writingDirection == WritingDirection::Horizontal)
			{
				drawPosition += asd::Vector2DF(glyphSrc.Width + m_letterSpacing, 0);
				offset = std::max((float)glyphSrc.Height, offset);
			}
			else
			{
				drawPosition += asd::Vector2DF(0, glyphSrc.Height + m_letterSpacing);
				offset = std::max((float)glyphSrc.Width, offset);
			}
		}

		Vector2DF center = (min + max) / 2;
		float rad = (center - min).GetLength();
		m_boundingCircle.Position = culling2d::Vector2DF(center.X, center.Y);
		m_boundingCircle.Radius = rad;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::Draw(Renderer2D* renderer)
	{
		if (!m_objectInfo.GetIsDrawn() || m_font == nullptr
			|| (m_parentInfo != nullptr && !m_parentInfo->GetInheritedBeingDrawn()))
		{
			return;
		}

		auto parentMatrix = GetParentsMatrix();
		auto matrix = GetMatrixToTransform();
		auto inheritedColor = m_parentInfo != nullptr ? m_parentInfo->GetInheritedColor() : Color(255, 255, 255, 255);
		auto inheritedDrawingPriority = m_parentInfo != nullptr ? m_parentInfo->GetInheritedDrawingPriority() : 0;

		renderer->AddText(
			parentMatrix,
			matrix,
			m_centerPosition,
			m_turnLR,
			m_turnUL,
			GetColor(),
			m_font,
			m_text.c_str(),
			m_writingDirection,
			m_alphablend,
			GetDrawingPriority(),
			m_lineSpacing,
			m_letterSpacing,
			m_textureFilterType);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetTextureFilterType(TextureFilterType textureFilterType)
	{
		m_textureFilterType = textureFilterType;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	TextureFilterType CoreTextObject2D_Imp::GetTextureFilterType() const
	{
		return m_textureFilterType;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetLetterSpacing(float letterSpacing)
	{
		m_letterSpacing = letterSpacing;
		SetCullingUpdate(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	float CoreTextObject2D_Imp::GetLetterSpacing() const
	{
		return m_letterSpacing;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetLineSpacing(float lineSpacing)
	{
		m_lineSpacing = lineSpacing;
		SetCullingUpdate(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	float CoreTextObject2D_Imp::GetLineSpacing() const
	{
		return m_lineSpacing;
	}
}
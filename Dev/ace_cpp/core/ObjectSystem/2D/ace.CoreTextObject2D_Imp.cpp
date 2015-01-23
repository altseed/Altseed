
#include "../common/ace.common.Base.h"
#include "ace.CoreTextObject2D_Imp.h"
#include "../../Graphics/Resource/ace.Font_Imp.h"
#include "../../Graphics/Resource/ace.Texture2D_Imp.h"

#ifdef WIN32
#ifdef min
#undef min
#endif 
#ifdef max
#undef max
#endif
#endif

namespace ace
{
	CoreTextObject2D_Imp::CoreTextObject2D_Imp(Graphics_Imp* graphics)
		: CoreObject2D_Imp(graphics)
		, m_font(nullptr)
		, m_centerPosition(Vector2DF())
		, m_color(Color())
		, m_turnLR(false)
		, m_turnUL(false)
		, m_text(ace::ToAString(""))
		, m_alphablend(AlphaBlend::Blend)
		, m_drawingPtiority(0)
		, m_writingDirection(WritingDirection::Horizontal)
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
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Color CoreTextObject2D_Imp::GetColor() const
	{
		return m_color;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetColor(Color color)
	{
		m_color = color;
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
	int CoreTextObject2D_Imp::GetDrawingPriority() const
	{
		return m_drawingPtiority;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetDrawingPriority(int priority)
	{
		m_drawingPtiority = priority;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	AlphaBlend CoreTextObject2D_Imp::GetAlphaBlendMode() const
	{
		return m_alphablend;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetAlphaBlendMode(AlphaBlend alphablend)
	{
		m_alphablend = alphablend;
	}
#pragma endregion

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::CalculateBoundingCircle()
	{
		Vector2DF drawPosition = Vector2DF(0, 0);

		int offset = 0;
		Font_Imp* font_Imp = (Font_Imp*)m_font;

		if (font_Imp == nullptr) return;

		auto parentMatrix = m_transform.GetParentsMatrix();
		auto matrix = m_transform.GetMatrixToTransform();

		Vector2DF min = Vector2DF(FLT_MAX, FLT_MAX);
		Vector2DF max = Vector2DF(-FLT_MAX, -FLT_MAX);

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
					drawPosition.Y += offset;
				}
				else
				{
					drawPosition.X += offset;
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

				if (min.X >= position.at(0).X || min.Y >= position.at(0).Y)
				{
					min = position.at(0);
				}

				if (max.X <= position.at(2).X || min.Y <= position.at(2).Y)
				{
					max = position.at(2);
				}

			}

			if (m_writingDirection == WritingDirection::Horizontal)
			{
				drawPosition += ace::Vector2DF(glyphSrc.Width, 0);
				offset = std::max(glyphSrc.Width, offset);
			}
			else
			{
				drawPosition += ace::Vector2DF(0, glyphSrc.Height);
				offset = std::max(glyphSrc.Height, offset);
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
		if (!m_objectInfo.GetIsDrawn() || m_font == nullptr)
		{
			return;
		}

		auto parentMatrix = m_transform.GetParentsMatrix();
		auto matrix = m_transform.GetMatrixToTransform();

		renderer->AddText(
			parentMatrix,
			matrix,
			m_centerPosition,
			m_turnLR,
			m_turnUL,
			m_color,
			m_font,
			m_text.c_str(),
			m_writingDirection,
			m_alphablend,
			m_drawingPtiority);
	}
}
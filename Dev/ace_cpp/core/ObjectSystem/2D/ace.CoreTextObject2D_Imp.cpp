#include "../common/ace.common.Base.h"
#include "ace.CoreTextObject2D_Imp.h"

namespace ace
{
	CoreTextObject2D_Imp::CoreTextObject2D_Imp(Graphics_Imp* graphics)
		: CoreObject2D_Imp(graphics)
		, m_font(nullptr)
		, m_centerPosition(Vector2DF())
		, m_color(Color())
		, m_turnLR(false)
		, m_turnUL(false)
		, m_writeText(ace::ToAString(""))
		, m_alphablend(eAlphaBlend::ALPHA_BLEND_BLEND)
		, m_drawingPtiority(0)
		, m_drawPosition(Vector2DF())
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
		SafeSubstitute(font, m_font);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector2DF CoreTextObject2D_Imp::GetDrawPosition() const
	{
		return m_drawPosition;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetDrawPosition(Vector2DF drawPosition)
	{
		m_drawPosition = drawPosition;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	TextWritingDirection CoreTextObject2D_Imp::GetTextWritingDirection() const
	{
		return m_textWritingDirection;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetTextWritingDirection(TextWritingDirection textWritingDirection)
	{
		m_textWritingDirection = textWritingDirection;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	astring CoreTextObject2D_Imp::GetWriteText() const
	{
		return m_writeText;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetWriteText(astring writeText)
	{
		m_writeText = writeText;
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
	eAlphaBlend CoreTextObject2D_Imp::GetAlphaBlendMode() const
	{
		return m_alphablend;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetAlphaBlendMode(eAlphaBlend alphablend)
	{
		m_alphablend = alphablend;
	}
#pragma endregion

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::Draw(Renderer2D* renderer, Matrix33 cameraMatrix)
	{
		if (!m_objectInfo.GetIsDrawn())
		{
			return;
		}

		for (int textIndex = 0; textIndex < m_writeText.length(); ++textIndex)
		{

		}

	}
}
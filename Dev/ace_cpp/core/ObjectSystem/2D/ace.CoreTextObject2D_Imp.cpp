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
		, m_alphablend(eAlphaBlend::ALPHA_BLEND_BLEND)
		, m_drawingPtiority(0)
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
	astring CoreTextObject2D_Imp::GetDrawText() const
	{
		return m_drawText;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextObject2D_Imp::SetDrawText(astring drawText)
	{
		m_drawText = drawText;
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

		//ここから描画を書く

	}
}
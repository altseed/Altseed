
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
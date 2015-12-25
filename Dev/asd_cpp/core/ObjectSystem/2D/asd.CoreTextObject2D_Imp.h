#pragma once
#include <memory>
#include "asd.CoreTextObject2D.h"
#include "asd.CoreObject2D_Imp.h"
#include "asd.DrawnParentInfo2D.h"

namespace asd
{
	class CoreTextObject2D_Imp
		: public CoreTextObject2D
		, public CoreObject2D_Imp
		, public ReferenceObject
	{
	protected:
		virtual void CalculateBoundingCircle();
	public:
		CoreTextObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreTextObject2D_Imp();

		Object2DType GetObjectType() const override { return Object2DType::Text; }

	private:
		WritingDirection m_writingDirection;
		Font* m_font;
		Vector2DF m_centerPosition;
		Color m_color;
		bool m_turnLR;
		bool m_turnUL;
		AlphaBlendMode m_alphablend;
		int m_drawingPtiority;
		astring m_text;
		TextureFilterType m_textureFilterType;
		float m_letterSpacing;
		float m_lineSpacing;

	public:
#pragma region Parameter
		Font* GetFont() const override;
		void SetFont(Font* font) override;

		WritingDirection GetWritingDirection() const override;
		void SetWritingDirection(WritingDirection writingDirection) override;

		const achar* GetText() const override;
		void SetText(const achar* text) override;

		Vector2DF GetCenterPosition() const override;
		void SetCenterPosition(Vector2DF position) override;

		Color GetColor() const override;
		void SetColor(Color color) override;

		bool GetTurnLR() const override;
		void SetTurnLR(bool turnLR) override;

		bool GetTurnUL() const override;
		void SetTurnUL(bool turnUL) override;

		int GetDrawingPriority() const override;
		void SetDrawingPriority(int priority) override;

		AlphaBlendMode GetAlphaBlendMode() const override;
		void SetAlphaBlendMode(AlphaBlendMode alphaBlend) override;

		void SetTextureFilterType(TextureFilterType textureFilterType) override;
		TextureFilterType GetTextureFilterType() const override;

		void SetLetterSpacing(float letterSpacing) override;
		float GetLetterSpacing() const override;

		void SetLineSpacing(float lineSpacing) override;
		float GetLineSpacing() const  override;

#pragma endregion

		void Draw(Renderer2D* renderer) override;
		/*
		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
		*/

#include "asd.CoreObject2D_Imp_Methods.h"
#include "asd.CoreDrawnObject2D_Imp_Methods.h"
	};
}
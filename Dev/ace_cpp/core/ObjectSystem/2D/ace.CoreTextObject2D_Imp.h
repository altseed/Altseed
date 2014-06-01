
#include "ace.CoreTextObject2D.h"
#include "ace.CoreObject2D_Imp.h"

namespace ace
{
	class CoreTextObject2D_Imp
		: public CoreTextObject2D
		, public CoreObject2D_Imp
		, public ReferenceObject
	{
	public:
		CoreTextObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreTextObject2D_Imp();

		eObject2DType GetObjectType() const override { return eObject2DType::RENDERED_OBJECT2D_TYPE_TEXT; }

	private:
		TextWritingDirection m_textWritingDirection;
		Font* m_font;
		Vector2DF m_centerPosition;
		Color m_color;
		bool m_turnLR;
		bool m_turnUL;
		eAlphaBlend m_alphablend;
		int m_drawingPtiority;
		astring m_writeText;

	public:
#pragma region Parameter
		Font* GetFont() const override;
		void SetFont(Font* font) override;

		TextWritingDirection GetTextWritingDirection() const override;
		void SetTextWritingDirection(TextWritingDirection textWritingDirection) override;

		astring GetWriteText() const override;
		void SetWriteText(astring writeText) override;

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

		eAlphaBlend GetAlphaBlendMode() const override;
		void SetAlphaBlendMode(eAlphaBlend alphaBlend) override;
#pragma endregion

		void Draw(Renderer2D* renderer, Matrix33 cameraMatrix) override;

		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
	};
}
#pragma once
#include "../common/Math/ace.RectF.h"
#include "ace.CoreTextureObject2D.h"
#include "ace.CoreObject2D_Imp.h"

namespace ace
{
	class CoreTextureObject2D_Imp
		: public CoreTextureObject2D
		, public CoreObject2D_Imp
		, public ReferenceObject
	{

	protected:
		virtual void CalculateBoundingCircle() override;
	private:
		Texture2D* m_texture;
		RectF m_src;
		Vector2DF m_centerPosition;
		Color m_color;
		bool m_turnLR;
		bool m_turnUL;
		AlphaBlend m_alphablend;
		int m_drawingPtiority;
		TextureFilterType m_textureFilterType;

	public:
		CoreTextureObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreTextureObject2D_Imp();

		Object2DType GetObjectType() const override { return Object2DType::Texture; }

#pragma region Parameter
		Texture2D* GetTexture() const;
		void SetTexture(Texture2D* texture);

		RectF GetSrc() const;
		void SetSrc(RectF value);

		Vector2DF GetCenterPosition() const;
		void SetCenterPosition(Vector2DF position);

		Color GetColor() const;
		void SetColor(Color color);

		bool GetTurnLR() const;
		void SetTurnLR(bool turnLR);

		bool GetTurnUL() const;
		void SetTurnUL(bool turnUL);

		int GetDrawingPriority() const;
		void SetDrawingPriority(int priority);

		AlphaBlend GetAlphaBlendMode() const;
		void SetAlphaBlendMode(AlphaBlend alphaBlend);

		void SetTextureFilterType(TextureFilterType textureFilterType) override;
		TextureFilterType GetTextureFilterType() const override;
#pragma endregion

		void Draw(Renderer2D* renderer);
		/*
		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
		*/

#include "ace.CoreObject2D_Imp_Methods.h"
	};
}
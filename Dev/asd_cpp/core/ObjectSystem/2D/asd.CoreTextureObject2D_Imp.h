#pragma once
#include <memory>
#include "../common/Math/asd.RectF.h"
#include "asd.CoreTextureObject2D.h"
#include "asd.CoreObject2D_Imp.h"
#include "asd.DrawnParentInfo2D.h"
#include "asd.CoreDrawnObject2D_Imp.h"

namespace asd
{
	class CoreTextureObject2D_Imp
		: public CoreTextureObject2D
		, public CoreDrawnObject2D_Imp
		, public ReferenceObject
	{

	protected:
		virtual void CalculateBoundingCircle() override;
	private:
		Texture2D* m_texture;
		RectF m_src;
		Vector2DF m_centerPosition;
		bool m_turnLR;
		bool m_turnUL;
		AlphaBlendMode m_alphablend;
		TextureFilterType m_textureFilterType;
		Material2D*	m_material = nullptr;

	public:
		CoreTextureObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreTextureObject2D_Imp();

		Object2DType GetObjectType() const override { return Object2DType::Texture; }

#pragma region Parameter
		Material2D* GetMaterial() const override;
		void SetMaterial(Material2D* material) override;

		Texture2D* GetTexture() const;
		void SetTexture(Texture2D* texture);

		RectF GetSrc() const;
		void SetSrc(RectF value);

		Vector2DF GetCenterPosition() const;
		void SetCenterPosition(Vector2DF position);

		bool GetTurnLR() const;
		void SetTurnLR(bool turnLR);

		bool GetTurnUL() const;
		void SetTurnUL(bool turnUL);

		AlphaBlendMode GetAlphaBlendMode() const;
		void SetAlphaBlendMode(AlphaBlendMode alphaBlend);

		void SetTextureFilterType(TextureFilterType textureFilterType) override;
		TextureFilterType GetTextureFilterType() const override;
#pragma endregion

		void Draw(Renderer2D* renderer);
		/*
		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
		*/

#include "asd.CoreObject2D_Imp_Methods.h"
#include "asd.CoreDrawnObject2D_Imp_Methods.h"
	};
}
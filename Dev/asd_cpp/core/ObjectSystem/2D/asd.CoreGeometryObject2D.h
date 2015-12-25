#pragma once
#include "asd.CoreObject2D.h"
#include "../common/Math/asd.RectI.h"
#include "../common/Math/asd.Matrix33.h"
#include "../../Graphics/Resource/asd.RenderTexture2D.h"
#include "asd.CoreDrawnObject2D.h"

namespace asd
{
	class CoreShape;

	class CoreGeometryObject2D
		: public CoreDrawnObject2D
	{
	public:
		CoreGeometryObject2D() {}
		virtual ~CoreGeometryObject2D() {}

		virtual CoreShape* GetShape() const = 0;
		virtual void SetShape(CoreShape* shape)= 0;

		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int priority) = 0;

		virtual AlphaBlendMode GetAlphaBlendMode() const = 0;
		virtual void SetAlphaBlendMode(AlphaBlendMode alphaBlend) = 0;

		virtual void SetCenterPosition(Vector2DF position) = 0;
		virtual Vector2DF GetCenterPosition() const = 0;

		virtual void SetColor(Color color) = 0;
		virtual Color GetColor() const = 0;

		virtual void SetTextureFilterType(TextureFilterType textureFilterType) = 0;
		virtual TextureFilterType GetTextureFilterType() const = 0;

		virtual void SetTexture(Texture2D* texture) = 0;
		virtual Texture2D* GetTexture() const = 0;

	};
}
#pragma once

#include "ace.Object2D.h"
#include "../../Shape/ace.Shape.h"

namespace ace
{
	class GeometryObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreGeometryObject2D> m_coreObject;
		std::shared_ptr<Texture2D> texturePtr;
		std::shared_ptr<Shape> shapePtr;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<CoreGeometryObject2D> Ptr;

		GeometryObject2D();
		virtual ~GeometryObject2D();

		std::shared_ptr<Shape> GetShape() const ;
		void SetShape(std::shared_ptr<Shape> shape);

		int GetDrawingPriority() const;
		void SetDrawingPriority(int priority);

		AlphaBlend GetAlphaBlendMode() const;
		void SetAlphaBlendMode(AlphaBlend alphaBlend);

		void SetCenterPosition(Vector2DF centerPosition);
		Vector2DF GetCenterPosition() const;

		void SetColor(Color color);
		Color GetColor() const;

		void SetTextureFilterType(TextureFilterType textureFilterType);
		TextureFilterType GetTextureFilterType() const;

		void SetTexture(std::shared_ptr<Texture2D> texture);
		std::shared_ptr<Texture2D> GetTexture() const;
	};
}
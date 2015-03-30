#include "ace.CoreGeometryObject2D.h"
#include "../../Shape/ace.CoreShape.h"
#include "ace.CoreObject2D_Imp.h"

namespace ace
{

	class CoreGeometryObject2D_Imp
		: public CoreGeometryObject2D
		, public CoreObject2D_Imp
		, public ReferenceObject
	{
		CoreShape* m_shape;
		int drawingPriority;
		AlphaBlend alphaBlendMode;
		Vector2DF centerPosition;
		Color m_color;
		TextureFilterType m_textureFilterType;
		Texture2D* m_texture;

	public:
		Object2DType GetObjectType() const override { return Object2DType::Geometry; }

		CoreGeometryObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreGeometryObject2D_Imp() {}

		CoreShape* GetShape() const;
		void SetShape(CoreShape* shape);

		int GetDrawingPriority() const ;
		void SetDrawingPriority(int priority) ;

		AlphaBlend GetAlphaBlendMode() const ;
		void SetAlphaBlendMode(AlphaBlend alphaBlend) ;

		void SetCenterPosition(Vector2DF position) ;
		Vector2DF GetCenterPosition() const ;

		void SetColor(Color color) ;
		Color GetColor() const ;

		void SetTextureFilterType(TextureFilterType textureFilterType) ;
		TextureFilterType GetTextureFilterType() const ;

		void SetTexture(Texture2D* texture) ;
		Texture2D* GetTexture() const;
#if !SWIG

		void Draw(Renderer2D* renderer) override;
#endif

#include "ace.CoreObject2D_Imp_Methods.h"

	};
}
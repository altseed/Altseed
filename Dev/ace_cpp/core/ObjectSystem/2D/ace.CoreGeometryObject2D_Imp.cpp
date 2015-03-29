#include "ace.CoreGeometryObject2D_Imp.h"

namespace ace
{
	CoreGeometryObject2D_Imp::CoreGeometryObject2D_Imp(Graphics_Imp* graphics)
		:
		CoreObject2D_Imp(graphics)
		,m_shape(nullptr)
		, drawingPriority(0)
		, alphaBlendMode(AlphaBlend::Add)
		, centerPosition(Vector2DF())
		, m_color(Color())
		, m_textureFilterType(TextureFilterType::Linear)
		, m_texture(nullptr)
	{

	}

	CoreShape* CoreGeometryObject2D_Imp::GetShape() const
	{
		return m_shape;
	}

	void CoreGeometryObject2D_Imp::SetShape(CoreShape* shape)
	{
		SafeSubstitute(m_shape, shape);
	}

	int CoreGeometryObject2D_Imp::GetDrawingPriority() const
	{
		return drawingPriority;
	}

	void CoreGeometryObject2D_Imp::SetDrawingPriority(int priority)
	{
		drawingPriority = priority;
	}

	AlphaBlend CoreGeometryObject2D_Imp::GetAlphaBlendMode() const
	{
		return alphaBlendMode;
	}

	void CoreGeometryObject2D_Imp::SetAlphaBlendMode(AlphaBlend alphaBlend)
	{
		alphaBlendMode = alphaBlend;
	}

	void CoreGeometryObject2D_Imp::SetCenterPosition(Vector2DF position)
	{
		centerPosition = centerPosition;
	}

	Vector2DF CoreGeometryObject2D_Imp::GetCenterPosition() const
	{
		return centerPosition;
	}

	void CoreGeometryObject2D_Imp::SetColor(Color color)
	{
		m_color = color;
	}

	Color CoreGeometryObject2D_Imp::GetColor() const
	{
		return m_color;
	}

	void CoreGeometryObject2D_Imp::SetTextureFilterType(TextureFilterType textureFilterType)
	{
		m_textureFilterType = textureFilterType;
	}

	TextureFilterType CoreGeometryObject2D_Imp::GetTextureFilterType() const
	{
		return m_textureFilterType;
	}

	void CoreGeometryObject2D_Imp::SetTexture(Texture2D* texture)
	{
		SafeSubstitute(m_texture, texture);
	}

	Texture2D* CoreGeometryObject2D_Imp::GetTexture() const
	{
		return m_texture;
	}

#if !SWIG

	void CoreGeometryObject2D_Imp::Draw(Renderer2D* renderer)
	{

	}

#endif
}
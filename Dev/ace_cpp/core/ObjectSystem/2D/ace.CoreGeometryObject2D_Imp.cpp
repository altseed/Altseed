#include "ace.CoreGeometryObject2D_Imp.h"
#include "../../Shape/ace.CoreTriangleShape.h"

namespace ace
{
	CoreGeometryObject2D_Imp::CoreGeometryObject2D_Imp(Graphics_Imp* graphics)
		:
		CoreObject2D_Imp(graphics)
		,m_shape(nullptr)
		, drawingPriority(0)
		, alphaBlendMode(AlphaBlendMode::Add)
		, centerPosition(Vector2DF())
		, m_color(Color())
		, m_textureFilterType(TextureFilterType::Linear)
		, m_texture(nullptr)
	{

	}

	CoreGeometryObject2D_Imp::~CoreGeometryObject2D_Imp()
	{
		SafeRelease(m_shape);
		SafeRelease(m_texture);
	}

	CoreShape* CoreGeometryObject2D_Imp::GetShape() const
	{
		return m_shape;
	}

	void CoreGeometryObject2D_Imp::SetShape(CoreShape* shape)
	{
		SetCullingUpdate(this);
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

	AlphaBlendMode CoreGeometryObject2D_Imp::GetAlphaBlendMode() const
	{
		return alphaBlendMode;
	}

	void CoreGeometryObject2D_Imp::SetAlphaBlendMode(AlphaBlendMode alphaBlend)
	{
		alphaBlendMode = alphaBlend;
	}

	void CoreGeometryObject2D_Imp::SetCenterPosition(Vector2DF position)
	{
		SetCullingUpdate(this);
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
		if (!m_objectInfo.GetIsDrawn())
		{
			return;
		}

		for (auto triangle : m_shape->GetDividedTriangles())
		{
			std::array<Vector2DF, 4> position;
			std::array<Vector2DF, 4> uvs;

			for (int i = 0; i < 3; ++i)
			{
				position[i] = triangle->GetPointByIndex(i);
				uvs[i] = triangle->GetUVByIndex(i);
			}
			position[3] = position[2];
			uvs[3] = uvs[2];

			auto parentMatrix = m_transform.GetParentsMatrix();
			auto matrix = m_transform.GetMatrixToTransform();

			for (auto& pos : position)
			{
				pos -= centerPosition;
				auto v3 = Vector3DF(pos.X, pos.Y, 1);
				auto result = parentMatrix * matrix * v3;
				pos = Vector2DF(result.X, result.Y);
			}

			Color color[4];
			color[0] = m_color;
			color[1] = m_color;
			color[2] = m_color;
			color[3] = m_color;

			renderer->AddSprite(position.data(), color, uvs.data(), (m_shape->GetShapeType() == ShapeType::LineShape) ? nullptr : m_texture, alphaBlendMode, drawingPriority, m_textureFilterType);
		}
	}

#endif

	void CoreGeometryObject2D_Imp::CalculateBoundingCircle()
	{
		m_boundingCircle = m_shape->GetBoundingCircle();

		std::array<Vector2DF, 4> position;

		auto p1 = m_boundingCircle.Position - culling2d::Vector2DF(m_boundingCircle.Radius, 0);
		position[0] = Vector2DF(p1.X, p1.Y);

		auto p2 = m_boundingCircle.Position + culling2d::Vector2DF(m_boundingCircle.Radius, 0);
		position[1] = Vector2DF(p2.X, p2.Y);

		auto p3 = m_boundingCircle.Position - culling2d::Vector2DF(0, m_boundingCircle.Radius);
		position[2] = Vector2DF(p3.X, p3.Y);

		auto p4 = m_boundingCircle.Position + culling2d::Vector2DF(0, m_boundingCircle.Radius);
		position[3] = Vector2DF(p4.X, p4.Y);

		auto parentMatrix = m_transform.GetParentsMatrix();
		auto matrix = m_transform.GetMatrixToTransform();

		Vector2DF sum = Vector2DF();
		for (auto& pos : position)
		{
			pos -= centerPosition;
			auto v3 = Vector3DF(pos.X, pos.Y, 1);
			auto result = parentMatrix * matrix * v3;
			pos = Vector2DF(result.X, result.Y);
			sum += pos;
		}

		auto c = sum / 4.0f;
		auto r = 0.0f;

		for (auto& pos : position)
		{
			r = Max(r, (pos - c).GetLength());
		}

		m_boundingCircle = culling2d::Circle(culling2d::Vector2DF(c.X, c.Y), r);
	}
}
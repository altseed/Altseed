#include "../common/Math/ace.Vector3DF.h"
#include "../common/ace.common.Base.h"
#include "ace.CoreTextureObject2D_Imp.h"
#include <array>
#include "ace.Culling2D.h"

namespace ace
{
	CoreTextureObject2D_Imp::CoreTextureObject2D_Imp(Graphics_Imp* graphics)
		: m_texture(nullptr)
		, m_centerPosition(Vector2DF())
		, m_color(Color())
		, m_turnLR(false)
		, m_turnUL(false)
		, m_alphablend(AlphaBlend::Blend)
		, m_drawingPtiority(0)
		, m_src(RectF(0, 0, 1, 1))
		, CoreObject2D_Imp(graphics)
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreTextureObject2D_Imp::~CoreTextureObject2D_Imp()
	{
		SafeRelease(m_texture);

		// 循環になるので参照カウンタは変更しない
		//SafeRelease(m_layer);
	}


#pragma region Paramater
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D* CoreTextureObject2D_Imp::GetTexture() const
	{
		return m_texture;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::SetTexture(Texture2D* texture)
	{
		SafeSubstitute(m_texture, texture);
		if (texture != nullptr)
		{
			SetSrc(RectF(0, 0, texture->GetSize().X, texture->GetSize().Y));
		}
		else
		{
			m_src = RectF(0, 0, 1, 1);
		}
	}

	RectF CoreTextureObject2D_Imp::GetSrc() const
	{
		return m_src;
	}

	void CoreTextureObject2D_Imp::SetSrc(RectF value)
	{
		m_src = value; 
		SetCullingUpdate(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector2DF CoreTextureObject2D_Imp::GetCenterPosition() const
	{
		return m_centerPosition;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::SetCenterPosition(Vector2DF position)
	{
		m_centerPosition = position;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Color CoreTextureObject2D_Imp::GetColor() const
	{
		return m_color;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::SetColor(Color color)
	{
		m_color = color;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool CoreTextureObject2D_Imp::GetTurnLR() const
	{
		return m_turnLR;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::SetTurnLR(bool turnLR)
	{
		m_turnLR = turnLR;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool CoreTextureObject2D_Imp::GetTurnUL() const
	{
		return m_turnUL;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::SetTurnUL(bool turnUL)
	{
		m_turnUL = turnUL;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	int CoreTextureObject2D_Imp::GetDrawingPriority() const
	{
		return m_drawingPtiority;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::SetDrawingPriority(int priority)
	{
		m_drawingPtiority = priority;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	AlphaBlend CoreTextureObject2D_Imp::GetAlphaBlendMode() const
	{
		return m_alphablend;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::SetAlphaBlendMode(AlphaBlend alphablend)
	{
		m_alphablend = alphablend;
	}
#pragma endregion

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::CalculateBoundingCircle()
	{
		std::array<Vector2DF, 4> position = m_src.GetVertexes();

		{
			Vector2DF origin = position[0];
			for (int i = 0; i < 4; ++i)
			{
				position[i] -= origin;
			}
		}

		auto parentMatrix = m_transform.GetParentsMatrix();
		auto matrix = m_transform.GetMatrixToTransform();

		for (auto& pos : position)
		{
			pos -= m_centerPosition;
			auto v3 = Vector3DF(pos.X, pos.Y, 1);
			auto result = parentMatrix * matrix * v3;
			pos = Vector2DF(result.X, result.Y);
		}

		Vector2DF center = (position[0] + position[1] + position[2] + position[3]) / 4;
		float len = (center - position[0]).GetLength();
		culling2d::Vector2DF cent = culling2d::Vector2DF(center.X, center.Y);
		m_boundingCircle.Position = cent;
		m_boundingCircle.Radius = len;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::Draw(Renderer2D* renderer)
	{
		if (!m_objectInfo.GetIsDrawn())
		{
			return;
		}

		std::array<Vector2DF, 4> position = m_src.GetVertexes();

		{
			Vector2DF origin = position[0];
			for (int i = 0; i < 4; ++i)
			{
				position[i] -= origin;
			}
		}

		auto textureSize = m_texture != nullptr ? m_texture->GetSize() : Vector2DI(1, 1);

		auto parentMatrix = m_transform.GetParentsMatrix();
		auto matrix = m_transform.GetMatrixToTransform();

		for (auto& pos : position)
		{
			pos -= m_centerPosition;
			auto v3 = Vector3DF(pos.X, pos.Y, 1);
			auto result = parentMatrix * matrix * v3;
			pos = Vector2DF(result.X, result.Y);
		}

		Color color[4];
		color[0] = m_color;
		color[1] = m_color;
		color[2] = m_color;
		color[3] = m_color;

		std::array<Vector2DF, 4> uvs;
		
		if (m_src.X < 0 || m_src.Y < 0 || m_src.Width < 0 || m_src.Height < 0)
		{
			uvs[0] = Vector2DF(0, 0);
			uvs[1] = Vector2DF(textureSize.X, 0);
			uvs[2] = Vector2DF(textureSize.X, textureSize.Y);
			uvs[3] = Vector2DF(0, textureSize.Y);
		}
		else
		{
			uvs = m_src.GetVertexes();
		}

		{
			auto size = Vector2DF(textureSize.X, textureSize.Y);
			for (int i = 0; i < 4; ++i)
			{
				uvs[i] /= size;
			}
		}

		if (m_turnLR)
		{
			std::swap(uvs[0], uvs[1]);
			std::swap(uvs[2], uvs[3]);
		}

		if (m_turnUL)
		{
			std::swap(uvs[0], uvs[3]);
			std::swap(uvs[1], uvs[2]);
		}

		renderer->AddSprite(
			position.data(),
			color,
			uvs.data(),
			m_texture,
			m_alphablend,
			m_drawingPtiority);
	}
}

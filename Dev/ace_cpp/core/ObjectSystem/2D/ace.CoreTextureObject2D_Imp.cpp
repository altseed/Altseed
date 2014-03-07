#include "../common/Math/ace.Vector3DF.h"
#include "../common/ace.common.Base.h"
#include "ace.CoreTextureObject2D_Imp.h"
#include <array>

namespace ace
{
	CoreTextureObject2D_Imp::CoreTextureObject2D_Imp()
		: m_texture(nullptr)
		, m_centerPosition(Vector2DF())
		, m_color(Color())
		, m_turnLR(false)
		, m_turnUL(false)
		, m_alphablend(eAlphaBlend::ALPHA_BLEND_BLEND)
		, m_drawingPtiority(0)
		, m_transform(TransformInfo2D())
		, m_objectInfo(ObjectInfo2D())
		, m_src(RectF(0, 0, 1, 1))
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
		SafeRelease(m_texture);
		m_texture = texture;
		SafeAddRef(texture);
		SetSrc(RectF(0, 0, texture->GetSize().X, texture->GetSize().Y));
	}

	RectF CoreTextureObject2D_Imp::GetSrc() const
	{
		return m_src;
	}

	void CoreTextureObject2D_Imp::SetSrc(RectF value)
	{
		m_src = value;
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
	eAlphaBlend CoreTextureObject2D_Imp::GetAlphaBlendMode() const
	{
		return m_alphablend;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::SetAlphaBlendMode(eAlphaBlend alphablend)
	{
		m_alphablend = alphablend;
	}
#pragma endregion

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::Draw(Renderer2D* renderer, Matrix33 cameraMatrix)
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
			auto result = cameraMatrix * parentMatrix * matrix * v3;
			pos = Vector2DF(result.X, result.Y);
		}

		Color color[4];
		color[0] = m_color;
		color[1] = m_color;
		color[2] = m_color;
		color[3] = m_color;

		std::array<Vector2DF, 4> uvs = m_src.GetVertexes();

		{
			auto size = Vector2DF(textureSize.X, textureSize.Y);
			for (int i = 0; i < 4; ++i)
			{
				uvs[i] /= size;
			}
		}

		for (auto& uv : uvs)
		{
			if (m_turnLR)
			{
				uv.X = uv.X == 0 ? 1 : 0;
			}
			if (m_turnUL)
			{
				uv.Y = uv.Y == 0 ? 1 : 0;
			}
		}

		renderer->AddSprite(
			position.data(),
			color,
			uvs.data(),
			m_texture,
			m_alphablend,
			m_drawingPtiority);
	}

	void CoreTextureObject2D_Imp::AddChild(CoreObject2D& child, eChildMode mode)
	{
		child.SetParent(*this, mode);
	}

	void CoreTextureObject2D_Imp::RemoveChild(CoreObject2D& child)
	{
		child.ClearParent();
	}
}

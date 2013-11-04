#include "../common/Math/ace.Vector3DF.h"
#include "../common/ace.common.Base.h"
#include "ace.CoreTextureObject2D_Imp.h"

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
		, m_layer(nullptr)
		, m_transform(TransformInfo2D())
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

#pragma region Get/Set
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
	void CoreTextureObject2D_Imp::Draw()
	{
		Vector2DF position[4];
		Color color[4];
		Vector2DF uvs[4];

		auto p = m_transform.GetPosition();

		{
			auto textureSize = m_texture != nullptr ? m_texture->GetSize() : Vector2DI(1, 1);
			auto x = p.X;
			auto y = p.Y;
			auto w = textureSize.X;
			auto h = textureSize.Y;

			position[0].X = 0;
			position[0].Y = 0;
			position[1].X = w;
			position[1].Y = 0;
			position[2].X = w;
			position[2].Y = h;
			position[3].X = 0;
			position[3].Y = h;
		}

		auto radian = DegreeToRadian(m_transform.GetAngle());
		auto scale = m_transform.GetScale();
		auto matrix = m_transform.GetParentsMatrix();
		for (auto& pos : position)
		{
			pos -= m_centerPosition;

			auto x = pos.X;
			auto y = pos.Y;
			float sin, cos;
			SinCos(radian, sin, cos);
			pos.X = x * cos - y * sin;
			pos.Y = x * sin + y * cos;

			pos.X *= scale.X;
			pos.Y *= scale.Y;

			pos += p;

			auto v3 = Vector3DF(pos.X, pos.Y, 1);
			auto result = matrix * v3;
			pos = Vector2DF(result.X, result.Y);
		}

		color[0] = m_color;
		color[1] = m_color;
		color[2] = m_color;
		color[3] = m_color;

		uvs[0].X = 0;
		uvs[0].Y = 0;
		uvs[1].X = 1;
		uvs[1].Y = 0;
		uvs[2].X = 1;
		uvs[2].Y = 1;
		uvs[3].X = 0;
		uvs[3].Y = 1;

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

		m_layer->GetRenderer()->AddSprite(
			position,
			color,
			uvs,
			m_texture,
			m_alphablend,
			m_drawingPtiority);
	}

	void CoreTextureObject2D_Imp::SetLayer(CoreLayer2D* layer)
	{
		m_layer = layer;
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
#include "../common/Math/asd.Vector3DF.h"
#include "../common/asd.common.Base.h"
#include "asd.CoreTextureObject2D_Imp.h"
#include <array>
#include "asd.Culling2D.h"
#include "../../Graphics/Resource/asd.Material2D.h"
#include "../../Graphics/Resource/asd.Shader2D.h"

namespace asd
{
	CoreTextureObject2D_Imp::CoreTextureObject2D_Imp(Graphics_Imp* graphics)
		: m_texture(nullptr)
		, m_centerPosition(Vector2DF())
		, m_turnLR(false)
		, m_turnUL(false)
		, m_alphablend(AlphaBlendMode::Blend)
		, m_src(RectF(0, 0, 1, 1))
		, CoreDrawnObject2D_Imp(graphics)
		, m_textureFilterType(TextureFilterType::Nearest)
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
	Material2D* CoreTextureObject2D_Imp::GetMaterial() const
	{
		return m_material;
	}

	void CoreTextureObject2D_Imp::SetMaterial(Material2D* material)
	{
		SafeSubstitute(m_material, material);
	}

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
		SetCullingUpdate(this);
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
		SetCullingUpdate(this);
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
	AlphaBlendMode CoreTextureObject2D_Imp::GetAlphaBlendMode() const
	{
		return m_alphablend;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::SetAlphaBlendMode(AlphaBlendMode alphaBlend)
	{
		m_alphablend = alphaBlend;
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

		auto parentMatrix = GetParentsMatrix();
		auto matrix = GetMatrixToTransform();

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


	void CoreTextureObject2D_Imp::SetTextureFilterType(TextureFilterType textureFilterType)
	{
		m_textureFilterType = textureFilterType;
	}

	TextureFilterType CoreTextureObject2D_Imp::GetTextureFilterType() const
	{
		return m_textureFilterType;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreTextureObject2D_Imp::Draw(Renderer2D* renderer)
	{
		if (!GetAbsoluteBeingDrawn() || !GetIsAlive())
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

		auto matrix = GetAbsoluteMatrixToTransform();

		for (auto& pos : position)
		{
			pos -= m_centerPosition;
			auto v3 = Vector3DF(pos.X, pos.Y, 1);
			auto result = matrix * v3;
			pos = Vector2DF(result.X, result.Y);
		}

		Color color[4];
		auto col = GetAbsoluteColor();
		color[0] = col;
		color[1] = col;
		color[2] = col;
		color[3] = col;

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

		if (m_material != nullptr)
		{
			renderer->AddSpriteWithMaterial(
				position.data(),
				color,
				uvs.data(),
				uvs.data(),
				m_texture,
				m_material,
				m_alphablend,
				GetAbsoluteDrawingPriority(),
				m_textureFilterType);
		}
		else
		{
			renderer->AddSprite(
				position.data(),
				color,
				uvs.data(),
				m_texture,
				m_alphablend,
				GetAbsoluteDrawingPriority(),
				m_textureFilterType);

		}
	}
}

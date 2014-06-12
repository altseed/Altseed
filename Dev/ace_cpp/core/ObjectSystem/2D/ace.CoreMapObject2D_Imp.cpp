#include "../common/ace.common.Base.h"
#include "ace.CoreMapObject2D_Imp.h"
#include "../../Graphics/Resource/ace.Chip2D_Imp.h"
#include "../../Graphics/Resource/ace.Texture2D_Imp.h"

namespace ace
{
	CoreMapObject2D_Imp::CoreMapObject2D_Imp(Graphics_Imp* graphics)
		: CoreObject2D_Imp(graphics)
		, m_centerPosition(Vector2DF())
		, m_drawingPtiority(0)
	{
		m_chips.clear();
	}

	CoreMapObject2D_Imp::~CoreMapObject2D_Imp()
	{
		for(auto chip : m_chips)
		{
			SafeRelease(chip);
		}
		m_chips.clear();
	}


#pragma region Parameter
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	const Vector2DF CoreMapObject2D_Imp::GetCenterPosition() const
	{
		return m_centerPosition;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreMapObject2D_Imp::SetCenterPosition(Vector2DF position)
	{
		m_centerPosition = position;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	const int CoreMapObject2D_Imp::GetDrawingPriority() const
	{
		return m_drawingPtiority;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreMapObject2D_Imp::SetDrawingPriority(int priority)
	{
		m_drawingPtiority = priority;
	}

#pragma endregion

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	const bool CoreMapObject2D_Imp::AddChip(Chip2D* chip)
	{
		if (chip == nullptr)
		{
			return false;
		}

		auto pair = m_chips.insert(chip);

		if (pair.second)
		{
			SafeAddRef(chip);
		}
		return pair.second;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	const bool CoreMapObject2D_Imp::RemoveChip(Chip2D* chip)
	{
		if (chip == nullptr)
		{
			return false;
		}

		auto prevSize = m_chips.size();
		auto newSize = m_chips.erase(chip);

		if (prevSize != newSize)
		{
			SafeRelease(chip);
		}
		return (prevSize != newSize);
	}
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreMapObject2D_Imp::Draw(Renderer2D* renderer, Matrix33 cameraMatrix)
	{
		if (!m_objectInfo.GetIsDrawn())
		{
			return;
		}

		const auto parentMatrix = m_transform.GetParentsMatrix();
		const auto matrix = m_transform.GetMatrixToTransform();

		for (auto chip = m_chips.begin(); chip != m_chips.end(); ++chip)
		{

			Texture2D* texture = (*chip)->GetTexture();

			if (texture == nullptr)
			{
				continue;
			}

			std::array<Vector2DF, 4> position = (*chip)->GetSrc().GetVertexes();

			{

				for (auto& pos : position)
				{
					pos -= m_centerPosition;
					auto v3 = Vector3DF(pos.X, pos.Y, 1);
					auto result = cameraMatrix * parentMatrix * matrix * v3;
					pos = Vector2DF(result.X, result.Y);
				}

			}


			std::array<Vector2DF, 4> uvs;
			{
				const auto textureSize = Vector2DF(texture->GetSize().X, texture->GetSize().Y);

				uvs.at(0) = Vector2DF(0, 0);
				uvs.at(1) = Vector2DF(textureSize.X, 0);
				uvs.at(2) = Vector2DF(textureSize.X, textureSize.Y);
				uvs.at(3) = Vector2DF(0, textureSize.Y);

				for (auto& uv : uvs)
				{
					uv /= textureSize;
				}

				if ((*chip)->GetTurnLR())
				{
					std::swap(uvs.at(0), uvs.at(1));
					std::swap(uvs.at(2), uvs.at(3));
				}

				if ((*chip)->GetTurnUL())
				{
					std::swap(uvs.at(0), uvs.at(3));
					std::swap(uvs.at(1), uvs.at(2));
				}
			}

			std::array<Color, 4> color;
			color[0] = (*chip)->GetColor();
			color[1] = (*chip)->GetColor();
			color[2] = (*chip)->GetColor();
			color[3] = (*chip)->GetColor();

			renderer->AddSprite(position.data(), &color[0], uvs.data(), texture, (*chip)->GetAlphaBlendMode(), m_drawingPtiority);
		}

	}
}
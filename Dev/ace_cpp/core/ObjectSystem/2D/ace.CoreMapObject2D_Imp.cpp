#include "../common/ace.common.Base.h"
#include "ace.CoreMapObject2D_Imp.h"
#include "../../Graphics/Resource/ace.Chip2D_Imp.h"
#include "../../Graphics/Resource/ace.Texture2D_Imp.h"
#include "../2D/ace.CoreLayer2D_Imp.h"
#include "../2D/ace.Culling2D.h"

namespace ace
{
	CoreMapObject2D_Imp::CoreMapObject2D_Imp(Graphics_Imp* graphics)
		: CoreObject2D_Imp(graphics)
		, m_centerPosition(Vector2DF())
		, m_drawingPtiority(0)
		, firstSortedKey(0)
	{
		m_chips.clear();
		alreadyCullingUpdated = true;
	}

	CoreMapObject2D_Imp::~CoreMapObject2D_Imp()
	{
		for (auto chip : m_chips)
		{
			SafeRelease(chip);
		}
		m_chips.clear();
	}

	void CoreMapObject2D_Imp::CalculateBoundingCircle()
	{

	}
#pragma region Parameter
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector2DF CoreMapObject2D_Imp::GetCenterPosition() const
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
	int CoreMapObject2D_Imp::GetDrawingPriority() const
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
	bool CoreMapObject2D_Imp::AddChip(Chip2D* chip)
	{
		if (chip == nullptr)
		{
			return false;
		}

		auto pair = m_chips.insert(chip);

		if (pair.second)
		{
			SafeAddRef(chip);

			auto chip_Imp = (Chip2D_Imp*)chip;
			chip_Imp->SetMapObject2D(this);

#if __CULLING_2D__
			auto layer = (CoreLayer2D_Imp*)m_objectInfo.GetLayer();
			if (layer != nullptr)
			{
				layer->AddChipCullingObject(chip_Imp,firstSortedKey);
			}
#endif

		}
		return pair.second;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool CoreMapObject2D_Imp::RemoveChip(Chip2D* chip)
	{
		if (chip == nullptr)
		{
			return false;
		}

		auto prevSize = m_chips.size();
		auto newSize = m_chips.erase(chip);

		//削除成功

		bool removed = prevSize != newSize;
		if (removed)
		{
			SafeRelease(chip);

#if __CULLING_2D__
			auto layer = (CoreLayer2D_Imp*)(m_objectInfo.GetLayer());
			if (layer != nullptr)
			{
				auto chip_Imp = (Chip2D_Imp*)chip;
				layer->RemoveChipCullingObject(chip_Imp);
			}
#endif
		}
		return removed;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreMapObject2D_Imp::Clear()
	{
#if __CULLING_2D__
		auto layer = (CoreLayer2D_Imp*)(m_objectInfo.GetLayer());
		if (layer != nullptr)
		{
			for (auto chip : m_chips)
			{
				auto chip_Imp = (Chip2D_Imp*)chip;
				layer->RemoveChipCullingObject(chip_Imp);
			}
		}
#endif
		for (auto chip : m_chips)
		{
			SafeRelease(chip);
		}
		m_chips.clear();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreMapObject2D_Imp::Draw(Renderer2D* renderer)
	{
		if (!m_objectInfo.GetIsDrawn())
		{
			return;
		}

		auto parentMatrix = m_transform.GetParentsMatrix();
		auto matrix = m_transform.GetMatrixToTransform();

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
					auto result = parentMatrix * matrix * v3;
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

			renderer->AddSprite(position.data(), color.data(), uvs.data(), texture, (*chip)->GetAlphaBlendMode(), m_drawingPtiority);
		}

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void CoreMapObject2D_Imp::DrawChip(Renderer2D* renderer, Chip2D* chip)
	{

		Texture2D* texture = chip->GetTexture();

		if (m_chips.find(chip) == m_chips.end() || texture == nullptr) return;


		auto parentMatrix = m_transform.GetParentsMatrix();
		auto matrix = m_transform.GetMatrixToTransform();

		std::array<Vector2DF, 4> position = chip->GetSrc().GetVertexes();

		{

			for (auto& pos : position)
			{
				pos -= m_centerPosition;
				auto v3 = Vector3DF(pos.X, pos.Y, 1);
				auto result = parentMatrix * matrix * v3;
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

			if (chip->GetTurnLR())
			{
				std::swap(uvs.at(0), uvs.at(1));
				std::swap(uvs.at(2), uvs.at(3));
			}

			if (chip->GetTurnUL())
			{
				std::swap(uvs.at(0), uvs.at(3));
				std::swap(uvs.at(1), uvs.at(2));
			}
		}

		std::array<Color, 4> color;
		color[0] = chip->GetColor();
		color[1] = chip->GetColor();
		color[2] = chip->GetColor();
		color[3] = chip->GetColor();

		renderer->AddSprite(position.data(), color.data(), uvs.data(), texture, chip->GetAlphaBlendMode(), m_drawingPtiority);

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	culling2d::Circle CoreMapObject2D_Imp::GetChipBoundingCircle(Chip2D* chip)
	{
		culling2d::Circle circle = culling2d::Circle();

		Texture2D* texture = chip->GetTexture();

		auto parentMatrix = m_transform.GetParentsMatrix();
		auto matrix = m_transform.GetMatrixToTransform();

		std::array<Vector2DF, 4> position = chip->GetSrc().GetVertexes();

		{

			for (auto& pos : position)
			{
				pos -= m_centerPosition;
				auto v3 = Vector3DF(pos.X, pos.Y, 1);
				auto result = parentMatrix * matrix * v3;
				pos = Vector2DF(result.X, result.Y);
			}

		}

		Vector2DF center = (position[0] + position[1] + position[2] + position[3]) / 4;
		float len = (center - position[0]).GetLength();
		culling2d::Vector2DF cent = culling2d::Vector2DF(center.X, center.Y);
		circle.Position = cent;
		circle.Radius = len;

		return circle;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
#if __CULLING_2D__
	void CoreMapObject2D_Imp::RegisterObjectToCulling()
	{
		auto layer = (CoreLayer2D_Imp*)m_objectInfo.GetLayer();

		if (layer != nullptr)
		{
			for (auto chip : m_chips)
			{
				auto chip_Imp = (Chip2D_Imp*)chip;

				layer->AddChipCullingObject(chip_Imp, firstSortedKey);
			}
		}
	}

	uint32_t CoreMapObject2D_Imp::GetFirstSortedKey()
	{
		return firstSortedKey;
	}

	void CoreMapObject2D_Imp::SetFirstSortedKey(uint32_t sortedKey)
	{
		firstSortedKey = sortedKey;
	}

#endif
}

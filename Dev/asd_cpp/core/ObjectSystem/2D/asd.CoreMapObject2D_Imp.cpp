#include "../common/asd.common.Base.h"
#include "asd.CoreMapObject2D_Imp.h"
#include "asd.CoreChip2D_Imp.h"
#include "../../Graphics/Resource/asd.Texture2D_Imp.h"
#include "../2D/asd.CoreLayer2D_Imp.h"
#include "../2D/asd.Culling2D.h"

namespace asd
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
	bool CoreMapObject2D_Imp::AddChip(CoreChip2D* chip)
	{
		if (chip == nullptr)
		{
			return false;
		}

		auto chip_Imp = (CoreChip2D_Imp*)chip;

		auto pair = m_chips.insert(chip_Imp);

		if (pair.second)
		{
			SafeAddRef(chip);
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
	bool CoreMapObject2D_Imp::RemoveChip(CoreChip2D* chip)
	{
		if (chip == nullptr)
		{
			return false;
		}

		auto chip_Imp = (CoreChip2D_Imp*)chip;

		auto prevSize = m_chips.size();
		auto newSize = m_chips.erase(chip_Imp);

		//削除成功

		bool removed = prevSize != newSize;
		if (removed)
		{
			SafeRelease(chip);

#if __CULLING_2D__
			auto layer = (CoreLayer2D_Imp*)(m_objectInfo.GetLayer());
			if (layer != nullptr)
			{
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
				auto chip_Imp = (CoreChip2D_Imp*)chip;
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
	void CoreMapObject2D_Imp::DrawChipInternal(CoreChip2D_Imp* chip, Matrix33& matrix, Matrix33& parentMatrix, Renderer2D* renderer)
	{
		auto src = chip->GetSrc();
		auto texture = chip->GetTexture();

		std::array<Vector2DF, 4> position = src.GetVertexes();

		{
			Vector2DF origin = position[0];
			for (int i = 0; i < 4; ++i)
			{
				position[i] -= origin;
			}
		}

		auto textureSize = texture != nullptr ? texture->GetSize() : Vector2DI(1, 1);

		for (auto& pos : position)
		{
			pos -= m_centerPosition;
			auto v3 = Vector3DF(pos.X, pos.Y, 1);
			auto result = parentMatrix * matrix * chip->GetTransformInfo2D().GetMatrixToTransform() * v3;
			pos = Vector2DF(result.X, result.Y);
		}

		Color color[4];
		color[0] = chip->GetColor();
		color[1] = chip->GetColor();
		color[2] = chip->GetColor();
		color[3] = chip->GetColor();

		std::array<Vector2DF, 4> uvs;

		if (src.X < 0 || src.Y < 0 || src.Width < 0 || src.Height < 0)
		{
			uvs[0] = Vector2DF(0, 0);
			uvs[1] = Vector2DF(textureSize.X, 0);
			uvs[2] = Vector2DF(textureSize.X, textureSize.Y);
			uvs[3] = Vector2DF(0, textureSize.Y);
		}
		else
		{
			uvs = src.GetVertexes();
		}

		{
			auto size = Vector2DF(textureSize.X, textureSize.Y);
			for (int i = 0; i < 4; ++i)
			{
				uvs[i] /= size;
			}
		}

		if (chip->GetTurnLR())
		{
			std::swap(uvs[0], uvs[1]);
			std::swap(uvs[2], uvs[3]);
		}

		if (chip->GetTurnUL())
		{
			std::swap(uvs[0], uvs[3]);
			std::swap(uvs[1], uvs[2]);
		}

		renderer->AddSprite(
			position.data(),
			color,
			uvs.data(),
			texture.get(),
			chip->GetAlphaBlendMode(),
			m_drawingPtiority,
			chip->GetTextureFilterType());
	}

	void CoreMapObject2D_Imp::Culling(const std::vector<culling2d::Object*>& culledObjects)
	{
		renderedChips.clear();

		for (auto& culledObj : culledObjects)
		{
			auto userData = (Culling2DUserData*)(culledObj->GetUserData());

			if (userData->Object->GetObjectType() == Object2DType::Map)
			{
				auto chip = userData->Chip;
				renderedChips.push_back(chip);
			}
		}

		std::sort(
			renderedChips.begin(), 
			renderedChips.end(), 
			[](CoreChip2D* a, CoreChip2D* b) { return b->GetDrawingPriority() < a->GetDrawingPriority(); });
	}

	void CoreMapObject2D_Imp::Draw(Renderer2D* renderer)
	{
		if (!m_objectInfo.GetIsDrawn())
		{
			return;
		}

		auto parentMatrix = GetParentsMatrix();
		auto matrix = GetMatrixToTransform();

		for (auto chip : renderedChips)
		{
			auto c = (CoreChip2D_Imp*)chip;

			DrawChipInternal(c, matrix, parentMatrix, renderer);
		}

	}

	void CoreMapObject2D_Imp::DrawChip(Renderer2D* renderer, CoreChip2D* chip)
	{
		if (!m_objectInfo.GetIsDrawn())
		{
			return;
		}

		auto chip_Imp = (CoreChip2D_Imp*)chip;

		auto texture = chip_Imp->GetTexture();

		assert(m_chips.find(chip_Imp) != m_chips.end());

		auto parentMatrix = GetParentsMatrix();
		auto matrix = GetMatrixToTransform();

		DrawChipInternal(chip_Imp, matrix, parentMatrix, renderer);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	culling2d::Circle CoreMapObject2D_Imp::GetChipBoundingCircle(CoreChip2D* chip)
	{
		culling2d::Circle circle = culling2d::Circle();

		auto parentMatrix = GetParentsMatrix();
		auto matrix = GetMatrixToTransform();

		auto chip_Imp = (CoreChip2D_Imp*)chip;

		auto src = chip_Imp->GetSrc();
		auto texture = chip_Imp->GetTexture();

		std::array<Vector2DF, 4> position = src.GetVertexes();

		{
			Vector2DF origin = position[0];
			for (int i = 0; i < 4; ++i)
			{
				position[i] -= origin;
			}
		}

		auto textureSize = texture != nullptr ? texture->GetSize() : Vector2DI(1, 1);

		for (auto& pos : position)
		{
			pos -= m_centerPosition;
			auto v3 = Vector3DF(pos.X, pos.Y, 1);
			auto result = parentMatrix * matrix * chip_Imp->GetTransformInfo2D().GetMatrixToTransform() * v3;
			pos = Vector2DF(result.X, result.Y);
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
				auto chip_Imp = (CoreChip2D_Imp*)chip;

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

#include "ace.MapObject2D.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	MapObject2D::MapObject2D()
		: m_coreObject(nullptr)
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateMapObject2D());
	}

	MapObject2D::~MapObject2D()
	{
	}

	CoreObject2D* MapObject2D::GetCoreObject() const
	{
		return m_coreObject.get();
	}

	const Vector2DF MapObject2D::GetCenterPosition() const
	{
		return m_coreObject->GetCenterPosition();
	}

	void MapObject2D::SetCenterPosition(Vector2DF position)
	{
		m_coreObject->SetCenterPosition(position);
	}

	const int MapObject2D::GetDrawingPriority() const
	{
		return m_coreObject->GetDrawingPriority();
	}

	void MapObject2D::SetDrawingPriority(int priority)
	{
		m_coreObject->SetDrawingPriority(priority);
	}

	const bool MapObject2D::AddChip(std::shared_ptr<Chip2D> chip)
	{
		m_chips.insert(chip);
		return m_coreObject->AddChip(chip.get());
	}

	const bool MapObject2D::RemoveChip(std::shared_ptr<Chip2D> chip)
	{
		m_chips.erase(chip);
		return m_coreObject->RemoveChip(chip.get());
	}
}
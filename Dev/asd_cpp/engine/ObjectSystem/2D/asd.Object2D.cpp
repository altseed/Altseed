#include <list>
#include "asd.Object2D.h"
#include "asd.Layer2D.h"
#include "../Registration/asd.EventToDisposeContent.h"
#define IS_INHERITED(obj, flag) (obj->m_parentInfo != nullptr && (obj->m_parentInfo->GetManagementMode() & ChildManagementMode::flag) != 0)
using namespace std;

namespace asd
{
	Object2D::Object2D()
		: m_owner(nullptr)
		, m_children(list<Object2D::Ptr>())
		, m_componentManager(std::make_shared<ComponentManager<Object2DComponent>>(this))
		, m_isUpdated(true)
		, m_isDrawn(true)
		, m_updatePriority(0)
		, m_onUpdatePriorityChanged(nullptr)
	{
	}

	Object2D::~Object2D()
	{
	}


	void Object2D::RaiseOnAdded()
	{
		OnAdded();
		for (auto& child : m_children)
		{
			if (IS_INHERITED(child, RegistrationToLayer))
			{
				GetLayer()->AddObject(child);
			}
		}
	}

	void Object2D::RaiseOnRemoved()
	{
		for (auto& child : m_children)
		{
			if (IS_INHERITED(child, RegistrationToLayer))
			{
				GetLayer()->RemoveObject(child);
			}
		}
		OnRemoved();
	}

	void Object2D::Dispose()
	{
		Engine::m_changesToCommit.push(make_shared<EventToDisposeContent>(shared_from_this()));
	}

	void Object2D::DisposeImmediately()
	{
		if (GetIsAlive())
		{
			GetCoreObject()->SetIsAlive(false);
			OnDispose();
			for (auto& child : m_children)
			{
				GetCoreObject()->RemoveChild(child->GetCoreObject());
				if (IS_INHERITED(child, Disposal))
				{
					child->Dispose();
				}
				child->m_parentInfo.reset();
			}
			auto parent = this->GetParent();
			if (parent != nullptr && parent->GetIsAlive())
			{
				parent->RemoveChild(shared_from_this());
			}
			if (m_owner != nullptr)
			{
				m_owner->ImmediatelyRemoveObject(shared_from_this(), false);
			}
		}
	}

	void Object2D::Update()
	{
		if (!GetIsAlive() || !GetAbsoluteBeingUpdated())
		{
			return;
		}

		OnUpdate();

		for (auto& component : m_componentManager->GetComponents())
		{
			component.second->Update();
		}
	}

	void Object2D::DrawAdditionally()
	{
		if (GetIsAlive() && GetAbsoluteBeingDrawn())
		{
			OnDrawAdditionally();
		}
	}


	void Object2D::OnAdded()
	{
	}

	void Object2D::OnRemoved()
	{
	}

	void Object2D::OnDispose()
	{
	}

	void Object2D::OnUpdate()
	{
	}

	void Object2D::OnDrawAdditionally()
	{
	}


	void Object2D::DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
		std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawSpriteAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, texture, alphaBlend, priority);
	}

	void Object2D::DrawSpriteWithMaterialAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
		std::shared_ptr<Material2D> material, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawSpriteWithMaterialAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, material, alphaBlend, priority);

	}

	void Object2D::DrawTextAdditionally(Vector2DF pos, Color color, std::shared_ptr<Font> font, const achar* text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawTextAdditionally(pos, color, font, text, writingDirection, alphaBlend, priority);
	}

	void Object2D::DrawRectangleAdditionally(RectF drawingArea, Color color, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawRectangleAdditionally(drawingArea, color, uv, texture, alphaBlend, priority);
	}

	void Object2D::DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawRotatedRectangleAdditionally(drawingArea, color, rotationCenter, angle, uv, texture, alphaBlend, priority);
	}

	void Object2D::DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawTriangleAdditionally(position1, position2, position3, color, uv1, uv2, uv3, texture, alphaBlend, priority);
	}

	void Object2D::DrawCircleAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawCircleAdditionally(center, outerDiameter, innerDiameter, color, vertNum, angle, texture, alphaBlend, priority);
	}

	void Object2D::DrawArcAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawArcAdditionally(center, outerDiameter, innerDiameter, color, vertNum, startingVerticalAngle, endingVerticalAngle, angle, texture, alphaBlend, priority);
	}

	void Object2D::DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawLineAdditionally(point1, point2, thickness, color, alphaBlend, priority);
	}

	void Object2D::DrawShapeAdditionally(std::shared_ptr<Shape> shape, Color color, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawShapeAdditionally(shape, color, texture, alphaBlend, priority);
	}


	bool Object2D::GetIsAlive() const
	{
		return GetCoreObject()->GetIsAlive();
	}

	Layer2D* Object2D::GetLayer() const
	{
		return m_owner;
	}

	void Object2D::SetLayer(Layer2D* layer)
	{
		m_owner = layer;
	}


	void Object2D::AddChild(const Object2D::Ptr& child, ChildManagementMode::Flags managementMode, ChildTransformingMode transformingMode)
	{
		GetCoreObject()->AddChild((child->GetCoreObject()), managementMode, transformingMode);
		m_children.push_back(child);
		child->m_parentInfo = make_shared<ParentInfo2D>(this, managementMode);

		if ((managementMode & ChildManagementMode::RegistrationToLayer) != 0)
		{
			auto childLayer = child->GetLayer();
			if (childLayer != GetLayer() && childLayer != nullptr)
			{
				childLayer->RemoveObject(child);
			}
			if (m_owner != nullptr)
			{
				m_owner->AddObject(child);
			}
		}
	}

	void Object2D::RemoveChild(const Object2D::Ptr& child)
	{
		GetCoreObject()->RemoveChild((child->GetCoreObject()));
		m_children.remove(child);
		child->m_parentInfo.reset();
	}

	Object2D* Object2D::GetParent() const
	{
		return m_parentInfo != nullptr ? m_parentInfo->GetParent() : nullptr;
	}

	const std::list<Object2D::Ptr>& Object2D::GetChildren() const
	{
		return m_children;
	}

	void Object2D::AddComponent(const Object2DComponent::Ptr& component, astring key)
	{
		m_componentManager->Add(component, key);
	}

	Object2DComponent::Ptr Object2D::GetComponent(astring key)
	{
		return m_componentManager->Get(key);
	}

	bool Object2D::RemoveComponent(astring key)
	{
		return m_componentManager->Remove(key);
	}

	void Object2D::ImmediatelyRemoveComponent(astring key)
	{
		m_componentManager->ImmediatelyRemoveComponent(key);
	}

	void Object2D::Register(const Object2DComponent::Ptr& component)
	{
		component->SetOwner(this);
	}

	void Object2D::Unregister(const Object2DComponent::Ptr& component)
	{
		component->SetOwner(nullptr);
	}

#pragma region Get/Set

	int32_t Object2D::GetCameraGroup() const
	{
		return GetCoreObject()->GetCameraGroup();
	}

	void Object2D::SetCameraGroup(int32_t group)
	{
		GetCoreObject()->SetCameraGroup(group);
	}

	bool Object2D::GetIsUpdated() const
	{
		return m_isUpdated;
	}

	void Object2D::SetIsUpdated(bool value)
	{
		m_isUpdated = value;
	}

	bool Object2D::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void Object2D::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
		GetCoreObject()->SetIsDrawn(value);
	}

	Vector2DF Object2D::GetPosition() const
	{
		return GetCoreObject()->GetPosition();
	}

	void Object2D::SetPosition(Vector2DF value)
	{
		GetCoreObject()->SetPosition(value);
	}

	Vector2DF Object2D::GetGlobalPosition()
	{
		return GetCoreObject()->GetGlobalPosition();
	}

	float Object2D::GetAngle() const
	{
		return GetCoreObject()->GetAngle();
	}

	void Object2D::SetAngle(float value)
	{
		GetCoreObject()->SetAngle(value);
	}

	Vector2DF Object2D::GetScale() const
	{
		return GetCoreObject()->GetScale();
	}

	void Object2D::SetScale(Vector2DF value)
	{
		GetCoreObject()->SetScale(value);
	}

	int Object2D::GetUpdatePriority() const
	{
		return m_updatePriority;
	}

	void Object2D::SetUpdatePriority(int value)
	{
		m_updatePriority = value;
		if (m_onUpdatePriorityChanged != nullptr)
		{
			m_onUpdatePriorityChanged(value);
		}
	}

	bool Object2D::GetAbsoluteBeingUpdated() const
	{
		return m_isUpdated
			&& !(IS_INHERITED(this, IsUpdated)
			&& !m_parentInfo->GetParent()->GetAbsoluteBeingUpdated());
	}

	bool Object2D::GetAbsoluteBeingDrawn() const
	{
		return GetCoreObject()->GetAbsoluteBeingDrawn();
	}
#pragma endregion
}
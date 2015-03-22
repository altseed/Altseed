#include <list>
#include "ace.Object2D.h"
#include "ace.Layer2D.h"
using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Object2D::Object2D()
		: m_owner(nullptr)
		, m_children(list<Object2D::Ptr>())
		, m_componentManager(this)
		, m_isUpdated(true)
		, m_isDrawn(true)
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Object2D::~Object2D()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Object2D::Start()
	{
		OnStart();
	}

	void Object2D::Update()
	{
		if (!m_isUpdated || !GetIsAlive())
		{
			return;
		}

		OnUpdate();
		m_componentManager.Update();
	}

	void Object2D::CallDestroy()
	{
		OnDispose();
	}

	void Object2D::OnStart()
	{
	}

	void Object2D::OnUpdate()
	{
	}

	void Object2D::OnDrawAdditionally()
	{
	}

	void Object2D::OnVanish()
	{
	}

	void Object2D::OnDispose()
	{
	}

	void Object2D::DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
		std::shared_ptr<Texture2D> texture, AlphaBlend alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawSpriteAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, texture, alphaBlend, priority);
	}

	void Object2D::DrawTextAdditionally(Vector2DF pos, Color color, std::shared_ptr<Font> font, const achar* text, WritingDirection writingDirection, AlphaBlend alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawTextAdditionally(pos, color, font, text, writingDirection, alphaBlend, priority);
	}

	void Object2D::DrawRectangleAdditionally(RectF drawingArea, Color color, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlend alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawRectangleAdditionally(drawingArea, color, uv, texture, alphaBlend, priority);
	}

	void Object2D::DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlend alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawRotatedRectangleAdditionally(drawingArea, color, rotationCenter, angle, uv, texture, alphaBlend, priority);
	}

	void Object2D::DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, std::shared_ptr<Texture2D> texture, AlphaBlend alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawTriangleAdditionally(position1, position2, position3, color, uv1, uv2, uv3, texture, alphaBlend, priority);
	}

	void Object2D::DrawCircleAdditionally(ace::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, std::shared_ptr<Texture2D> texture, AlphaBlend alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawCircleAdditionally(center, outerDiameter, innerDiameter, color, vertNum, angle, texture, alphaBlend, priority);
	}

	void Object2D::DrawArcAdditionally(ace::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, std::shared_ptr<Texture2D> texture, AlphaBlend alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawArcAdditionally(center, outerDiameter, innerDiameter, color, vertNum, startingVerticalAngle, endingVerticalAngle, angle, texture, alphaBlend, priority);
	}

	void Object2D::DrawLineAdditionally(Vector2DF point1, Vector2DF point2, Color color, float thickness, AlphaBlend alphaBlend, int32_t priority)
	{
		auto layer = GetLayer();
		layer->DrawLineAdditionally(point1, point2, color, thickness, alphaBlend, priority);
	}

	bool Object2D::GetIsAlive() const
	{
		return GetCoreObject()->GetIsAlive();
	}

	void Object2D::Vanish()
	{
		GetCoreObject()->SetIsAlive(false);
		OnVanish();
	}

	Layer2D* Object2D::GetLayer() const
	{
		return m_owner;
	}

	void Object2D::SetLayer(Layer2D* layer)
	{
		m_owner = layer;
	}

	void Object2D::AddChild(const Object2D::Ptr& child, ChildMode mode)
	{
		GetCoreObject()->AddChild((child->GetCoreObject()), mode);
		m_children.push_back(child);
	}

	void Object2D::RemoveChild(const Object2D::Ptr& child)
	{
		GetCoreObject()->RemoveChild((child->GetCoreObject()));
		m_children.remove(child);
	}

	const std::list<Object2D::Ptr>& Object2D::GetChildren() const
	{
		return m_children;
	}

	void Object2D::AddComponent(const Object2DComponent::Ptr& component, astring key)
	{
		m_componentManager.Add(component, key);
	}

	const Object2DComponent::Ptr& Object2D::GetComponent(astring key)
	{
		return m_componentManager.Get(key);
	}

	bool Object2D::RemoveComponent(astring key)
	{
		return m_componentManager.Remove(key);
	}

#pragma region Get/Set

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

#pragma endregion
}
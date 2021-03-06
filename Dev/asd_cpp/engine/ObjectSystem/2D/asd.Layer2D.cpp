﻿#include <list>
#include <exception>
#include "asd.Layer2D.h"
#include <Utility/asd.Timer.h>
using namespace std;

namespace asd
{
	extern ObjectSystemFactory* g_objectSystemFactory;


	Layer2D::Layer2D()
		: m_coreLayer(nullptr)
		, m_objects(std::make_shared<ObjectManager<Object2D>>(this))
		, m_components(std::make_shared<ComponentManager<Layer2DComponent>>(this))
	{
		m_coreLayer = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateLayer2D());
		m_commonObject = m_coreLayer;
	}

	Layer2D::~Layer2D()
	{
		for (auto& object : m_objects->GetContents())
		{
			object->SetLayer(nullptr);
		}

		collidersMap.clear();
	}

	bool Layer2D::GetIsAlive() const
	{
		return Layer::GetIsAlive();
	}

	void Layer2D::UpdateInternal()
	{
		m_objects->Update();
		for (auto& component : m_components->GetComponents())
		{
			component.second->Update();
		}
	}

	void Layer2D::BeginUpdating()
	{
		m_isUpdatedCurrent = m_isUpdated;
		m_coreLayer->BeginUpdating(m_isUpdatedCurrent);
	}

	void Layer2D::EndUpdateting()
	{
		m_coreLayer->EndUpdating(m_isUpdatedCurrent);
	}

	void Layer2D::DrawAdditionally()
	{
		if (!m_isDrawn)
		{
			return;
		}

		for (auto& object : m_objects->GetContents())
		{
			object->DrawAdditionally();
			object->GetCoreObject()->DrawVisibleCollisionsAdditionally();
		}
		OnDrawAdditionally();
	}

	void Layer2D::DisposeInternal()
	{
		m_objects->Dispose();
		for (auto& component : m_components->GetComponents())
		{
			component.second->RaiseOnDisposed();
		}
	}

	void Layer2D::RaiseOnAdded()
	{
		OnAdded();

		for (auto& component : m_components->GetComponents())
		{
			component.second->RaiseOnAdded();
		}
	}

	void Layer2D::RaiseOnRemoved()
	{
		for (auto& component : m_components->GetComponents())
		{
			component.second->RaiseOnRemoved();
		}

		OnRemoved();
	}


	void Layer2D::DrawSpriteAdditionally(
		const Vector2DF& upperLeftPos, const Vector2DF& upperRightPos, const Vector2DF& lowerRightPos, const Vector2DF& lowerLeftPos,
		const Color& upperLeftCol, const Color& upperRightCol, const Color& lowerRightCol, const Color& lowerLeftCol,
		const Vector2DF& upperLeftUV, const Vector2DF& upperRightUV, const Vector2DF& lowerRightUV, const Vector2DF& lowerLeftUV,
		std::shared_ptr<Texture2D>  texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawSpriteAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawSpriteWithMaterialAdditionally(
		const Vector2DF& upperLeftPos, const Vector2DF& upperRightPos, const Vector2DF& lowerRightPos, const Vector2DF& lowerLeftPos,
		const Color& upperLeftCol, const Color& upperRightCol, const Color& lowerRightCol, const Color& lowerLeftCol,
		const Vector2DF& upperLeftUV, const Vector2DF& upperRightUV, const Vector2DF& lowerRightUV, const Vector2DF& lowerLeftUV,
		std::shared_ptr<Material2D> material, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawSpriteWithMaterialAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, material.get(), alphaBlend, priority);

	}

	void Layer2D::DrawTextAdditionally(Vector2DF pos, Color color, std::shared_ptr<Font> font, const achar* text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawTextAdditionally(
			pos,
			color,
			font.get(),
			text,
			writingDirection,
			alphaBlend,
			priority);
	}

	void Layer2D::DrawRectangleAdditionally(const RectF& drawingArea, const Color& color, const RectF& uv, const std::shared_ptr<Texture2D>& texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer==nullptr) return;
		m_coreLayer->DrawRectangleAdditionally(drawingArea, color, uv, texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawRotatedRectangleAdditionally(drawingArea, color, rotationCenter, angle, uv, texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawTriangleAdditionally(position1, position2, position3, color, uv1, uv2, uv3, texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawCircleAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawCircleAdditionally(center, outerDiameter, innerDiameter, color, vertNum, angle, texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawArcAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawArcAdditionally(center, outerDiameter, innerDiameter, color, vertNum, startingVerticalAngle, endingVerticalAngle, angle, texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawLineAdditionally(point1, point2, thickness, color, alphaBlend, priority);
	}

	void Layer2D::DrawShapeAdditionally(std::shared_ptr<Shape> shape, Color color, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawShapeAdditionally(shape->GetCoreShape().get(), color, texture.get(), alphaBlend, priority);
	}


	void Layer2D::AddObject(const Object2D::Ptr& object)
	{
		m_objects->Add(object);
		m_coreLayer->PrepareToAddObject(object->GetCoreObject());
	}

	void Layer2D::RemoveObject(const Object2D::Ptr& object)
	{
		m_objects->Remove(object, true);
		m_coreLayer->PrepareToRemoveObject(object->GetCoreObject());
	}

	void Layer2D::ImmediatelyRemoveObject(const Object2D::Ptr& object, bool raiseEvent)
	{
		m_objects->ImmediatelyRemoveObject(object, raiseEvent);
	}

	void Layer2D::Register(const Object2D::Ptr& object)
	{
		object->SetLayer(this);
	}

	void Layer2D::Unregister(const Object2D::Ptr& object)
	{
		object->SetLayer(nullptr);
	}

	void Layer2D::AddToCore(const Object2D::Ptr& object)
	{
		m_coreLayer->AddObject(object->GetCoreObject());
	}
	
	void Layer2D::RemoveFromCore(const Object2D::Ptr& object)
	{
		m_coreLayer->RemoveObject(object->GetCoreObject());
	}

	void Layer2D::Clear()
	{
		m_objects->Clear();
	}


	void Layer2D::AddComponent(const Layer2DComponent::Ptr& component, astring key)
	{
		m_components->Add(component, key);
	}

	bool Layer2D::RemoveComponent(astring key)
	{
		return m_components->Remove(key);
	}

	void Layer2D::ImmediatelyRemoveComponent(astring key)
	{
		m_components->ImmediatelyRemoveComponent(key);
	}

	void Layer2D::Register(const Layer2DComponent::Ptr& component)
	{
		component->SetOwner(this);
	}

	void Layer2D::Unregister(const Layer2DComponent::Ptr& component)
	{
		component->SetOwner(nullptr);
	}

	Layer2DComponent::Ptr Layer2D::GetComponent(astring key)
	{
		return m_components->Get(key);
	}


	list<Object2D::Ptr> Layer2D::GetObjects() const
	{
		return m_objects->GetContents();
	}

	int Layer2D::GetObjectCount() const
	{
		return m_objects->GetContents().size();
	}

	bool Layer2D::GetIsDistortionEnabled() const
	{
		return m_coreLayer->GetIsDistortionEnabled();
	}

	void Layer2D::SetIsDistortionEnabled(bool value)
	{
		m_coreLayer->SetIsDistortionEnabled(value);
	}
}

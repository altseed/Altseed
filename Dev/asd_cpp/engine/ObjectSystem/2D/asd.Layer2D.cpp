#include <list>
#include <exception>
#include "asd.Layer2D.h"
#include <Utility/asd.Timer.h>
using namespace std;

namespace asd
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Layer2D::Layer2D()
		: m_coreLayer(nullptr)
		, m_objects()
		, m_components(this)
	{
		m_coreLayer = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateLayer2D());
		m_commonObject = m_coreLayer;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Layer2D::~Layer2D()
	{
		for (auto& object : m_objects.GetContents())
		{
			object->SetLayer(nullptr);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::UpdateInternal()
	{
		m_objects.Update();

		for (auto& vanishing : m_objects.GetVanishingContents())
		{
			RemoveObject(vanishing);
			vanishing->Dispose();
		}
		m_objects.GetVanishingContents().clear();

		m_components.Update();
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

		for (auto& object : m_objects.GetContents())
		{
			object->OnDrawAdditionally();
		}
		OnDrawAdditionally();
	}

	void Layer2D::DisposeInternal()
	{
		for (auto& o : m_objects.GetContents())
		{
			o->Dispose();
		}
	}

	void Layer2D::DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
		Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
		Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
		std::shared_ptr<Texture2D>  texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawSpriteAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, texture.get(), alphaBlend, priority);
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

	void Layer2D::DrawRectangleAdditionally(RectF drawingArea, Color color, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer==nullptr) return;
		m_coreLayer->DrawRectangleAdditionally(drawingArea, color, uv, (texture == nullptr) ? nullptr : texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawRotatedRectangleAdditionally(drawingArea, color, rotationCenter, angle, uv, (texture == nullptr) ? nullptr : texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawTriangleAdditionally(position1, position2, position3, color, uv1, uv2, uv3, (texture == nullptr) ? nullptr : texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawCircleAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawCircleAdditionally(center, outerDiameter, innerDiameter, color, vertNum, angle, (texture==nullptr)?nullptr:texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawArcAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawArcAdditionally(center, outerDiameter, innerDiameter, color, vertNum, startingVerticalAngle, endingVerticalAngle, angle, (texture == nullptr) ? nullptr : texture.get(), alphaBlend, priority);
	}

	void Layer2D::DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawLineAdditionally(point1, point2, thickness, color, alphaBlend, priority);
	}

	void Layer2D::DrawShapeAdditionally(std::shared_ptr<Shape> shape, Color color, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority)
	{
		if (m_coreLayer == nullptr) return;
		m_coreLayer->DrawShapeAdditionally(shape->GetCoreShape().get(), color, (texture == nullptr) ? nullptr : texture.get(), alphaBlend, priority);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::AddObject(const Object2D::Ptr& object)
	{
		ACE_ASSERT(object->GetLayer() == nullptr, "追加しようとしたオブジェクトは、すでに別のレイヤーに所属しています。");
		m_objects.Add(object);
		m_coreLayer->AddObject(object->GetCoreObject());
		object->SetLayer(this);
		object->RaiseOnAdded();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::RemoveObject(const Object2D::Ptr& object)
	{
		m_objects.Remove(object);
		object->RaiseOnRemoved();
		object->SetLayer(nullptr);
		m_coreLayer->RemoveObject(object->GetCoreObject());
	}

	const list<Object2D::Ptr>& Layer2D::GetObjects() const
	{
		return m_objects.GetContents();
	}

	void Layer2D::Clear()
	{
		for (auto object : m_objects.GetContents())
		{
			object->RaiseOnRemoved();
			object->SetLayer(nullptr);
		}
		m_objects.Clear();
		m_coreLayer->Clear();
	}
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::AddComponent(const Layer2DComponent::Ptr& component, astring key)
	{
		m_components.Add(component, key);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	const Layer2DComponent::Ptr& Layer2D::GetComponent(astring key)
	{
		return m_components.Get(key);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Layer2D::RemoveComponent(astring key)
	{
		return m_components.Remove(key);
	}

	int Layer2D::GetObjectCount() const
	{
		return m_objects.GetContents().size();
	}
}

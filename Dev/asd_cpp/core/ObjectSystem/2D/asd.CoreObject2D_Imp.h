
#pragma once

#include "asd.CoreObject2D.h"
#include "asd.CoreLayer2D_Imp.h"
#include "asd.ObjectInfo2D.h"
#include "asd.TransformInfo2D.h"
#include "asd.Culling2D.h"

#include "../../asd.ReferenceObject.h"

#include <unordered_set>
#include <Math/asd.Vector3DF.h>
#include <Box2D/Box2D.h>

#include "Collision/2D/asd.CoreCollision2DEvent.h"
#include "Collision/2D/asd.CoreCollision2DManager.h"
#include "Collision/2D/asd.CoreCollision2D.h"


namespace asd
{
	class CoreCollider2D_Imp;

	class CoreObject2D_Imp
		// : public CoreObject2D 菱形継承防止の為
	{
		friend class CoreCollision2D;
		friend class CoreCollision2DManager;
		friend class CoreLayer2D_Imp;
	private:
		CoreObject2D_Imp* CoreObject2DToImp(CoreObject2D* obj);
		void NotifyTransformToColliders();

	protected:
		std::unordered_set<CoreCollider2D*> colliders;
		std::vector<CoreCollision2DEvent> currentFrameCollisionEvents;

		int32_t			cameraGroup = INT_MAX;

		ObjectInfo2D	m_objectInfo;
		TransformInfo2D m_transform;
		ParentInfo2D::Ptr m_parentInfo;
		Graphics_Imp*	m_graphics;
		culling2d::Circle m_boundingCircle;
		culling2d::Object *cullingObject;
		bool alreadyCullingUpdated;
		std::set<CoreObject2D*> children;

		int32_t			indexInLayer = 0;

		void SetCullingUpdate(CoreObject2D_Imp* obj);

	public:
		CoreObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreObject2D_Imp();

		// カリング系
		virtual void CalculateBoundingCircle(){}

		void SetCullingObject(culling2d::Object *cullingObj)
		{
			cullingObject = cullingObj;
		}

		culling2d::Object* GetCullingObject() const
		{
			return cullingObject;
		}

		bool GetAlreadyCullingUpdated() const
		{
			return alreadyCullingUpdated;
		}

		void SetAlreadyCullingUpdated(bool cullingUpdated)
		{
			alreadyCullingUpdated = cullingUpdated;
		}

		culling2d::Circle& GetBoundingCircle();

		// システム系
		int32_t GetCameraGroup() const
		{
			return cameraGroup;
		}

		void SetCameraGroup(int32_t group)
		{
			cameraGroup = group;
		}

		bool GetIsDrawn() const
		{
			return m_objectInfo.GetIsDrawn();
		}
		void SetIsDrawn(bool value)
		{
			m_objectInfo.SetIsDrawn(value);
		}

		bool GetIsAlive() const
		{
			return m_objectInfo.GetIsAlive();
		}

		void SetIsAlive(bool value)
		{
			if (!value&&m_objectInfo.GetLayer() != nullptr)
			{
				auto layerImp = (CoreLayer2D_Imp*)m_objectInfo.GetLayer();
				layerImp->RemoveTransformedObject(cullingObject);
			}
			m_objectInfo.SetIsAlive(value);
		}

		// レイヤー処理系
		int32_t GetIndexInLayer() { return indexInLayer; }

		void SetIndexInLayer(int32_t value) { indexInLayer = value; }

		// オブジェクトシステム系
		void SetLayer(CoreLayer2D* layer)
		{
			m_objectInfo.SetLayer(layer);
		}
	public:

		CoreLayer2D* GetLayer()
		{
			return m_objectInfo.GetLayer();
		}

		void SetParent(CoreObject2D& parent, ChildManagementMode::Flags managementMode, ChildTransformingMode transformingMode)
		{
			m_parentInfo = std::make_shared<ParentInfo2D>(&parent, managementMode, transformingMode);
		}

		void ClearParent()
		{
			m_parentInfo = nullptr;
		}

		const ParentInfo2D* GetParentInfo() const
		{
			return m_parentInfo.get();
		}

		virtual void OnAdding(Renderer2D* renderer) {}

		virtual void OnRemoving(Renderer2D* renderer) {}

		// 変形系
		Vector2DF GetPosition() const
		{
			return m_transform.GetPosition();
		}
		void SetPosition(Vector2DF value)
		{
			m_transform.SetPosition(value);
			SetCullingUpdate(this);
			NotifyTransformToColliders();
		}
		Vector2DF GetGlobalPosition()
		{
			return GetAbsolutePosition();
		}

		float GetAngle() const
		{
			return m_transform.GetAngle();
		}
		void SetAngle(float value)
		{
			m_transform.SetAngle(value);
			SetCullingUpdate(this);
			NotifyTransformToColliders();
		}

		Vector2DF GetScale() const
		{
			return m_transform.GetScale();
		}
		void SetScale(Vector2DF value)
		{
			m_transform.SetScale(value);
			SetCullingUpdate(this);
			NotifyTransformToColliders();
		}

		Matrix33 GetMatrixToTranslate()
		{
			return m_transform.GetMatrixToTranslate();
		}
		Matrix33 GetMatrixToTransform()
		{
			return m_transform.GetMatrixToTransform();
		}
		Matrix33 GetParentsMatrix()
		{
			if (m_parentInfo != nullptr)
			{
				return m_parentInfo->GetInheritedMatrixToTransform();
			}
			else
			{
				return Matrix33();
			}
		}

		// 親子関係系
		Vector2DF GetAbsolutePosition();
		Matrix33 GetAbsoluteMatrixToTranslate();
		Matrix33 GetAbsoluteMatrixToTransform();
		bool GetAbsoluteBeingDrawn() const;

		void AddCollider(CoreCollider2D *collider);

		void RemoveCollider(CoreCollider2D *collider);

		CoreCollision2DEvent* GetCollisionEvent(int n);

		int GetCollisionEventNum();

		void DrawVisibleCollisionsAdditionally();
	};
}

#pragma once

#include "asd.CoreObject2D.h"
#include "asd.CoreLayer2D_Imp.h"
#include "asd.ObjectInfo2D.h"
#include "asd.TransformInfo2D.h"
#include "asd.Culling2D.h"

#include "../../asd.ReferenceObject.h"

#include <Math/asd.Vector3DF.h>

namespace asd
{
	class CoreObject2D_Imp
		// : public CoreObject2D 菱形継承防止の為
	{
	private:
		CoreObject2D_Imp* CoreObject2DToImp(CoreObject2D* obj);
	protected:

		int32_t			cameraGroup = INT_MAX;

		ObjectInfo2D	m_objectInfo;
		TransformInfo2D m_transform;
		ParentInfo2D::Ptr m_parentInfo;
		Graphics_Imp*	m_graphics;
		culling2d::Circle m_boundingCircle;
		culling2d::Object *cullingObject;
		bool alreadyCullingUpdated;
		std::set<CoreObject2D*> children;

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

		// オブジェクトシステム系
		void SetLayer(CoreLayer2D* layer)
		{
			m_objectInfo.SetLayer(layer);
		}

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

		virtual void OnAdded(Renderer2D* renderer) {}

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
		}

		Vector2DF GetScale() const
		{
			return m_transform.GetScale();
		}
		void SetScale(Vector2DF value)
		{
			m_transform.SetScale(value);
			SetCullingUpdate(this);
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
	};
}
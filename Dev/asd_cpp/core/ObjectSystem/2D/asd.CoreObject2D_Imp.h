
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

		virtual void OnAdded(Renderer2D* renderer) {}

		virtual void OnRemoving(Renderer2D* renderer) {}


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
			auto vec2 = GetPosition();
			auto vec3 = Vector3DF(vec2.X, vec2.Y, 1);
			auto result = GetParentsMatrix() * vec3;
			return Vector2DF(result.X, result.Y);
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
				return m_parentInfo->GetInheritedMatrix();
			}
			else
			{
				return Matrix33();
			}
		}
	};
}
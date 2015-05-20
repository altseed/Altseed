
#pragma once

#include "ace.CoreObject2D.h"
#include "ace.CoreLayer2D_Imp.h"
#include "ace.ObjectInfo2D.h"
#include "ace.TransformInfo2D.h"
#include "ace.Culling2D.h"

#include "../../ace.ReferenceObject.h"

#include <Math/ace.Vector3DF.h>

namespace ace
{
	class CoreObject2D_Imp
		// : public CoreObject2D 菱形継承防止の為
	{
	private:
		CoreObject2D_Imp* CoreObject2DToImp(CoreObject2D* obj);
	protected:
		ObjectInfo2D	m_objectInfo;
		TransformInfo2D m_transform;
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
			m_objectInfo.SetIsAlive(value);
		}

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
			return m_transform.GetGlobalPosition();
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

		void SetLayer(CoreLayer2D* layer)
		{
			m_objectInfo.SetLayer(layer);
		}

		CoreLayer2D* GetLayer()
		{
			return m_objectInfo.GetLayer();
		}

		void SetParent(CoreObject2D& parent, ChildMode mode)
		{
			m_transform.SetParent(parent, mode);
		}

		void ClearParent()
		{
			m_transform.ClearParent();
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
			return m_transform.GetParentsMatrix();
		}

		virtual void OnAdded(Renderer2D* renderer) {}

		virtual void OnRemoving(Renderer2D* renderer) {}

		culling2d::Circle& GetBoundingCircle();
	};
}
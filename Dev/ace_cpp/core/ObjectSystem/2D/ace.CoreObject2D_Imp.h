#pragma once
#include "ace.CoreLayer2D.h"
#include "ace.CoreObject2D.h"
#include "ace.TransformInfo2D.h"
#include "../common/Math/ace.Vector2DF.h"

namespace ace
{
	class CoreObject2D_Imp :
		public CoreObject2D,
		public ReferenceObject
	{
	private:
		TransformInfo2D m_transform;
		CoreLayer2D* m_layer;

	public:
		CoreObject2D_Imp();
		virtual ~CoreObject2D_Imp()
		{
		}

		Vector2DF GetPosition() const
		{
			return m_transform.GetPosition();
		}

		void SetPosition(Vector2DF value)
		{
			m_transform.SetPosition(value);
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
		}

		Vector2DF GetScale() const
		{
			return m_transform.GetScale();
		}

		void SetScale(Vector2DF value)
		{
			m_transform.SetScale(value);
		}

		void AddChild(ICoreObject2D& child, eChildMode mode);
		void RemoveChild(ICoreObject2D& child);

#if !SWIG
		virtual void Draw();

		void SetLayer(CoreLayer2D* layer);
		void SetParent(ICoreObject2D& parent, eChildMode mode)
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

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}
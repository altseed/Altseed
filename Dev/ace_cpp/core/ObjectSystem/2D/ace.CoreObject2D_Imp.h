
#pragma once

#include "ace.CoreObject2D.h"
#include "ace.ObjectInfo2D.h"
#include "ace.TransformInfo2D.h"

#include "../../ace.ReferenceObject.h"

#include <Math/ace.Vector3DF.h>

#define CORE_OBJECT2D_IMP_COMMON	\
	int AddRef() { return ReferenceObject::AddRef(); }		\
	int Release() { return ReferenceObject::Release(); }	\
	int GetRef() { return ReferenceObject::GetRef(); }		\
	virtual bool GetIsDrawn() const override { return CoreObject2D_Imp::GetIsDrawn(); }\
	virtual void SetIsDrawn(bool value) override { return CoreObject2D_Imp::SetIsDrawn(value); }\
	virtual bool GetIsAlive() const override { return CoreObject2D_Imp::GetIsAlive(); }\
	virtual void SetIsAlive(bool value) override { return CoreObject2D_Imp::SetIsAlive(value); }\
	virtual void SetLayer(CoreLayer2D* layer) override { return CoreObject2D_Imp::SetLayer(layer); }\
	\



#define CORE_OBJECT2D_IMP_TRANSFORM	\
	virtual Vector2DF GetPosition() const override { return CoreObject2D_Imp::GetPosition(); }\
	virtual void SetPosition(Vector2DF value) override { return CoreObject2D_Imp::SetPosition(value); }\
	virtual Vector2DF GetGlobalPosition() override { return CoreObject2D_Imp::GetGlobalPosition(); }\
	virtual float GetAngle() const override { return CoreObject2D_Imp::GetAngle(); }\
	virtual void SetAngle(float value) override { return CoreObject2D_Imp::SetAngle(value); }\
	virtual Vector2DF GetScale() const override { return CoreObject2D_Imp::GetScale(); }\
	virtual void SetScale(Vector2DF value) override { return CoreObject2D_Imp::SetScale(value); }\
	virtual void SetParent(CoreObject2D& parent, eChildMode mode) override { return CoreObject2D_Imp::SetParent(parent,mode); }\
	virtual void ClearParent() override { return CoreObject2D_Imp::ClearParent(); }\
	virtual Matrix33 GetMatrixToTranslate() override { return CoreObject2D_Imp::GetMatrixToTranslate(); }\
	virtual Matrix33 GetMatrixToTransform() override { return CoreObject2D_Imp::GetMatrixToTransform(); }\
	\


#define CORE_OBJECT2D_IMP_CHILD	\
	void AddChild(CoreObject2D& child, eChildMode mode) {	\
	child.SetParent(*this, mode); \
	} \
	\
	void RemoveChild(CoreObject2D& child) { \
	child.ClearParent(); \
	} \


namespace ace
{
	class CoreObject2D_Imp
		// : public CoreObject2D 菱形継承防止の為
	{
	private:

	protected:
		ObjectInfo2D	m_objectInfo;
		TransformInfo2D m_transform;
		Graphics_Imp*	m_graphics;
	public:
		CoreObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreObject2D_Imp();

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

		void SetLayer(CoreLayer2D* layer)
		{
			m_objectInfo.SetLayer(layer);
		}

		void SetParent(CoreObject2D& parent, eChildMode mode)
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
	};
}
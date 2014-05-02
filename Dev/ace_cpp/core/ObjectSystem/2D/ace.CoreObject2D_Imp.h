
#pragma once

#include "ace.CoreObject2D.h"
#include "ace.ObjectInfo2D.h"

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
	\


namespace ace
{
	class CoreObject2D_Imp
		// : public CoreObject2D ïHå`åpè≥ñhé~ÇÃà◊
	{
	private:

	protected:
		ObjectInfo2D	m_objectInfo;
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
	};
}
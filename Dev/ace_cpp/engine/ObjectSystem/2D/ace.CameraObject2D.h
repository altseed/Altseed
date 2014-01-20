#pragma once
#include <memory>
#include "ace.Object2D.h"

namespace ace
{
	class CameraObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreCameraObject2D> m_coreObject;

		CoreObject2D* GetCoreObject() const;

	public:
		CameraObject2D();
		virtual ~CameraObject2D();

		RectI GetSrc() const { return m_coreObject->GetSrc(); }
		void SetSrc(RectI value) { m_coreObject->SetSrc(value); }

		RectI GetDst() const { return m_coreObject->GetDst(); }
		void SetDst(RectI value) { m_coreObject->SetDst(value); }
	};
}
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

		RectI GetSrc() const;
		void SetSrc(RectI value);

		RectI GetDst() const;
		void SetDst(RectI value);
	};
}
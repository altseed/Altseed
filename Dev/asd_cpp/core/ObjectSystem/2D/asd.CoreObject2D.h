#pragma once
#include "../common/Math/asd.Vector2DF.h"
#include "../common/Math/asd.Matrix33.h"
#include "../asd.ChildManagementMode.h"
#include "../asd.ChildTransformingMode.h"
#include "../../Graphics/2D/asd.Renderer2D.h"
#include "asd.CoreLayer2D.h"

namespace asd
{
	class CoreObject2D 
		: public IReference
	{
	public:
		virtual ~CoreObject2D()
		{
		}

		virtual Object2DType GetObjectType() const = 0;

		virtual int32_t GetCameraGroup() const = 0;
		virtual void SetCameraGroup(int32_t group) = 0;

		virtual bool GetIsDrawn() const = 0;
		virtual void SetIsDrawn(bool value) = 0;

		virtual bool GetIsAlive() const = 0;
		virtual void SetIsAlive(bool value) = 0;

		virtual Vector2DF GetPosition() const = 0;
		virtual void SetPosition(Vector2DF position) = 0;
		virtual Vector2DF GetGlobalPosition() = 0;

		virtual float GetAngle() const = 0;
		virtual void SetAngle(float value) = 0;

		virtual Vector2DF GetScale() const = 0;
		virtual void SetScale(Vector2DF value) = 0;

		virtual void AddChild(CoreObject2D* chlid, ChildManagementMode::Flags managementMode, ChildTransformingMode transformingMode) = 0;
		virtual void RemoveChild(CoreObject2D* chlid) = 0;

#if !SWIG
		virtual void Draw(Renderer2D* renderer) = 0;

		virtual void SetLayer(CoreLayer2D* layer) = 0;
		virtual CoreLayer2D* GetLayer() = 0;
		virtual void SetParent(CoreObject2D& parent, ChildManagementMode::Flags managementMode, ChildTransformingMode transformingMode) = 0;
		virtual void ClearParent() = 0;

		virtual Matrix33 GetMatrixToTranslate() = 0;
		virtual Matrix33 GetMatrixToTransform() = 0;
		virtual Matrix33 GetParentsMatrix() = 0;
#endif
	};
}
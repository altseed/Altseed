#pragma once
#include "../common/Math/ace.Vector2DF.h"
#include "../common/Math/ace.Matrix33.h"
#include "../ace.eChildMode.h"
#include "../../Graphics/2D/ace.Renderer2D.h"
#include "ace.CoreLayer2D.h"

namespace ace
{
	class CoreObject2D 
		: public IReference
	{
	public:
		virtual ~CoreObject2D()
		{
		}

		virtual eObject2DType GetObjectType() const = 0;

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

		virtual void AddChild(CoreObject2D& chlid, eChildMode mode) = 0;
		virtual void RemoveChild(CoreObject2D& chlid) = 0;

#if !SWIG
		virtual void Draw(Renderer2D* renderer, Matrix33 cameraMatrix) = 0;

		virtual void SetLayer(CoreLayer2D* layer) = 0;
		virtual void SetParent(CoreObject2D& parent, eChildMode mode) = 0;
		virtual void ClearParent() = 0;

		virtual Matrix33 GetMatrixToTranslate() = 0;
		virtual Matrix33 GetMatrixToTransform() = 0;
#endif
	};
}
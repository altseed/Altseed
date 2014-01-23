#pragma once
#include "ace.CoreCameraObject2D.h"
#include "ace.ObjectInfo2D.h"

namespace ace
{
	class CoreCameraObject2D_Imp
		: public CoreCameraObject2D
		, public ReferenceObject
	{
	private:
		ObjectInfo2D m_objectInfo;

		RectI m_src;
		RectI m_dst;

		Graphics_Imp* m_graphics;
		Renderer2D* m_renderer;
		RenderTexture_Imp* m_renderTarget;

		void ResetBuffer();

	public:
		CoreCameraObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreCameraObject2D_Imp();

		bool GetIsCamera() const
		{
			return true;
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

		RectI GetSrc() const;
		void SetSrc(RectI value);

		RectI GetDst() const;
		void SetDst(RectI value);

		Renderer2D* GetRenderer() const;
		Matrix33 GetCameraMatrix();

		void SetForRenderTarget();
		void FlushToBuffer();
		void DrawBuffer(Renderer2D* renderer);

#pragma region 無効な機能
		Vector2DF GetPosition() const
		{
			throw "CameraObject2D の GetPosition は無効です。";
		}
		void SetPosition(Vector2DF value)
		{
			throw "CameraObject2D の SetPosition は無効です。";
		}
		Vector2DF GetGlobalPosition()
		{
			throw "CameraObject2D の GetGlobalPosition は無効です。";
		}

		float GetAngle() const
		{
			throw "CameraObject2D の GetAngle は無効です。";
		}
		void SetAngle(float value)
		{
			throw "CameraObject2D の SetAngle は無効です。";
		}

		Vector2DF GetScale() const
		{
			throw "CameraObject2D の GetScale は無効です。";
		}
		void SetScale(Vector2DF value)
		{
			throw "CameraObject2D の SetScale は無効です。";
		}

		void AddChild(CoreObject2D& chlid, eChildMode mode)
		{
			throw "CameraObject2D の AddChild は無効です。";
		}
		void RemoveChild(CoreObject2D& chlid)
		{
			throw "CameraObject2D の RemoveChild は無効です。";
		}

		void SetParent(CoreObject2D& parent, eChildMode mode)
		{
			throw "CameraObject2D の SetParent は無効です。";
		}
		void ClearParent()
		{
			throw "CameraObject2D の ClearParent は無効です。";
		}

		Matrix33 GetMatrixToTranslate()
		{
			throw "CameraObject2D の GetMatrixToTranslate は無効です。";
		}
		Matrix33 GetMatrixToTransform()
		{
			throw "CameraObject2D の GetMatrixToTransform は無効です。";
		}
#pragma endregion

#if !SWIG
		void Draw(Renderer2D* renderer, Matrix33 cameraMatrix);

		void SetLayer(CoreLayer2D* layer)
		{
			m_objectInfo.SetLayer(layer);
		}

		int AddRef() { return ReferenceObject::AddRef(); }
		int Release() { return ReferenceObject::Release(); }
		int GetRef() { return ReferenceObject::GetRef(); }
#endif
	};
}
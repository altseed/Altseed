#pragma once
#include "ace.CoreTextureObject2D.h"
#include "ace.TransformInfo2D.h"
#include "ace.ObjectInfo2D.h"

namespace ace
{
	class CoreTextureObject2D_Imp :
		public CoreTextureObject2D,
		public ReferenceObject
	{
	private:
		TransformInfo2D m_transform;
		ObjectInfo2D m_objectInfo;

		Texture2D* m_texture;
		Vector2DF m_centerPosition;
		Color m_color;
		bool m_turnLR;
		bool m_turnUL;
		eAlphaBlend m_alphablend;
		int m_drawingPtiority;

	public:
		CoreTextureObject2D_Imp();
		virtual ~CoreTextureObject2D_Imp();

		bool GetIsCamera() const
		{
			return false;
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

#pragma region Paramater
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


		Texture2D* GetTexture() const;
		void SetTexture(Texture2D* texture);

		Vector2DF GetCenterPosition() const;
		void SetCenterPosition(Vector2DF position);

		Color GetColor() const;
		void SetColor(Color color);

		bool GetTurnLR() const;
		void SetTurnLR(bool turnLR);

		bool GetTurnUL() const;
		void SetTurnUL(bool turnUL);

		int GetDrawingPriority() const;
		void SetDrawingPriority(int priority);

		eAlphaBlend GetAlphaBlendMode() const;
		void SetAlphaBlendMode(eAlphaBlend alphaBlend);
#pragma endregion

		void AddChild(CoreObject2D& child, eChildMode mode);
		void RemoveChild(CoreObject2D& child);

#if !SWIG
		void Draw(Renderer2D* renderer, Matrix33 cameraMatrix);
		
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

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}
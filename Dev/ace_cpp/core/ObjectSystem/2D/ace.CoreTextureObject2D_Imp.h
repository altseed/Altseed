#pragma once
#include "../common/Math/ace.RectF.h"
#include "ace.CoreTextureObject2D.h"
#include "ace.CoreObject2D_Imp.h"

#include "ace.TransformInfo2D.h"

namespace ace
{
	class CoreTextureObject2D_Imp
		: public CoreTextureObject2D
		, public CoreObject2D_Imp
		, public ReferenceObject
	{
	private:
		TransformInfo2D m_transform;
		Texture2D* m_texture;
		RectF m_src;
		Vector2DF m_centerPosition;
		Color m_color;
		bool m_turnLR;
		bool m_turnUL;
		eAlphaBlend m_alphablend;
		int m_drawingPtiority;

	public:
		CoreTextureObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreTextureObject2D_Imp();

		bool GetIsCamera() const
		{
			return false;
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

		RectF GetSrc() const;
		void SetSrc(RectF value);

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
#endif
		CORE_OBJECT2D_IMP_COMMON
	};
}
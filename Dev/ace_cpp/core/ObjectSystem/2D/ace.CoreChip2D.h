#pragma once

#include <ace.common.Base.h>
#include "../../ace.ReferenceObject.h"
#include "ace.TransformInfo2D.h"

namespace ace
{
	/**
	@brief	フォントの情報が記録されているクラス
	*/
	class CoreChip2D
		:public IReference
	{
		friend class Accessor;
	private:

	protected:

		virtual Texture2D* GetTexture_() const = 0;
	public:

		CoreChip2D(){}
		virtual ~CoreChip2D(){}

#if !SWIG
		virtual void SetTexture(std::shared_ptr<Texture2D> texture) = 0;
		virtual std::shared_ptr<Texture2D> GetTexture() = 0;
#endif
		virtual void SetTexture(Texture2D* texture) = 0;

		virtual RectF GetSrc() const = 0;
		virtual void SetSrc(RectF src) = 0;

		virtual float GetAngle() const = 0;
		virtual void SetAngle(float angle) = 0;

		virtual Vector2DF GetScale() const = 0;
		virtual void SetScale(Vector2DF scale) = 0;

		virtual Vector2DF GetPosition() const = 0;
		virtual void SetPosition(Vector2DF position) = 0;

		virtual Vector2DF GetCenterPosition() const = 0;
		virtual void SetCenterPosition(Vector2DF position) = 0;

		virtual Color GetColor() const = 0;
		virtual void SetColor(Color color) = 0;

		virtual bool GetTurnLR() const = 0;
		virtual void SetTurnLR(bool turnLR) = 0;

		virtual bool GetTurnUL() const = 0;
		virtual void SetTurnUL(bool turnUL) = 0;

		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int priority) = 0;

		virtual AlphaBlendMode GetAlphaBlendMode() const = 0;
		virtual void SetAlphaBlendMode(AlphaBlendMode alphaBlend) = 0;

		virtual void SetTextureFilterType(TextureFilterType textureFilterType) = 0;
		virtual TextureFilterType GetTextureFilterType() const = 0;
	};
}

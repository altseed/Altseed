#pragma once

#include <ace.common.Base.h>
#include "../../ace.ReferenceObject.h"

namespace ace
{
	/**
	@brief	フォントの情報が記録されているクラス
	*/
	class Chip2D
		:public IReference
	{
		friend class Accessor;
	private:

	protected:
		Chip2D(){}
		virtual ~Chip2D(){}

		virtual Texture2D* GetTexture_() const = 0;
	public:

#if !SWIG
		virtual void SetTexture(std::shared_ptr<Texture2D> texture) = 0;
		virtual std::shared_ptr<Texture2D> GetTexture() = 0;
#endif
		virtual void SetTexture(Texture2D* texture) = 0;

		virtual RectF GetSrc() const = 0;
		virtual void SetSrc(RectF src) = 0;

		virtual Color GetColor() const = 0;
		virtual void SetColor(Color color) = 0;

		virtual bool GetTurnLR() const = 0;
		virtual void SetTurnLR(bool turnLR) = 0;

		virtual bool GetTurnUL() const = 0;
		virtual void SetTurnUL(bool turnUL) = 0;

		virtual AlphaBlendMode GetAlphaBlendMode() const = 0;
		virtual void SetAlphaBlendMode(AlphaBlendMode alphaBlend) = 0;
	};
}

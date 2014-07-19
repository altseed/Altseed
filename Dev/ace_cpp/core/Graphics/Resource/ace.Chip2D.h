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
	private:

	protected:
		Chip2D(){}
		virtual ~Chip2D(){}
	public:
		virtual Texture2D* GetTexture() const = 0;
		virtual void SetTexture(Texture2D* texture) = 0;

		virtual const RectF GetSrc() const = 0;
		virtual void SetSrc(RectF src) = 0;

		virtual const Color GetColor() const = 0;
		virtual void SetColor(Color color) = 0;

		virtual const bool GetTurnLR() const = 0;
		virtual void SetTurnLR(bool turnLR) = 0;

		virtual const bool GetTurnUL() const = 0;
		virtual void SetTurnUL(bool turnUL) = 0;

		virtual const AlphaBlend GetAlphaBlendMode() const = 0;
		virtual void SetAlphaBlendMode(AlphaBlend alphaBlend) = 0;
	};
};
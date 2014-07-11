#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Texture2D.h"
#include <Math/ace.RectF.h>
#include "ace.Chip2D.h"
#include "../ace.DeviceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Chip2D_Imp
		: public Chip2D
		, public DeviceObject
	{
	protected:

		Graphics *m_graphics;

		Texture2D* m_texture;
		RectF m_src;
		Color m_color;
		bool m_turnLR;
		bool m_turnUL;
		AlphaBlend m_alphaBlend;

	public:

		Texture2D* GetTexture() const override;
		void SetTexture(Texture2D* texture) override;

		const RectF GetSrc() const override;
		void SetSrc(RectF src) override;

		const Color GetColor() const override;
		void SetColor(Color color) override;

		const bool GetTurnLR() const override;
		void SetTurnLR(bool turnLR) override;

		const bool GetTurnUL() const override;
		void SetTurnUL(bool turnUL) override;

		const AlphaBlend GetAlphaBlendMode() const override;
		void SetAlphaBlendMode(AlphaBlend alphaBlend) override;

		Chip2D_Imp(Graphics* graphics);
		virtual ~Chip2D_Imp();

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
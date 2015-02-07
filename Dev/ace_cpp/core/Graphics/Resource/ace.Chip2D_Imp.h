#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Texture2D.h"
#include <Math/ace.RectF.h>
#include "ace.Chip2D.h"
#include "../ace.DeviceObject.h"
#include "../../ObjectSystem/2D/ace.Culling2D.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	class CoreMapObject2D_Imp;
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Chip2D_Imp
		: public Chip2D
		, public DeviceObject
	{
	private:

		Graphics *m_graphics;

		Texture2D* m_texture;
		RectF m_src;
		Color m_color;
		bool m_turnLR;
		bool m_turnUL;
		AlphaBlend m_alphaBlend;

#if __CULLING_2D__
		culling2d::Object *cullingObject;
		bool alreadyCullingUpdated;
#endif
		CoreMapObject2D_Imp* mapObject2D;

		Texture2D* GetTexture_() const;
		void SetTexture_(Texture2D* texture);
	public:

		CoreMapObject2D_Imp* GetMapObject2D() const;
		void SetMapObject2D(CoreMapObject2D_Imp* mapObject);

#if __CULLING_2D__
		bool GetAlreadyCullingUpdated() const;
		void SetAlreadyCullingUpdated(bool cullingUpdated);
#endif

	public:

#if !SWIG
		void SetTexture(std::shared_ptr<Texture2D> texture);
		std::shared_ptr<Texture2D> GetTexture();
#endif


		RectF GetSrc() const override;
		void SetSrc(RectF src) override;

		Color GetColor() const override;
		void SetColor(Color color) override;

		bool GetTurnLR() const override;
		void SetTurnLR(bool turnLR) override;

		bool GetTurnUL() const override;
		void SetTurnUL(bool turnUL) override;

		AlphaBlend GetAlphaBlendMode() const override;
		void SetAlphaBlendMode(AlphaBlend alphaBlend) override;

		Chip2D_Imp(Graphics* graphics);
		virtual ~Chip2D_Imp();

#if __CULLING_2D__
		void SetCullingObject(culling2d::Object *cullingObj);
		culling2d::Object* GetCullingObject() const;

		culling2d::Circle GetBoundingCircle();
#endif

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
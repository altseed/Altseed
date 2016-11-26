#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../Graphics/Resource/asd.Texture2D.h"
#include <Math/asd.RectF.h>
#include "asd.CoreChip2D.h"
#include "../../ObjectSystem/2D/asd.Culling2D.h"
#include "asd.TransformInfo2D.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	class CoreMapObject2D_Imp;
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class CoreChip2D_Imp
		: public CoreChip2D
		, public ReferenceObject
	{
	private:

		Graphics*	m_graphics = nullptr;
		CoreMapObject2D_Imp* mapObject2D = nullptr;
		Texture2D*	m_texture = nullptr;

		RectF m_src;
		Color m_color;
		bool m_turnLR;
		bool m_turnUL;
		AlphaBlendMode m_alphablend;
		TransformInfo2D m_transformInfo;
		Vector2DF m_centerPosition;
		int m_drawingPriority;
		TextureFilterType m_textureFilterType;

#if __CULLING_2D__
		culling2d::Object *cullingObject;
		bool alreadyCullingUpdated;
#endif
		

		Texture2D* GetTexture_() const;
		void SetTexture_(Texture2D* texture);

		void SetCullingUpdate();
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
		TransformInfo2D GetTransformInfo2D() const;
#endif
		void SetTexture(Texture2D* texture);

		RectF GetSrc() const override;
		void SetSrc(RectF src) override;

		Vector2DF GetPosition() const override;
		void SetPosition(Vector2DF position) override;

		float GetAngle() const override;
		void SetAngle(float angle) override;

		Vector2DF GetScale() const override;
		void SetScale(Vector2DF scale) override;

		Vector2DF GetCenterPosition() const override;
		void SetCenterPosition(Vector2DF position) override;

		Color GetColor() const override;
		void SetColor(Color color) override;

		bool GetTurnLR() const override;
		void SetTurnLR(bool turnLR) override;

		bool GetTurnUL() const override;
		void SetTurnUL(bool turnUL) override;

		int GetDrawingPriority() const override;
		void SetDrawingPriority(int priority) override;

		AlphaBlendMode GetAlphaBlendMode() const override;
		void SetAlphaBlendMode(AlphaBlendMode alphaBlend) override;

		void SetTextureFilterType(TextureFilterType textureFilterType) override;
		TextureFilterType GetTextureFilterType() const override;

		CoreChip2D_Imp(Graphics* graphics);
		virtual ~CoreChip2D_Imp();

#if __CULLING_2D__
		void SetCullingObject(culling2d::Object *cullingObj);
		culling2d::Object* GetCullingObject() const;

		culling2d::Circle GetBoundingCircle();
#endif

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
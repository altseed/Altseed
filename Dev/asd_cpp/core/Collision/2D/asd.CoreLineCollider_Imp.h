#pragma once
#include "../../asd.ReferenceObject.h"
#include "asd.CoreLineCollider.h"
#include "asd.CoreCollider2D_Imp.h"
#include <Culling2D_AABB.h>
#include "Box2D/Box2D.h"

namespace asd
{
	class CoreLineCollider_Imp
		:public CoreLineCollider
		, public ReferenceObject
		, public CoreCollider2D_Imp
	{
		Vector2DF startingPosition;
		Vector2DF endingPosition;
		float thickness;
		b2PolygonShape b2polygonShape;
		astring tag;
		bool isVisible;
	public:
		CoreLineCollider_Imp();
		virtual ~CoreLineCollider_Imp();

		Vector2DF GetStartingPosition() const override;
		void SetStartingPosition(Vector2DF startingPosition) override;

		Vector2DF GetEndingPosition() const override;
		void SetEndingPosition(Vector2DF endingPosition) override;

		float GetThickness() const override;
		void SetThickness(float thickness) override;

		virtual void Update() override;

		const achar* GetTag() const override;
		void SetTag(const achar* tag) override;

		bool GetIsVisible() const override;
		void SetIsVisible(bool isVisible) override;

		int64_t GetCreationId() const override;
#if !SWIG
	public:
		virtual Collider2DShapeType GetShapeType() const override;

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }

		b2Shape* GetB2Shape() override;
		void DrawVisibleCollisionsAdditionally(CoreLayer2D*) override;
#endif
	};
};
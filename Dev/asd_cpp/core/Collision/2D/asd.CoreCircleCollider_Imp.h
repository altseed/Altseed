#pragma once
#include "../../asd.ReferenceObject.h"
#include "asd.CoreCircleCollider.h"
#include "asd.CoreCollider2D_Imp.h"
#include <Culling2D_AABB.h>
#include <Box2D/Box2D.h>

namespace asd
{
	class CoreCircleCollider_Imp
		:public CoreCircleCollider
		, public ReferenceObject
		, public CoreCollider2D_Imp
	{
		Vector2DF center;
		float radius;
		b2CircleShape b2circleShape;
		astring tag;
		bool isVisible;
	public:
		CoreCircleCollider_Imp();
		virtual ~CoreCircleCollider_Imp();

		Vector2DF GetCenter() const override;
		void SetCenter(Vector2DF position) override;

		float GetRadius() const override;
		void SetRadius(float angle) override;

		Collider2DShapeType GetType() const override;

		virtual void Update() override;

		const achar* GetTag() const override;
		void SetTag(const achar* tag) override;

		bool GetIsVisible() const override;
		void SetIsVisible(bool isVisible) override;

		long GetCreationId() const override;

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }

		b2Shape* GetB2Shape() override;

		void DrawVisibleCollisionsAdditionally(CoreLayer2D*) override;
#endif
	};
};
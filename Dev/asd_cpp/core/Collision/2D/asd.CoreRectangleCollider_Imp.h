
#include "asd.CoreRectangleCollider.h"
#include "asd.CoreCollider2D_Imp.h"
#include "../../asd.ReferenceObject.h"
#include <Culling2D_AABB.h>
#include "Box2D/Box2D.h"

namespace asd
{
	class CoreRectangleCollider_Imp
		:public CoreRectangleCollider
		, public CoreCollider2D_Imp
		, public ReferenceObject
	{
		RectF area;
		b2PolygonShape b2polygonShape;
		astring tag;
		bool isVisible;
	public:
		CoreRectangleCollider_Imp();
		virtual ~CoreRectangleCollider_Imp() {}

		RectF GetArea() const override;
		void SetArea(RectF area) override;

		Collider2DShapeType GetType() const override;
		void Update();

		const achar* GetTag() const override;
		void SetTag(const achar* tag) override;

		bool GetIsVisible() const override;
		void SetIsVisible(bool isVisible) override;
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
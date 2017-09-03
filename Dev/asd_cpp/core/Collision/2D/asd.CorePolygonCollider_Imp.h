#pragma once

#include "asd.CorePolygonCollider.h"
#include "../../asd.ReferenceObject.h"
#include "asd.CoreCollider2D_Imp.h"
#include <Culling2D_AABB.h>
#include "Box2D/Box2D.h"

namespace asd
{
	class CorePolygonCollider_Imp
		:public CorePolygonCollider
		, public ReferenceObject
		, public CoreCollider2D_Imp
	{
	private:
		std::vector<Vector2DF> vertexes;
		bool	isValid = true;
		b2PolygonShape b2polygonShape;
		astring tag;
		bool isVisible;
	public:
		CorePolygonCollider_Imp();
		virtual ~CorePolygonCollider_Imp();

		bool IsValid() override;
		void AddVertex(Vector2DF vertex) override;
		void ClearVertexes() override;
		int GetVertexesNum() const override;

		virtual void SetVertex(int index, Vector2DF vertex) override;
		virtual Vector2DF GetVertex(int index) const override;

		virtual void Update() override;

		const achar* GetTag() const override;
		void SetTag(const achar* tag) override;

		bool GetIsVisible() const override;
		void SetIsVisible(bool isVisible) override;

		int64_t GetCreationId() const override;
#if !SWIG

	public:
		Collider2DShapeType GetShapeType() const override;

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }

		b2Shape* GetB2Shape() override;
		void DrawVisibleCollisionsAdditionally(CoreLayer2D*) override;
#endif
	};
};
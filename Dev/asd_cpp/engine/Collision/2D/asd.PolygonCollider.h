#pragma once
#include "asd.Collider2D.h"

namespace asd {

	class PolygonCollider : public Collider2D
	{
	private:
		std::shared_ptr<CorePolygonCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;
	public:
		PolygonCollider();
		~PolygonCollider();

		bool IsValid();
		void AddVertex(Vector2DF vertex);
		void ClearVertexes();
		int GetVertexesNum() const;

		void SetVertex(int index, Vector2DF vertex);
		Vector2DF GetVertex(int index) const;

		Collider2DShapeType GetType() const override;
	};
}
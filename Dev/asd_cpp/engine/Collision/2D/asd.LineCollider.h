#pragma once

#include "asd.Collider2D.h"

namespace asd {

	class LineCollider : public Collider2D{
	private:
		std::shared_ptr<CoreLineCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;

	public:

		LineCollider();
		virtual ~LineCollider();

		Vector2DF GetStartingPosition() const;
		void SetStartingPosition(Vector2DF startingPosition);

		Vector2DF GetEndingPosition() const;
		void SetEndingPosition(Vector2DF endingPosition);

		float GetThickness() const;
		void SetThickness(float thickness);

		Collider2DShapeType GetType() const;
	};
}

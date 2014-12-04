#pragma once
#include "../common/Math/ace.RectF.h"
#include "ace.CoreMapObject2D.h"
#include "ace.CoreObject2D_Imp.h"

namespace ace
{
	class CoreMapObject2D_Imp
		: public CoreMapObject2D
		, public CoreObject2D_Imp
		, public ReferenceObject
	{
	protected:
		virtual void CalculateBoundingCircle();
	private:
		std::set<Chip2D*> m_chips;
		std::set<Chip2D*> m_drawChips;
		Vector2DF m_centerPosition;
		int m_drawingPtiority;
		culling2d::RectF m_currentDrawSrc;

		void addChipToDraw(Chip2D* chip);
	public:
		CoreMapObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreMapObject2D_Imp();

		Object2DType GetObjectType() const override { return Object2DType::Map; }

#pragma region Parameter
		Vector2DF GetCenterPosition() const;
		void SetCenterPosition(Vector2DF position);

		int GetDrawingPriority() const;
		void SetDrawingPriority(int priority);

		bool AddChip(Chip2D* chip);
		bool RemoveChip(Chip2D* chip);

		void Clear();
#pragma endregion
		culling2d::RectF &GetCurrentDrawSrc();
		void SetCurrentDrawSrc(culling2d::RectF currentDrawSrc);

		void Draw(Renderer2D* renderer);

		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
	};
}

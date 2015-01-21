#pragma once
#include "../common/Math/ace.RectF.h"
#include "ace.CoreMapObject2D.h"
#include "ace.CoreObject2D_Imp.h"
#include "ace.Culling2D.h"

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
		Vector2DF m_centerPosition;
		int m_drawingPtiority;
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

		std::set<Chip2D*>& GetChips()
		{
			return m_chips;
		}

		void Clear();
#pragma endregion

#if __CULLING_2D__
		void RegisterObjectToCulling();
#endif
		void Draw(Renderer2D* renderer);

		void DrawChip(Renderer2D* renderer, Chip2D* chip);

		culling2d::Circle GetChipBoundingCircle(Chip2D* chip);
		/*
		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
		*/

#include "ace.CoreObject2D_Imp_Methods.h"
	};
}

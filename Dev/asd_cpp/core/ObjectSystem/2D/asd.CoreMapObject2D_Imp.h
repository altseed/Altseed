#pragma once
#include "../common/Math/asd.RectF.h"
#include "asd.CoreMapObject2D.h"
#include "asd.CoreObject2D_Imp.h"
#include "asd.Culling2D.h"

namespace asd
{
	class CoreMapObject2D_Imp
		: public CoreMapObject2D
		, public CoreObject2D_Imp
		, public ReferenceObject
	{
	protected:
		virtual void CalculateBoundingCircle();
	private:
		std::set<CoreChip2D_Imp*> m_chips;
		Vector2DF m_centerPosition;
		int m_drawingPtiority;
		uint32_t firstSortedKey;

		std::vector<CoreChip2D*> renderedChips;

		void DrawChipInternal(CoreChip2D_Imp* chip, Matrix33& matrix, Matrix33& parentMatrix, Renderer2D* renderer);
	public:
		CoreMapObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreMapObject2D_Imp();

		Object2DType GetObjectType() const override { return Object2DType::Map; }

#pragma region Parameter
		Vector2DF GetCenterPosition() const;
		void SetCenterPosition(Vector2DF position);

		int GetDrawingPriority() const;
		void SetDrawingPriority(int priority);

		bool AddChip(CoreChip2D* chip);
		bool RemoveChip(CoreChip2D* chip);

		std::set<CoreChip2D_Imp*>& GetChips()
		{
			return m_chips;
		}

		void Clear();
#pragma endregion

#if __CULLING_2D__
		void RegisterObjectToCulling();

		uint32_t GetFirstSortedKey();
		void SetFirstSortedKey(uint32_t sortedKey);
#endif
		void Culling(const std::vector<culling2d::Object*>& culledObjects);

		void Draw(Renderer2D* renderer) override;

		culling2d::Circle GetChipBoundingCircle(CoreChip2D* chip);
		/*
		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
		*/

#include "asd.CoreObject2D_Imp_Methods.h"
	};
}

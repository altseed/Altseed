#include "ace.CoreRectangle.h"

namespace ace
{
	class CoreRectangle_Imp
		:public CoreRectangle
		, public ReferenceObject
	{
		RectF drawingArea;
		RectF uv;
		float angle;
	public:
		CoreRectangle_Imp();
		virtual ~CoreRectangle_Imp(){}

		RectF GetDrawingArea() const override;
		void SetDrawingArea(RectF drawingArea) override;

		RectF GetUV() const override;
		void SetUV(RectF uv) override;

		float GetAngle() const override;
		void SetAngle(float angle) override;

		ShapeType GetShapeType() const override;

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
		virtual std::vector<CoreTriangle*> GetDividedTriangles() override;
#endif
	};
};
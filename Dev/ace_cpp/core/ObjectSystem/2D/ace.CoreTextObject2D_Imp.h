
#include "ace.CoreTextObject2D.h"
#include "ace.CoreObject2D_Imp.h"

namespace ace
{
	class CoreTextObject2D_Imp
		: public CoreTextObject2D
		, public CoreObject2D_Imp
		, public ReferenceObject
	{
	public:
		CoreTextObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreTextObject2D_Imp();

		eObject2DType GetObjectType() const override { return eObject2DType::RENDERED_OBJECT2D_TYPE_TEXT; }

		void Draw(Renderer2D* renderer, Matrix33 cameraMatrix) override {}

		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
	};
}

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


		CORE_OBJECT2D_IMP_COMMON
	};
}
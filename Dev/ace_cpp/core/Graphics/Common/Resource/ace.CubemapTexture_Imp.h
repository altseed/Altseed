
#include "ace.CubemapTexture.h"

namespace ace
{
	class CubemapTexture_Imp
		: public CubemapTexture
		, public ReferenceObject
	{
	private:

	public:

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}
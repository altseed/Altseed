
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "../../../ace.ReferenceObject.h"
#include "ace.Texture2D.h"
#include "ace.RenderTexture_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Material2D
		: public IReference
	{
	protected:
		virtual Texture2D* GetTexture2D__(const achar* name) = 0;
		virtual void SetTexture2D__(const achar* name, Texture2D* value) = 0;

		Material2D(){}
		virtual ~Material2D(){}
	public:
		virtual float GetFloat(const achar* name) = 0;
		virtual void SetFloat(const achar* name, float value) = 0;

		virtual Vector2DF GetVector2DF(const achar* name) = 0;
		virtual void SetVector2DF(const achar* name, Vector2DF value) = 0;

		virtual Vector3DF GetVector3DF(const achar* name) = 0;
		virtual void SetVector3DF(const achar* name, Vector3DF value) = 0;

#if! SWIG
		std::shared_ptr<Texture2D> GetTexture2D(const achar* name)
		{
			return CreateSharedPtrWithReleaseDLL(GetTexture2D__(name));
		}

		void SetTexture2D(const achar* name, std::shared_ptr<Texture2D> value)
		{
			SetTexture2D__(name, value.get());
		}

		void SetTexture2D(const achar* name, std::shared_ptr<RenderTexture2D> value)
		{
			SetTexture2D__(name, value.get());
		}
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
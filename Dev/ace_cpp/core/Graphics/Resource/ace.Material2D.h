
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "../../ace.ReferenceObject.h"
#include "ace.Texture2D.h"
#include "ace.RenderTexture2D_Imp.h"

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
		friend class Accessor;

	protected:
		virtual Texture2D* GetTexture2D_(const achar* name) = 0;		
		virtual Shader2D* GetShader2D_() = 0;

		Material2D(){}
		virtual ~Material2D(){}
	public:
		virtual float GetFloat(const achar* name) = 0;
		virtual void SetFloat(const achar* name, float value) = 0;

		virtual Vector2DF GetVector2DF(const achar* name) = 0;
		virtual void SetVector2DF(const achar* name, Vector2DF value) = 0;

		virtual Vector3DF GetVector3DF(const achar* name) = 0;
		virtual void SetVector3DF(const achar* name, Vector3DF value) = 0;

		virtual Vector4DF GetVector4DF(const achar* name) = 0;
		virtual void SetVector4DF(const achar* name, Vector4DF value) = 0;

		virtual void SetTexture2D(const achar* name, Texture2D* value) = 0;
		virtual void SetShader2D(Shader2D* shader) = 0;

		virtual TextureFilterType GetTextureFilterType(const achar* name) = 0;
		virtual void SetTextureFilterType(const achar* name, TextureFilterType filter) = 0;

		virtual TextureWrapType GetTextureWrapType(const achar* name) = 0;
		virtual void SetTextureWrapType(const achar* name, TextureWrapType wrap) = 0;

#if! SWIG
		std::shared_ptr<Texture2D> GetTexture2D(const achar* name)
		{
			auto v = GetTexture2D_(name);
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		void SetTexture2D(const achar* name, std::shared_ptr<Texture2D> value)
		{
			SetTexture2D(name, value.get());
		}

		void SetTexture2D(const achar* name, std::shared_ptr<RenderTexture2D> value)
		{
			SetTexture2D(name, value.get());
		}

		std::shared_ptr<Shader2D> GetShader2D()
		{
			auto v = GetShader2D_();
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		void SetShader2D(std::shared_ptr<Shader2D> shader)
		{
			SetShader2D(shader.get());
		}
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
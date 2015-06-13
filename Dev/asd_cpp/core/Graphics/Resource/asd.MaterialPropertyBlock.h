
#pragma once

#include <asd.common.Base.h>
#include "../../asd.ReferenceObject.h"

#include "asd.RenderTexture2D.h"

namespace asd
{
	/**
		@brief	マテリアルへのパラメーターの集合
	*/
	class MaterialPropertyBlock
		: public IReference
	{
		friend class Accessor;

	protected:
		MaterialPropertyBlock() {}
		virtual ~MaterialPropertyBlock() {}

		/**
		@brief	テクスチャを取得する。
		@name	名称
		*/
		virtual Texture2D* GetTexture2D_(const achar* name) = 0;
	
	public:

		virtual float GetFloat(const achar* name) = 0;
		virtual void SetFloat(const achar* name, float value) = 0;

		virtual Vector2DF GetVector2DF(const achar* name) = 0;
		virtual void SetVector2DF(const achar* name, Vector2DF value) = 0;

		virtual Vector3DF GetVector3DF(const achar* name) = 0;
		virtual void SetVector3DF(const achar* name, Vector3DF value) = 0;

		virtual Vector4DF GetVector4DF(const achar* name) = 0;
		virtual void SetVector4DF(const achar* name, Vector4DF value) = 0;

		virtual TextureFilterType GetTextureFilterType(const achar* name) = 0;
		virtual void SetTextureFilterType(const achar* name, TextureFilterType filter) = 0;

		virtual TextureWrapType GetTextureWrapType(const achar* name) = 0;
		virtual void SetTextureWrapType(const achar* name, TextureWrapType wrap) = 0;

		virtual void SetTexture2D(const achar* name, Texture2D* value) = 0;

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
#endif
	};
}


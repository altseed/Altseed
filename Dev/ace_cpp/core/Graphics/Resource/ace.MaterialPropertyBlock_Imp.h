
#pragma once

#include "ace.MaterialPropertyBlock.h"

namespace ace
{
	class MaterialPropertyBlock_Imp
		: public MaterialPropertyBlock
		, public ReferenceObject
	{
	private:
		std::map<astring, ShaderConstantValue>		values;

	public:

		MaterialPropertyBlock_Imp();
		virtual ~MaterialPropertyBlock_Imp();

		std::map<astring, ShaderConstantValue>& GetValues() { return values; }

		float GetFloat(const achar* name) override;
		void SetFloat(const achar* name, float value) override;

		Vector2DF GetVector2DF(const achar* name) override;
		void SetVector2DF(const achar* name, Vector2DF value) override;

		Vector3DF GetVector3DF(const achar* name) override;
		void SetVector3DF(const achar* name, Vector3DF value) override;

		Vector4DF GetVector4DF(const achar* name) override;
		void SetVector4DF(const achar* name, Vector4DF value) override;

		Texture2D* GetTexture2D_(const achar* name) override;
		void SetTexture2D(const achar* name, Texture2D* value) override;

		TextureFilterType GetTextureFilterType(const achar* name) override;
		void SetTextureFilterType(const achar* name, TextureFilterType filter) override;

		TextureWrapType GetTextureWrapType(const achar* name) override;
		void SetTextureWrapType(const achar* name, TextureWrapType wrap) override;

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}
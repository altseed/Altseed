
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <asd.common.Base.h>
#include "../../asd.ReferenceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class MaterialCommand
	{
	protected:
		NativeShader_Imp*					shader;
		std::vector<ShaderConstantValue>	constantValues;
	public:
		MaterialCommand(NativeShader_Imp* shader, std::shared_ptr<MaterialPropertyBlock>& block);
		virtual ~MaterialCommand();

		NativeShader_Imp* GetShader() { return shader; }
		std::vector<ShaderConstantValue>& GetConstantValues() { return constantValues; }
	};

	class Material_Imp
		: public ReferenceObject
	{
	protected:
		std::shared_ptr<MaterialPropertyBlock>	block;
		
		Material_Imp();
		virtual ~Material_Imp();
	public:

		float GetFloat_Imp(const achar* name);
		void SetFloat_Imp(const achar* name, float value);

		Vector2DF GetVector2DF_Imp(const achar* name);
		void SetVector2DF_Imp(const achar* name, Vector2DF value);

		Vector3DF GetVector3DF_Imp(const achar* name);
		void SetVector3DF_Imp(const achar* name, Vector3DF value);

		Vector4DF GetVector4DF_Imp(const achar* name);
		void SetVector4DF_Imp(const achar* name, Vector4DF value);

		/**
			@brief	テクスチャを取得する。
			@name	名称
		*/
		Texture2D* GetTexture2D_(const achar* name);
		void SetTexture2D_(const achar* name, Texture2D* value);

		TextureFilterType GetTextureFilterType(const achar* name);
		void SetTextureFilterType(const achar* name, TextureFilterType filter);

		TextureWrapType GetTextureWrapType(const achar* name);
		void SetTextureWrapType(const achar* name, TextureWrapType wrap);

	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
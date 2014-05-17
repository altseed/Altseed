
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "../../ace.ReferenceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class MaterialCommand
	{
	public:
		struct Value
		{
			ShaderVariableType ValueType;

			union
			{
				float		Float4[4];
				float		Mat44[16];
				Texture2D*	TexturePtr;
			} Data;
		};

	protected:
		std::map<astring, Value>		m_values;

	public:
		MaterialCommand(std::map<astring, Value>& values);
		virtual ~MaterialCommand();

		virtual void SetValueToShader() = 0;
	};

	class Material_Imp
		: public ReferenceObject
	{
	protected:
		std::map<astring, MaterialCommand::Value>		m_values;

		Material_Imp(){}
		virtual ~Material_Imp();
	public:

		float GetFloat_Imp(const achar* name);
		void SetFloat_Imp(const achar* name, float value);

		Vector2DF GetVector2DF_Imp(const achar* name);
		void SetVector2DF_Imp(const achar* name, Vector2DF value);

		Vector3DF GetVector3DF_Imp(const achar* name);
		void SetVector3DF_Imp(const achar* name, Vector3DF value);

		/**
			@brief	テクスチャを取得する。
			@name	名称
			@note
			テクスチャを取得した際にテクスチャの参照カウンタが1加算される。
		*/
		Texture2D* GetTexture2D_Imp(const achar* name);
		void SetTexture2D_Imp(const achar* name, Texture2D* value);

	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
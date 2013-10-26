
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "../../../ace.ReferenceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Material_Imp
		: public ReferenceObject
	{
	protected:
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

		std::map<astring, Value>		m_values;

		Material_Imp(){}
		virtual ~Material_Imp();
	public:

		float GetFloat_(const achar* name);
		void SetFloat_(const achar* name, float value);

		Vector2DF GetVector2DF_(const achar* name);
		void SetVector2DF_(const achar* name, Vector2DF value);

		Vector3DF GetVector3DF_(const achar* name);
		void SetVector3DF_(const achar* name, Vector3DF value);

		/**
			@brief	テクスチャを取得する。
			@name	名称
			@note
			テクスチャを取得した際にテクスチャの参照カウンタが1加算される。
		*/
		Texture2D* GetTexture2D_(const achar* name);
		void SetTexture2D_(const achar* name, Texture2D* value);

	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
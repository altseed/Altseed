
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Material_Imp.h"
#include "ace.Material2D.h"

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Material2D_Imp
		: public Material2D
		, public Material_Imp
	{
	private:
		Shader2D_Imp*	m_shader;

		Material2D_Imp(Shader2D_Imp* shader);
		virtual ~Material2D_Imp();

	protected:
		
	public:

		virtual float GetFloat(const achar* name) { return Material_Imp::GetFloat_(name); }
		virtual void SetFloat(const achar* name, float value) { Material_Imp::SetFloat_(name, value); }

		virtual Vector2DF GetVector2DF(const achar* name) { return Material_Imp::GetVector2DF_(name); }
		virtual void SetVector2DF(const achar* name, Vector2DF value) { Material_Imp::SetVector2DF_(name, value); }

		virtual Vector3DF GetVector3DF(const achar* name) { return Material_Imp::GetVector3DF_(name); }
		virtual void SetVector3DF(const achar* name, Vector3DF value) { Material_Imp::SetVector3DF_(name, value); }

		Texture2D* GetTexture2D__(const achar* name) { return Material_Imp::GetTexture2D_(name); }
		void SetTexture2D__(const achar* name, Texture2D* value) { return Material_Imp::SetTexture2D_(name, value); }

		Shader2D_Imp* GetShader_Imp();
		void SetShader_Imp(Shader2D_Imp* shader);

		/**
			@brief	マテリアルに設定されている値をシェーダーに設定する。
		*/
		void SetValuesToShader();

#if !SWIG
		static Material2D_Imp* Create(Shader2D_Imp* shader);
#endif

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}
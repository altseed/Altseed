
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Material_Imp.h"
#include "asd.Material3D.h"

#include <Math/asd.Vector2DF.h>
#include <Math/asd.Vector3DF.h>


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	class Material3DCommand
		: public MaterialCommand
	{
	private:
		RenderTexture2D*	m_target;
		Shader3D_Imp*		m_shader;
	public:
		Material3DCommand(Shader3D_Imp* shader, std::shared_ptr<MaterialPropertyBlock>& values);
		virtual ~Material3DCommand();

		RenderTexture2D* GetTarget();

		void SetTarget(RenderTexture2D* target);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Material3D_Imp
		: public Material3D
		, public Material_Imp
	{
	private:

		Shader3D_Imp*	m_shader;

		Material3D_Imp(Shader3D_Imp* shader);
		virtual ~Material3D_Imp();

	protected:

	public:

		virtual float GetFloat(const achar* name) { return Material_Imp::GetFloat_Imp(name); }
		virtual void SetFloat(const achar* name, float value) { Material_Imp::SetFloat_Imp(name, value); }

		virtual Vector2DF GetVector2DF(const achar* name) { return Material_Imp::GetVector2DF_Imp(name); }
		virtual void SetVector2DF(const achar* name, Vector2DF value) { Material_Imp::SetVector2DF_Imp(name, value); }

		virtual Vector3DF GetVector3DF(const achar* name) { return Material_Imp::GetVector3DF_Imp(name); }
		virtual void SetVector3DF(const achar* name, Vector3DF value) { Material_Imp::SetVector3DF_Imp(name, value); }

		virtual Vector4DF GetVector4DF(const achar* name) { return Material_Imp::GetVector4DF_Imp(name); }
		virtual void SetVector4DF(const achar* name, Vector4DF value) { Material_Imp::SetVector4DF_Imp(name, value); }

		Texture2D* GetTexture2D_(const achar* name) { return Material_Imp::GetTexture2D_(name); }
		void SetTexture2D(const achar* name, Texture2D* value) { return Material_Imp::SetTexture2D_(name, value); }

		TextureFilterType GetTextureFilterType(const achar* name) { return Material_Imp::GetTextureFilterType(name); }
		void SetTextureFilterType(const achar* name, TextureFilterType filter) { Material_Imp::SetTextureFilterType(name, filter); }

		TextureWrapType GetTextureWrapType(const achar* name) { return Material_Imp::GetTextureWrapType(name); }
		void SetTextureWrapType(const achar* name, TextureWrapType wrap) { Material_Imp::SetTextureWrapType(name, wrap); }

		Shader3D* GetShader3D_();
		void SetShader3D(Shader3D* shader);

		/**
		@brief	マテリアルに設定されている値をシェーダーに設定するコマンドを出力する。
		*/
		std::shared_ptr <Material3DCommand> GenerateShaderCommand();

		std::shared_ptr<MaterialPropertyBlock> GetMaterialPropertyBlock() { return block; }

#if !SWIG
		static Material3D_Imp* Create(Shader3D_Imp* shader);
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
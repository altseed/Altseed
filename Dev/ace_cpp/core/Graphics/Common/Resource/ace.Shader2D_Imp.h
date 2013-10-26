
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "ace.Shader2D.h"
#include "../../../ace.Core.Base_Imp.h"
#include "../../../ace.ReferenceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	シェーダー
	*/
	class Shader2D_Imp
		: public Shader2D
		, public ReferenceObject
	{
	protected:
		std::shared_ptr<NativeShader_Imp>	m_shader;
		std::vector<ShaderVariableProperty>	m_vertexVariableProperties;
		std::vector<ShaderVariableProperty>	m_pixelVariableProperties;

		Shader2D_Imp(
			Graphics* graphics,
			std::shared_ptr<NativeShader_Imp> shader,
			std::vector<ShaderVariableProperty> vertexVariableProperties,
			std::vector<ShaderVariableProperty> pixelVariableProperties);
		virtual ~Shader2D_Imp();

	public:
		static Shader2D_Imp* Create(
			Graphics* graphics,
			const achar* shaderText,
			const achar* shaderFileName,
			std::vector <ShaderVariableProperty>& pixelVariableProperties,
			Log* log);

#if !SWIG
		std::shared_ptr<NativeShader_Imp> GetNativeShader() { return m_shader; }
		std::vector<ShaderVariableProperty>& GetVertexVariableProperties() { return m_vertexVariableProperties; }
		std::vector<ShaderVariableProperty>& GetPixelVariableProperties() { return m_pixelVariableProperties; }
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

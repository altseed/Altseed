﻿
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "ace.Shader3D.h"
#include "../../ace.Core.Base_Imp.h"
#include "../../ace.ReferenceObject.h"

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
	class Shader3D_Imp
		: public Shader3D
		, public ReferenceObject
	{
	protected:
		std::shared_ptr<NativeShader_Imp>	m_shader;

		Shader3D_Imp(
			Graphics* graphics,
			std::shared_ptr<NativeShader_Imp> shader);
		virtual ~Shader3D_Imp();

	public:
		static Shader3D_Imp* Create(
			Graphics* graphics,
			const achar* shaderText,
			const achar* shaderFileName,
			Log* log);

#if !SWIG
		std::shared_ptr<NativeShader_Imp> GetNativeShader() { return m_shader; }
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
﻿
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <asd.common.Base.h>
#include "asd.Shader2D.h"
#include "../../asd.Core.Base_Imp.h"
#include "../../asd.ReferenceObject.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
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

		Shader2D_Imp(
			Graphics* graphics,
			std::shared_ptr<NativeShader_Imp> shader);
		virtual ~Shader2D_Imp();

	public:
		static Shader2D_Imp* Create(
			Graphics* graphics,
			const achar* shaderText,
			const achar* shaderFileName,
			Log* log);

		static Shader2D_Imp* Create(Graphics* graphics, const achar* path, Log* log);

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

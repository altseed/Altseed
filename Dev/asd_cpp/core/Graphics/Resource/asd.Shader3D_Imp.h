
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <asd.common.Base.h>
#include "asd.Shader3D.h"
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
	class Shader3D_Imp
		: public Shader3D
		, public ReferenceObject
	{
	protected:
		Graphics*							graphics;

		astring								shaderText;
		astring								shaderFileName;

		std::shared_ptr<NativeShader_Imp>	shader;
		std::shared_ptr<NativeShader_Imp>	shader_light;
		std::shared_ptr<NativeShader_Imp>	shader_depth;
		std::shared_ptr<NativeShader_Imp>	shader_light_depth;

		std::shared_ptr<NativeShader_Imp>	mass_shaders[2];
		std::shared_ptr<NativeShader_Imp>	mass_shaders_light[2];
		std::shared_ptr<NativeShader_Imp>	mass_shaders_depth[2];
		std::shared_ptr<NativeShader_Imp>	mass_shaders_light_depth[2];

		std::shared_ptr<NativeShader_Imp>	terrain_shader;
		std::shared_ptr<NativeShader_Imp>	terrain_shader_light;

		Shader3D_Imp(
			Graphics* graphics,
			std::shared_ptr<NativeShader_Imp> shader,
			std::shared_ptr<NativeShader_Imp> shader_light,
			std::shared_ptr<NativeShader_Imp> shader_depth,
			std::shared_ptr<NativeShader_Imp> shader_light_depth,
			const achar* shaderText,
			const achar* shaderFileName);
		virtual ~Shader3D_Imp();
	public:
		static Shader3D_Imp* Create(
			Graphics* graphics,
			const achar* shaderText,
			const achar* shaderFileName,
			Log* log);

		void CompileMass();
		void CompileTerrain();

#if !SWIG
		std::shared_ptr<NativeShader_Imp> GetNativeShader() { return shader; }
		std::shared_ptr<NativeShader_Imp> GetNativeShaderLight() { return shader_light; }
		std::shared_ptr<NativeShader_Imp> GetNativeShaderDepth() { return shader_depth; }
		std::shared_ptr<NativeShader_Imp> GetNativeShaderLightDepth() { return shader_light_depth; }

		std::shared_ptr<NativeShader_Imp> GetNativeShaderMass(int32_t index) { return mass_shaders[index]; }
		std::shared_ptr<NativeShader_Imp> GetNativeShaderMassLight(int32_t index) { return mass_shaders_light[index]; }
		std::shared_ptr<NativeShader_Imp> GetNativeShaderMassDepth(int32_t index) { return mass_shaders_depth[index]; }
		std::shared_ptr<NativeShader_Imp> GetNativeShaderMassLightDepth(int32_t index) { return mass_shaders_light_depth[index]; }

		std::shared_ptr<NativeShader_Imp> GetNativeShaderTerrain() { return terrain_shader; }
		std::shared_ptr<NativeShader_Imp> GetNativeShaderTerrainLight() { return terrain_shader_light; }
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

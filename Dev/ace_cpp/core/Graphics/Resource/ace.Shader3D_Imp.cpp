
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Shader3D_Imp.h"
#include "ace.NativeShader_Imp.h"
#include "../ace.Graphics_Imp.h"

#include "../Shader/DX/3D/Model_Internal_VS.h"
#include "../Shader/DX/3D/Lightweight_Model_Internal_VS.h"

#include "../Shader/DX/3D/MassModel_Internal_VS.h"
#include "../Shader/DX/3D/Lightweight_MassModel_Internal_VS.h"

#include "../Shader/DX/3D/Terrain_Internal_VS.h"
#include "../Shader/DX/3D/Lightweight_Terrain_Internal_VS.h"

#include "../Shader/GL/3D/Model_Internal_VS.h"
#include "../Shader/GL/3D/Lightweight_Model_Internal_VS.h"

#include "../Shader/GL/3D/MassModel_Internal_VS.h"
#include "../Shader/GL/3D/Lightweight_MassModel_Internal_VS.h"

#include "../Shader/GL/3D/Terrain_Internal_VS.h"
#include "../Shader/GL/3D/Lightweight_Terrain_Internal_VS.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Shader3D_Imp::Shader3D_Imp(
		Graphics* graphics,
		std::shared_ptr<NativeShader_Imp> shader,
		std::shared_ptr<NativeShader_Imp> shader_light,
		std::shared_ptr<NativeShader_Imp> shader_depth,
		std::shared_ptr<NativeShader_Imp> shader_light_depth,
		const achar* shaderText,
		const achar* shaderFileName)
		: graphics(graphics)
		, shader(shader)
		, shader_light(shader_light)
		, shader_depth(shader_depth)
		, shader_light_depth(shader_light_depth)
		, shaderText(shaderText)
		, shaderFileName(shaderFileName)
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Shader3D_Imp::~Shader3D_Imp()
	{

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Shader3D_Imp* Shader3D_Imp::Create(
		Graphics* graphics,
		const achar* shaderText,
		const achar* shaderFileName,
		Log* log)
	{
		auto g = (Graphics_Imp*) graphics;

		std::shared_ptr<NativeShader_Imp> shader;
		std::shared_ptr<NativeShader_Imp> shader_light;
		std::shared_ptr<NativeShader_Imp> shader_depth;
		std::shared_ptr<NativeShader_Imp> shader_light_depth;


		std::vector<ace::VertexLayout> vl;
		vl.push_back(ace::VertexLayout("Position", ace::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Normal", ace::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Binormal", ace::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("UVSub", ace::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::VertexLayoutFormat::R8G8B8A8_UNORM));
		vl.push_back(ace::VertexLayout("BoneWeights", ace::VertexLayoutFormat::R8G8B8A8_UNORM));
		vl.push_back(ace::VertexLayout("BoneIndexes", ace::VertexLayoutFormat::R8G8B8A8_UINT));
		vl.push_back(ace::VertexLayout("BoneIndexesOriginal", ace::VertexLayoutFormat::R8G8B8A8_UINT));


		std::vector<ace::Macro> macro;
		std::vector<ace::Macro> macro_light;

		std::vector<ace::Macro> macro_depth;
		std::vector<ace::Macro> macro_light_depth;

		macro_depth.push_back(Macro("EXPORT_DEPTH", "1"));
		macro_light_depth.push_back(Macro("EXPORT_DEPTH", "1"));

		macro_light.push_back(Macro("LIGHTWEIGHT", "1"));
		macro_light_depth.push_back(Macro("LIGHTWEIGHT", "1"));

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
			shader = g->CreateShader_Imp(
				model_internal_vs_dx,
				"shader2d_dx_vs",
				ToUtf8String(shaderText).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro);

			shader_light = g->CreateShader_Imp(
				lightweight_model_internal_vs_dx,
				"shader2d_dx_vs",
				ToUtf8String(shaderText).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro_light);

			shader_depth = g->CreateShader_Imp(
				model_internal_vs_dx,
				"shader2d_dx_vs",
				ToUtf8String(shaderText).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro_depth);

			shader_light_depth = g->CreateShader_Imp(
				lightweight_model_internal_vs_dx,
				"shader2d_dx_vs",
				ToUtf8String(shaderText).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro_light_depth);
		}
		else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			shader = g->CreateShader_Imp(
				model_internal_vs_gl,
				"shader3d_gl_vs",
				ToUtf8String(shaderText).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro);

			shader_light = g->CreateShader_Imp(
				lightweight_model_internal_vs_gl,
				"shader3d_gl_vs",
				ToUtf8String(shaderText).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro_light);

			shader_depth = g->CreateShader_Imp(
				model_internal_vs_gl,
				"shader3d_gl_vs",
				ToUtf8String(shaderText).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro_depth);

			shader_light_depth = g->CreateShader_Imp(
				lightweight_model_internal_vs_gl,
				"shader3d_gl_vs",
				ToUtf8String(shaderText).c_str(),
				ToUtf8String(shaderFileName).c_str(),
				vl,
				macro_light_depth);
		}
		else
		{
			assert(0);
		}

		if (shader == nullptr) return nullptr;

		return new Shader3D_Imp(
			g,
			shader,
			shader_light,
			shader_depth,
			shader_light_depth,
			shaderText,
			shaderFileName);
	}

	void Shader3D_Imp::CompileMass()
	{
		if (mass_shaders[0] != nullptr) return;

		auto g = (Graphics_Imp*) graphics;

		std::shared_ptr<NativeShader_Imp> shaders[2];
		std::shared_ptr<NativeShader_Imp> shaders_light[2];
		std::shared_ptr<NativeShader_Imp> shaders_depth[2];
		std::shared_ptr<NativeShader_Imp> shaders_light_depth[2];

		for (int32_t i = 0; i < 2; i++)
		{
			std::vector<ace::VertexLayout> vl;
			vl.push_back(ace::VertexLayout("Position", ace::VertexLayoutFormat::R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("Normal", ace::VertexLayoutFormat::R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("Binormal", ace::VertexLayoutFormat::R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("UV", ace::VertexLayoutFormat::R32G32_FLOAT));
			vl.push_back(ace::VertexLayout("UVSub", ace::VertexLayoutFormat::R32G32_FLOAT));
			vl.push_back(ace::VertexLayout("Color", ace::VertexLayoutFormat::R8G8B8A8_UNORM));
			vl.push_back(ace::VertexLayout("BoneWeights", ace::VertexLayoutFormat::R8G8B8A8_UNORM));
			vl.push_back(ace::VertexLayout("BoneIndexes", ace::VertexLayoutFormat::R8G8B8A8_UINT));
			//vl.push_back(ace::VertexLayout("BoneIndexesOriginal", ace::VertexLayoutFormat::R8G8B8A8_UINT));

			std::vector<ace::Macro> macro;
			std::vector<ace::Macro> macro_light;

			std::vector<ace::Macro> macro_depth;
			std::vector<ace::Macro> macro_light_depth;

			macro_depth.push_back(Macro("EXPORT_DEPTH", "1"));
			macro_light_depth.push_back(Macro("EXPORT_DEPTH", "1"));

			macro_light.push_back(Macro("LIGHTWEIGHT", "1"));
			macro_light_depth.push_back(Macro("LIGHTWEIGHT", "1"));

			if (i == 1)
			{
				macro.push_back(Macro("ANIMATION_IS_ENABLED", "1"));
				macro_light.push_back(Macro("ANIMATION_IS_ENABLED", "1"));
				macro_depth.push_back(Macro("ANIMATION_IS_ENABLED", "1"));
				macro_light_depth.push_back(Macro("ANIMATION_IS_ENABLED", "1"));
			}

			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
			{
				shaders[i] = g->CreateShader_Imp(
					mass_model_internal_vs_dx,
					"shader2d_dx_vs",
					ToUtf8String(shaderText.c_str()).c_str(),
					ToUtf8String(shaderFileName.c_str()).c_str(),
					vl,
					macro);

				shaders_light[i] = g->CreateShader_Imp(
					lightweight_mass_model_internal_vs_dx,
					"shader2d_dx_vs",
					ToUtf8String(shaderText.c_str()).c_str(),
					ToUtf8String(shaderFileName.c_str()).c_str(),
					vl,
					macro_light);

				shaders_depth[i] = g->CreateShader_Imp(
					mass_model_internal_vs_dx,
					"shader2d_dx_vs",
					ToUtf8String(shaderText.c_str()).c_str(),
					ToUtf8String(shaderFileName.c_str()).c_str(),
					vl,
					macro_depth);

				shaders_light_depth[i] = g->CreateShader_Imp(
					lightweight_mass_model_internal_vs_dx,
					"shader2d_dx_vs",
					ToUtf8String(shaderText.c_str()).c_str(),
					ToUtf8String(shaderFileName.c_str()).c_str(),
					vl,
					macro_light_depth);
			}
			else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				shaders[i] = g->CreateShader_Imp(
					mass_model_internal_vs_gl,
					"shader3d_gl_vs",
					ToUtf8String(shaderText.c_str()).c_str(),
					ToUtf8String(shaderFileName.c_str()).c_str(),
					vl,
					macro);

				shaders_light[i] = g->CreateShader_Imp(
					lightweight_mass_model_internal_vs_gl,
					"shader3d_gl_vs",
					ToUtf8String(shaderText.c_str()).c_str(),
					ToUtf8String(shaderFileName.c_str()).c_str(),
					vl,
					macro_light);

				shaders_depth[i] = g->CreateShader_Imp(
					mass_model_internal_vs_gl,
					"shader3d_gl_vs",
					ToUtf8String(shaderText.c_str()).c_str(),
					ToUtf8String(shaderFileName.c_str()).c_str(),
					vl,
					macro_depth);

				shaders_light_depth[i] = g->CreateShader_Imp(
					lightweight_mass_model_internal_vs_gl,
					"shader3d_gl_vs",
					ToUtf8String(shaderText.c_str()).c_str(),
					ToUtf8String(shaderFileName.c_str()).c_str(),
					vl,
					macro_light_depth);
			}

			mass_shaders[i] = shaders[i];
			mass_shaders_light[i] = shaders_light[i];
			mass_shaders_depth[i] = shaders_depth[i];
			mass_shaders_light_depth[i] = shaders_light_depth[i];
		}
	}

	void Shader3D_Imp::CompileTerrain()
	{
		if (terrain_shader != nullptr) return;

		auto g = (Graphics_Imp*) graphics;

		std::shared_ptr<NativeShader_Imp> shader;
		std::shared_ptr<NativeShader_Imp> shader_light;

		std::vector<ace::VertexLayout> vl;
		vl.push_back(ace::VertexLayout("Position", ace::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Normal", ace::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Binormal", ace::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("UVSub", ace::VertexLayoutFormat::R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::VertexLayoutFormat::R8G8B8A8_UNORM));
		//vl.push_back(ace::VertexLayout("BoneWeights", ace::VertexLayoutFormat::R8G8B8A8_UNORM));
		//vl.push_back(ace::VertexLayout("BoneIndexes", ace::VertexLayoutFormat::R8G8B8A8_UINT));
		//vl.push_back(ace::VertexLayout("BoneIndexesOriginal", ace::VertexLayoutFormat::R8G8B8A8_UINT));


		std::vector<ace::Macro> macro;
		std::vector<ace::Macro> macro_light;

		macro_light.push_back(Macro("LIGHTWEIGHT", "1"));

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::DirectX11)
		{
			shader = g->CreateShader_Imp(
				terrain_internal_vs_dx,
				"shader2d_dx_vs",
				ToUtf8String(shaderText.c_str()).c_str(),
				ToUtf8String(shaderFileName.c_str()).c_str(),
				vl,
				macro);

			shader_light = g->CreateShader_Imp(
				lightweight_terrain_internal_vs_dx,
				"shader2d_dx_vs",
				ToUtf8String(shaderText.c_str()).c_str(),
				ToUtf8String(shaderFileName.c_str()).c_str(),
				vl,
				macro_light);
		}
		else if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			shader = g->CreateShader_Imp(
				terrain_internal_vs_gl,
				"shader3d_gl_vs",
				ToUtf8String(shaderText.c_str()).c_str(),
				ToUtf8String(shaderFileName.c_str()).c_str(),
				vl,
				macro);

			shader_light = g->CreateShader_Imp(
				lightweight_terrain_internal_vs_gl,
				"shader3d_gl_vs",
				ToUtf8String(shaderText.c_str()).c_str(),
				ToUtf8String(shaderFileName.c_str()).c_str(),
				vl,
				macro_light);
		}

		terrain_shader = shader;
		terrain_shader_light = shader_light;
	}
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

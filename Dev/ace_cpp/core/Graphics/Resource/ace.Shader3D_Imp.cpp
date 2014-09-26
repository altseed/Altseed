
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Shader3D_Imp.h"
#include "ace.NativeShader_Imp.h"
#include "../ace.Graphics_Imp.h"

#include "../Shader/DX/3D/Model_Internal_VS.h"
#include "../Shader/DX/3D/Lightweight_Model_Internal_VS.h"

#include "../Shader/GL/3D/Model_Internal_VS.h"
#include "../Shader/GL/3D/Lightweight_Model_Internal_VS.h"

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
		std::shared_ptr<NativeShader_Imp> shader_light_depth)
		: shader(shader)
		, shader_light(shader_light)
		, shader_depth(shader_depth)
		, shader_light_depth(shader_light_depth)
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
		vl.push_back(ace::VertexLayout("Position", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Normal", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("Binormal", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("UVSub", ace::LAYOUT_FORMAT_R32G32_FLOAT));
		vl.push_back(ace::VertexLayout("Color", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));
		vl.push_back(ace::VertexLayout("BoneWeights", ace::LAYOUT_FORMAT_R8G8B8A8_UNORM));
		vl.push_back(ace::VertexLayout("BoneIndexes", ace::LAYOUT_FORMAT_R8G8B8A8_UINT));
		vl.push_back(ace::VertexLayout("BoneIndexesOriginal", ace::LAYOUT_FORMAT_R8G8B8A8_UINT));


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
			shader_light_depth);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

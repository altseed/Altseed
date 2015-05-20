
#include "ace.LightRendering.h"

#include "../../ace.Graphics_Imp.h"
#include "../../Resource/ace.ShaderCache.h"

#include "../../Shader/DX/3D/Screen_VS.h"
#include "../../Shader/GL/3D/Screen_VS.h"

#include "../../Shader/DX/3D/Light_PS.h"
#include "../../Shader/GL/3D/Light_PS.h"

#include "../../Shader/DX/3D/Blur_PS.h"
#include "../../Shader/GL/3D/Blur_PS.h"

#include "../Object/ace.RenderedObject3D.h"
#include "../Object/ace.RenderedCameraObject3D.h"
#include "../Object/ace.RenderedDirectionalLightObject3D.h"

#include "../../Command/ace.RenderingCommandHelper.h"

namespace ace
{
	LightRendering::LightRendering(Graphics* graphics)
	{
		auto g = (Graphics_Imp*) graphics;

		{
			std::vector<ace::VertexLayout> vl;
			vl.push_back(ace::VertexLayout("Position", ace::VertexLayoutFormat::R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("UV", ace::VertexLayoutFormat::R32G32_FLOAT));

			std::vector<ace::Macro> macro_x;
			macro_x.push_back(Macro("BLUR_X", "1"));

			std::vector<ace::Macro> macro_y;
			macro_y.push_back(Macro("BLUR_Y", "1"));

			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				shadowBlurXShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurX").c_str(),
					screen_vs_gl,
					blur_ps_gl,
					vl,
					macro_x);

				shadowBlurYShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurY").c_str(),
					screen_vs_gl,
					blur_ps_gl,
					vl,
					macro_y);
			}
			else
			{
				shadowBlurXShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurX").c_str(),
					screen_vs_dx,
					blur_ps_dx,
					vl,
					macro_x);

				shadowBlurYShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.BlurY").c_str(),
					screen_vs_dx,
					blur_ps_dx,
					vl,
					macro_y);
			}
		}

		{
			std::vector<ace::VertexLayout> vl;
			vl.push_back(ace::VertexLayout("Position", ace::VertexLayoutFormat::R32G32B32_FLOAT));
			vl.push_back(ace::VertexLayout("UV", ace::VertexLayoutFormat::R32G32_FLOAT));

			std::vector<ace::Macro> macro_d_a;
			macro_d_a.push_back(Macro("DIRECTIONAL_LIGHT", "1"));
			macro_d_a.push_back(Macro("AMBIENT_LIGHT", "1"));

			std::vector<ace::Macro> macro_d;
			macro_d.push_back(Macro("DIRECTIONAL_LIGHT", "1"));

			std::vector<ace::Macro> macro_a;
			macro_a.push_back(Macro("AMBIENT_LIGHT", "1"));

			if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
			{
				directionalWithAmbientLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_A_Light").c_str(),
					screen_vs_gl,
					light_ps_gl,
					vl,
					macro_d_a);

				directionalLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_Light").c_str(),
					screen_vs_gl,
					light_ps_gl,
					vl,
					macro_d);

				ambientLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.A_Light").c_str(),
					screen_vs_gl,
					light_ps_gl,
					vl,
					macro_a);
			}
			else
			{
				directionalWithAmbientLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_A_Light").c_str(),
					screen_vs_dx,
					light_ps_dx,
					vl,
					macro_d_a);

				directionalLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.D_Light").c_str(),
					screen_vs_dx,
					light_ps_dx,
					vl,
					macro_d);

				ambientLightShader = g->GetShaderCache()->CreateFromCode(
					ToAString(L"Internal.A_Light").c_str(),
					screen_vs_dx,
					light_ps_dx,
					vl,
					macro_a);
			}
		}
	}

	LightRendering::~LightRendering()
	{

	}


	void LightRendering::Draw(RenderingCommandHelper* helper, std::set<RenderedObject3DProxy*>& objects, RenderedCameraObject3DProxy* cameraP, RenderedDirectionalLightObject3DProxy* lightP, Color skyColor, Color groundColor)
	{
	}

	void LightRendering::Draw(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cameraP, Color skyColor, Color groundColor)
	{

	}
}
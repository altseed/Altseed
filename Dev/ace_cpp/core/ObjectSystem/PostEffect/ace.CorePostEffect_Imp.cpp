
#include "ace.CorePostEffect_Imp.h"
#include "../../Graphics/2D/ace.PostEffectRenderer.h"
#include "../../Graphics/Resource/ace.Shader2D_Imp.h"
#include "../../Graphics/Resource/ace.Material2D_Imp.h"

#include "../../Graphics/Shader/DX/PostEffect/LightBloom_PS.h"
#include "../../Graphics/Shader/GL/PostEffect/LightBloom_PS.h"

namespace ace
{
	CorePostEffect_Imp::CorePostEffect_Imp(Graphics* graphics)
	{
	}

	CorePostEffect_Imp::~CorePostEffect_Imp()
	{
	}

	void CorePostEffect_Imp::DrawOnTexture2DWithMaterial(RenderTexture2D* target, Material2D* material)
	{
		auto target_ = (RenderTexture2D_Imp*) target;
		auto material_ = (Material2D_Imp*) material;

		auto command = material_->GenerateShaderCommand();
		command->SetTarget(target_);

		m_commands.push_back(command);
	}

	const char* CorePostEffect_Imp::GetLightBloomShader(GraphicsType graphicsType) const
	{
		if (graphicsType == GraphicsType::DirectX11)
		{
			return lightbloom_ps_dx;
		}
		else if (graphicsType == GraphicsType::OpenGL)
		{
			return lightbloom_ps_gl;
		}

		return nullptr;
	}
}

#include "ace.CorePostEffect_Imp.h"
#include "../../Graphics/Common/2D/ace.PostEffectRenderer.h"
#include "../../Graphics/Common/Resource/ace.Shader2D_Imp.h"
#include "../../Graphics/Common/Resource/ace.Material2D_Imp.h"

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
		auto target_ = (RenderTexture_Imp*) target;
		auto material_ = (Material2D_Imp*) material;

		auto command = material_->GenerateShaderCommand();
		command->SetTarget(target_);

		m_commands.push_back(command);
	}
}
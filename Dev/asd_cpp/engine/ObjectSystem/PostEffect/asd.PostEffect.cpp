
#include "asd.PostEffect.h"

namespace asd
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	PostEffect::PostEffect()
	{
		m_corePostEffect = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreatePostEffect());
	}

	PostEffect::~PostEffect()
	{
	
	}

	CorePostEffect* PostEffect::GetCoreObject() const
	{
		return m_corePostEffect.get();
	}

	void PostEffect::DrawOnTexture2DWithMaterial(std::shared_ptr<RenderTexture2D> target, std::shared_ptr<Material2D> material)
	{
		m_corePostEffect->DrawOnTexture2DWithMaterial(
			target.get(),
			material.get());
	}

	bool PostEffect::GetIsEnabled() const
	{
		return isEnabled;
	}

	void PostEffect::SetIsEnabled(bool value)
	{
		isEnabled = value;
	}
}

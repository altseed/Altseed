
#include "ace.PostEffect.h"

namespace ace
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
}

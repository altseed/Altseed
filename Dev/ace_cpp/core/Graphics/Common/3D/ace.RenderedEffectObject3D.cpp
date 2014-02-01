
#include "ace.RenderedEffectObject3D.h"
#include "ace.Renderer3D.h"

#include "../ace.Graphics_Imp.h"

#include "../Resource/ace.Effect_Imp.h"

namespace ace
{
	RenderedEffectObject3D::RenderedEffectObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
		, m_effect(nullptr)
	{
	}

	RenderedEffectObject3D::~RenderedEffectObject3D()
	{
		SafeRelease(m_effect);
	}

	void RenderedEffectObject3D::Flip()
	{
		RenderedObject3D::Flip();
	}

	void RenderedEffectObject3D::Rendering(RenderingProperty& prop)
	{

	}

	void RenderedEffectObject3D::SetEffect(Effect* effect)
	{
		SafeSubstitute(m_effect, effect);
	}

	void RenderedEffectObject3D::Play()
	{
		if (m_effect == nullptr) return;
		if (m_renderer == nullptr) return;

		auto e = (Effect_Imp*) m_effect;
		auto ne = e->GetEffect();	
		auto pos = GetPosition();

		auto handle = m_renderer->GetEffectManager()->Play(ne, pos.X, pos.Y, pos.Z);

		m_handles.push_back(handle);
	}

	void RenderedEffectObject3D::OnAdded(Renderer3D* renderer)
	{
		assert(m_renderer == nullptr);

		m_renderer = renderer;
	}

	void RenderedEffectObject3D::OnRemoving(Renderer3D* renderer)
	{
		assert(m_renderer != nullptr);

		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->StopEffect(h);
		}

		m_renderer = nullptr;
	}

}
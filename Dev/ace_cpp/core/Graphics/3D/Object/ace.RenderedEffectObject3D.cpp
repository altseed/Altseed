
#include "ace.RenderedEffectObject3D.h"
#include "../Renderer/ace.Renderer3D.h"

#include "../../ace.Graphics_Imp.h"

#include "../../Resource/ace.Effect_Imp.h"

namespace ace
{
	RenderedEffectObject3D::RenderedEffectObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
		, m_effect(nullptr)
	{
		proxy = new RenderedEffectObject3DProxy();
	}

	RenderedEffectObject3D::~RenderedEffectObject3D()
	{
		SafeRelease(m_effect);
		SafeRelease(proxy);
	}

	void RenderedEffectObject3D::Flip()
	{
		RenderedObject3D::Flip();

		// 一定以上たまったら削除処理
		if (m_handles.size() > GCThreshold)
		{
			for (size_t i = 0; i < m_handles.size();)
			{
				if (m_renderer->GetEffectManager()->Exists(m_handles[i]))
				{
					i++;
				}
				else
				{
					for (size_t j = i; j < m_handles.size() - 1; j++)
					{
						m_handles[j] = m_handles[j + 1];
					}
					m_handles.resize(m_handles.size() - 1);
				}
			}
		}

		if (m_syncEffects && m_handles.size() > 0)
		{
			auto mat = GetLocalMatrix();
			Effekseer::Matrix43 efMat;

			// 転置して代入
			for (auto c = 0; c < 3; c++)
			{
				for (auto r = 0; r < 4; r++)
				{
					efMat.Value[r][c] = mat.Values[c][r];
				}
			}

			for (size_t i = 0; i < m_handles.size();)
			{
				if (m_renderer->GetEffectManager()->Exists(m_handles[i]))
				{
					m_renderer->GetEffectManager()->SetMatrix(m_handles[i], efMat);
					i++;
				}
				else
				{
					for (size_t j = i; j < m_handles.size() - 1; j++)
					{
						m_handles[j] = m_handles[j + 1];
					}
					m_handles.resize(m_handles.size() - 1);
				}
			}
		}
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
		auto mat = GetLocalMatrix();
		Effekseer::Matrix43 efMat;

		// 転置して代入
		for (auto c = 0; c < 3; c++)
		{
			for (auto r = 0; r < 4; r++)
			{
				efMat.Value[r][c] = mat.Values[c][r];
			}
		}
		auto handle = m_renderer->GetEffectManager()->Play(ne, pos.X, pos.Y, pos.Z);
		m_renderer->GetEffectManager()->SetMatrix(handle, efMat);
		m_handles.push_back(handle);
	}

	void RenderedEffectObject3D::Stop()
	{
		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->StopEffect(h);
		}
		m_handles.clear();
	}

	void RenderedEffectObject3D::StopRoot()
	{
		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->StopRoot(h);
		}
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
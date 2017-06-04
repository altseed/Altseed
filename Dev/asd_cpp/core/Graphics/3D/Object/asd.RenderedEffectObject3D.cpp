
#include "asd.RenderedEffectObject3D.h"
#include "../Renderer/asd.Renderer3D.h"

#include "../../asd.Graphics_Imp.h"

#include "../../Resource/asd.Effect_Imp.h"

namespace asd
{
	int32_t RenderedEffectObject3D::PlayInternal(int32_t id)
	{
		if (m_effect == nullptr) return -1;
		if (m_renderer == nullptr) return -1;

		auto internalHandle = -1;
		if (id >= 0)
		{
			internalHandle = id;
		}
		else
		{
			internalHandle = nextInternalHandle;
			nextInternalHandle++;
		}

		auto e = (Effect_Imp*)m_effect;
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

		internalHandleToEffekseerHandle[internalHandle] = handle;

		return internalHandle;
	}

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

	void RenderedEffectObject3D::Flip(float deltaTime)
	{
		RenderedObject3D::Flip(deltaTime);

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
					auto value = m_handles[i];

					for (size_t j = i; j < m_handles.size() - 1; j++)
					{
						m_handles[j] = m_handles[j + 1];
					}
					m_handles.resize(m_handles.size() - 1);

					internalHandleToEffekseerHandle.erase(value);
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

	void RenderedEffectObject3D::SetEffect(Effect* effect)
	{
		SafeSubstitute(m_effect, effect);
	}

	int32_t RenderedEffectObject3D::Play()
	{
		if (m_effect == nullptr) return -1;
		if (m_renderer == nullptr)
		{
			auto internalHandle = nextInternalHandle;
			nextInternalHandle++;

			Command cmd;
			cmd.Type = CommandType::Play;
			cmd.ID = internalHandle;
			commands.push_back(cmd);

			return internalHandle;
		}
		else
		{
			return PlayInternal(-1);
		}
	}

	void RenderedEffectObject3D::Stop()
	{
		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->StopEffect(h);
		}
		m_handles.clear();
		internalHandleToEffekseerHandle.clear();

		if (m_renderer == nullptr)
		{
			Command cmd;
			cmd.Type = CommandType::Stop;
			commands.push_back(cmd);
		}
	}

	void RenderedEffectObject3D::StopRoot()
	{
		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->StopRoot(h);
		}

		if (m_renderer == nullptr)
		{
			Command cmd;
			cmd.Type = CommandType::StopRoot;
			commands.push_back(cmd);
		}
	}

	void RenderedEffectObject3D::Show()
	{
		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->SetShown(h, true);
		}

		if (m_renderer == nullptr)
		{
			Command cmd;
			cmd.Type = CommandType::Show;
			commands.push_back(cmd);
		}
	}

	void RenderedEffectObject3D::Hide()
	{
		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->SetShown(h, false);
		}

		if (m_renderer == nullptr)
		{
			Command cmd;
			cmd.Type = CommandType::Hide;
			commands.push_back(cmd);
		}
	}

	bool RenderedEffectObject3D::GetIsPlaying()
	{
		for (size_t i = 0; i < m_handles.size(); i++)
		{
			if (m_renderer->GetEffectManager()->Exists(m_handles[i]))
			{
				return true;
			}
		}

		return false;
	}

	void RenderedEffectObject3D::OnAdded(Renderer3D* renderer)
	{
		assert(m_renderer == nullptr);

		m_renderer = renderer;
		
		for (auto& c : commands)
		{
			if (c.Type == CommandType::Play)
			{
				PlayInternal(c.ID);
			}
			else if (c.Type == CommandType::Stop)
			{
				for (auto& h : m_handles)
				{
					m_renderer->GetEffectManager()->StopEffect(h);
				}
				m_handles.clear();
				internalHandleToEffekseerHandle.clear();
			}
			else if (c.Type == CommandType::StopRoot)
			{
				for (auto& h : m_handles)
				{
					m_renderer->GetEffectManager()->StopRoot(h);
				}
			}
			else if (c.Type == CommandType::Show)
			{
				for (auto& h : m_handles)
				{
					m_renderer->GetEffectManager()->SetShown(h, true);
				}
			}
			else if (c.Type == CommandType::Hide)
			{
				for (auto& h : m_handles)
				{
					m_renderer->GetEffectManager()->SetShown(h, false);
				}
			}
		}

		commands.clear()
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
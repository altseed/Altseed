
#include "asd.CoreEffectObject2D_Imp.h"
#include "../../Graphics/Resource/asd.Effect_Imp.h"

namespace asd
{
	Effect* CoreEffectObject2D_Imp::GetEffect_() const
	{
		return m_effect;
	}

	Effekseer::Matrix43 CoreEffectObject2D_Imp::CalcEffectMatrix()
	{
		auto pos = GetPosition();

		auto parentMatrix = GetParentsMatrix();
		auto matrix = GetMatrixToTransform();
		auto mat = parentMatrix * matrix;
		//auto v3 = Vector3DF(pos.X, pos.Y, 1);
		//auto pos_ = parentMatrix * matrix * v3;

		auto posX = mat.Values[0][2];
		auto posY = mat.Values[1][2];

		auto sx = sqrt(mat.Values[0][0] * mat.Values[0][0] + mat.Values[1][0] * mat.Values[1][0]);
		auto sy = sqrt(mat.Values[0][1] * mat.Values[0][1] + mat.Values[1][1] * mat.Values[1][1]);

		auto cos_ = mat.Values[0][0] / sx;
		auto sin_ = mat.Values[1][0] / sy;

		Effekseer::Matrix43 efMat;
		efMat.Indentity();

		// 位置設定
		efMat.Value[3][0] = posX;
		efMat.Value[3][1] = -posY;
		efMat.Value[3][2] = 0.0f;

		// 回転拡大設定
		efMat.Value[0][0] = sx * cos_;
		efMat.Value[0][1] = sx * (-sin_);
		efMat.Value[1][0] = sy * (sin_);
		efMat.Value[1][1] = sy * cos_;

		efMat.Value[2][2] = (sx + sy) / 2.0f;

		Effekseer::Matrix43 ef2Mat;

		// X軸回転
		Effekseer::Matrix43 rotyMatX;
		rotyMatX.RotationX(DegreeToRadian(-m_rotationX));
		Effekseer::Matrix43::Multiple(ef2Mat, rotyMatX, efMat);

		// Y軸回転
		Effekseer::Matrix43 rotyMatY;
		rotyMatY.RotationY(DegreeToRadian(-m_rotationY));
		Effekseer::Matrix43::Multiple(ef2Mat, rotyMatY, ef2Mat);

		return ef2Mat;
	}

	CoreEffectObject2D_Imp::CoreEffectObject2D_Imp(Graphics_Imp* graphics)
		: CoreObject2D_Imp(graphics)
		, m_effect(nullptr)
		, m_renderer(nullptr)
		, m_drawingPtiority(0)
	{

	}

	CoreEffectObject2D_Imp::~CoreEffectObject2D_Imp()
	{
		SafeRelease(m_effect);
	}

	void CoreEffectObject2D_Imp::SetEffect(Effect* effect)
	{
		SafeSubstitute(m_effect, effect);
	}

	int32_t CoreEffectObject2D_Imp::Play()
	{
		if (m_effect == nullptr) return -1;
		if (m_renderer == nullptr) return -1;
		ASSERT_STATIC_CAST(Effect_Imp*, m_effect);

		auto e = (Effect_Imp*) m_effect;
		auto ne = e->GetEffect();

		Effekseer::Matrix43 efMat = CalcEffectMatrix();
		
		auto handle = m_renderer->GetEffectManager()->Play(ne, 0.0f, 0.0f, 0.0f);
		m_renderer->GetEffectManager()->SetMatrix(handle, efMat);
		m_handles.push_back(handle);

		return handle;
	}

	void CoreEffectObject2D_Imp::Stop()
	{
		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->StopEffect(h);
		}
		m_handles.clear();
	}

	void CoreEffectObject2D_Imp::StopRoot()
	{
		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->StopRoot(h);
		}
	}

	void CoreEffectObject2D_Imp::Show()
	{
		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->SetShown(h, true);
		}
	}

	void CoreEffectObject2D_Imp::Hide()
	{
		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->SetShown(h, false);
		}
	}

	bool CoreEffectObject2D_Imp::GetIsPlaying()
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
	
	float CoreEffectObject2D_Imp::GetEffectRotationX() const
	{
		return m_rotationX;
	}

	void CoreEffectObject2D_Imp::SetEffectRotationX(float value)
	{
		m_rotationX = value;
	}

	float CoreEffectObject2D_Imp::GetEffectRotationY() const
	{
		return m_rotationY;
	}

	void CoreEffectObject2D_Imp::SetEffectRotationY(float value)
	{
		m_rotationY = value;
	}

	float CoreEffectObject2D_Imp::GetEffectRotation() const
	{
		return m_rotationY;
	}

	void CoreEffectObject2D_Imp::SetEffectRotation(float value)
	{
		m_rotationY = value;
	}

	void CoreEffectObject2D_Imp::OnAdded(Renderer2D* renderer)
	{
		assert(m_renderer == nullptr);
		ASSERT_STATIC_CAST(Renderer2D_Imp*, renderer);

		m_renderer = (Renderer2D_Imp*)renderer;
	}

	void CoreEffectObject2D_Imp::OnRemoving(Renderer2D* renderer)
	{
		assert(m_renderer != nullptr);
		ASSERT_STATIC_CAST(Renderer2D_Imp*, renderer);

		for (auto& h : m_handles)
		{
			m_renderer->GetEffectManager()->StopEffect(h);
		}

		m_renderer = nullptr;
	}

	void CoreEffectObject2D_Imp::Draw(Renderer2D* renderer)
	{
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
			Effekseer::Matrix43 efMat = CalcEffectMatrix();

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

		for (size_t i = 0; i < m_handles.size(); i++)
		{
			if (m_renderer->GetEffectManager()->Exists(m_handles[i]))
			{
				m_renderer->AddEffect(m_handles[i], m_drawingPtiority);
			}
		}
	}

	int CoreEffectObject2D_Imp::GetDrawingPriority() const
	{
		return m_drawingPtiority;
	}

	void CoreEffectObject2D_Imp::SetDrawingPriority(int priority)
	{
		m_drawingPtiority = priority;
	}
}
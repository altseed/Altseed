#include "ace.Layer.h"
using namespace std;

namespace ace
{
	void Layer::BeginDrawing()
	{
		m_commonObject->BeginDrawing();
	}

	void Layer::EndDrawing()
	{
		m_commonObject->EndDrawing();

		if (m_postEffects.size() > 0)
		{
			m_commonObject->GetRenderTarget0()->AddRef();
			m_commonObject->GetRenderTarget1()->AddRef();

			auto rt0 = CreateSharedPtrWithReleaseDLL(m_commonObject->GetRenderTarget0());
			auto rt1 = CreateSharedPtrWithReleaseDLL(m_commonObject->GetRenderTarget1());

			int32_t index = 0;
			for (auto& p : m_postEffects)
			{
				if (index % 2 == 0)
				{
					p->OnDraw(rt1, rt0);
				}
				else
				{
					p->OnDraw(rt0, rt1);
				}
				index++;
			}

			m_commonObject->SetTargetToLayer(index % 2);
		}

		m_commonObject->EndDrawingAfterEffects();
	}

	Layer::Layer()
		: m_scene(nullptr)
		, m_isUpdated(true)
		, m_isDrawn(true)
	{
	}

	Layer::~Layer()
	{
		m_commonObject.reset();
	}

	Scene* Layer::GetScene() const
	{
		return m_scene;
	}

	void Layer::SetScene(Scene* value)
	{
		m_scene = value;
	}

	std::shared_ptr<CoreLayer> Layer::GetCoreLayer() const
	{ 
		return m_commonObject; 
	}

	bool Layer::GetIsUpdated() const
	{
		return m_isUpdated;
	}

	void Layer::SetIsUpdated(bool value)
	{
		m_isUpdated = value;
	}

	bool Layer::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void Layer::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
		m_commonObject->SetIsDrawn(value);
	}

	int Layer::GetDrawingPriority() const
	{
		return m_commonObject->GetDrawingPriority();
	}

	void Layer::SetDrawingPriority(int value)
	{
		m_commonObject->SetDrawingPriority(value);
	}

	void Layer::AddPostEffect(const std::shared_ptr<PostEffect>& postEffect)
	{
		m_postEffects.push_back(postEffect);
		m_commonObject->AddPostEffect(postEffect->GetCoreObject());
	}

	void Layer::ClearPostEffects()
	{
		m_postEffects.clear();
		m_commonObject->ClearPostEffects();
	}

}
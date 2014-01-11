
#include "ace.Scene.h"
#include "ace.Layer.h"

using namespace std;

namespace ace
{
	void Layer::BeginDrawing()
	{
		GetScene()->m_coreScene->SetRenderTargetForDrawingLayer();

		m_commonObject->BeginDrawing();
	}

	void Layer::EndDrawing()
	{
		m_commonObject->EndDrawing();

		if (m_postEffects.size() > 0)
		{
			for (auto& p : m_postEffects)
			{
				GetScene()->m_coreScene->BeginPostEffect(p->GetCoreObject());

				GetScene()->m_coreScene->GetSrcTarget()->AddRef();
				GetScene()->m_coreScene->GetDstTarget()->AddRef();

				auto src = CreateSharedPtrWithReleaseDLL(GetScene()->m_coreScene->GetSrcTarget());
				auto dst = CreateSharedPtrWithReleaseDLL(GetScene()->m_coreScene->GetDstTarget());
				
				p->OnDraw(dst, src);

				GetScene()->m_coreScene->EndPostEffect(p->GetCoreObject());
			}
		}
	}

	void Layer::Draw()
	{
		m_commonObject->Draw();
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
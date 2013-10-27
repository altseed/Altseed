#include <list>
#include <exception>
#include "ace.Layer2D.h"
using namespace std;

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Layer2D::Layer2D()
		: m_coreLayer(nullptr)
		, m_objects(list<shared_ptr<Object>>())
		, m_scene(nullptr)
	{
		m_coreLayer = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateLayer2D());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Layer2D::~Layer2D()
	{
		for (auto& object : m_objects)
		{
			object->SetLayer(nullptr);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::Update()
	{
		OnUpdating();

		for (auto& object : m_objects)
		{
			object->Update();
		}

		OnUpdated();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::DrawAdditionally()
	{
		for (auto& object : m_objects)
		{
			object->OnDrawAdditionally();
		}
		OnDrawAdditionally();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::BeginDrawing()
	{
		m_coreLayer->BeginDrawing();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::EndDrawing()
	{
		m_coreLayer->EndDrawing();

		if (m_postEffects.size() > 0)
		{
			m_coreLayer->GetRenderTarget0()->AddRef();
			m_coreLayer->GetRenderTarget1()->AddRef();

			auto rt0 = CreateSharedPtrWithReleaseDLL(m_coreLayer->GetRenderTarget0());
			auto rt1 = CreateSharedPtrWithReleaseDLL(m_coreLayer->GetRenderTarget1());
	
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

			m_coreLayer->SetTargetToLayer(index % 2);
		}

		m_coreLayer->EndDrawingAfterEffects();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::OnUpdating()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::OnUpdated()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::OnDrawAdditionally()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::AddObject(const std::shared_ptr<Object>& object)
	{
		if (object->GetLayer() != nullptr)
		{
			throw "追加しようとしたオブジェクトは、すでに別のレイヤーに所属しています。";
		}
		m_objects.push_back(object);
		auto coreObj = object->GetCoreObject();
		m_coreLayer->AddObject(coreObj);
		object->SetLayer(this);
		object->Start();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::RemoveObject(const std::shared_ptr<Object>& object)
	{
		m_objects.remove(object);
		m_coreLayer->RemoveObject(object->GetCoreObject());
		object->SetLayer(nullptr);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::AddPostEffect(const std::shared_ptr<PostEffect>& postEffect)
	{
		m_postEffects.push_back(postEffect);
		m_coreLayer->AddPostEffect(postEffect->GetCoreObject());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::ClearPostEffects()
	{
		m_postEffects.clear();
		m_coreLayer->ClearPostEffects();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Scene* Layer2D::GetScene() const
	{
		return m_scene;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::SetScene(Scene* value)
	{
		m_scene = value;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	shared_ptr<CoreLayer> Layer2D::GetCoreLayer() const
	{
		return m_coreLayer;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	int Layer2D::GetDrawingPriority() const
	{
		return m_coreLayer->GetDrawingPriority();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Layer2D::SetDrawingPriority(int value)
	{
		m_coreLayer->SetDrawingPriority(value);
	}

}

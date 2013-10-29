#include <exception>
#include "ace.Scene.h"
using namespace std;

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Scene::Scene()
		: m_layers(list<LayerPtr>())
		, m_coreScene(nullptr)
		, m_components(map<astring, ComponentPtr>())
	{
		m_coreScene = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateScene());
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Scene::~Scene()
	{
		for (auto& layer : m_layers)
		{
			layer->SetScene(nullptr);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::Update()
	{
		OnUpdating();

		for (auto& layer : m_layers)
		{
			layer->Update();
		}

		for (auto& component : m_components)
		{
			component.second->Update();
		}

		OnUpdated();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::DrawAdditionally()
	{
		m_layers.sort([](shared_ptr<Layer>& x, shared_ptr<Layer>& y)
		{
			return x->GetDrawingPriority() >= y->GetDrawingPriority();
		});

		for (auto& layer : m_layers)
		{
			layer->DrawAdditionally();
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::BeginDrawing()
	{
		m_layers.sort([](shared_ptr<Layer>& x, shared_ptr<Layer>& y)
		{
			return x->GetDrawingPriority() >= y->GetDrawingPriority();
		});

		for (auto& layer : m_layers)
		{
			layer->BeginDrawing();
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::EndDrawing()
	{
		m_layers.sort([](shared_ptr<Layer>& x, shared_ptr<Layer>& y)
		{
			return x->GetDrawingPriority() >= y->GetDrawingPriority();
		});

		for (auto& layer : m_layers)
		{
			layer->EndDrawing();
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::OnUpdating()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::OnUpdated()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::AddLayer(const LayerPtr& layer)
	{
		if (layer->GetScene() != nullptr)
		{
			throw "追加しようとしたレイヤーは、すでに別のシーンに所属しています。";
		}
		m_layers.push_back(layer);
		m_coreScene->AddLayer(layer->GetCoreLayer().get());
		layer->SetScene(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::RemoveLayer(const LayerPtr& layer)
	{
		m_layers.remove(layer);
		m_coreScene->RemoveLayer(layer->GetCoreLayer().get());
		layer->SetScene(nullptr);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::AddComponent(const ComponentPtr& component, astring key)
	{
		m_components[key] = component;
		component->SetScene(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Scene::ComponentPtr& Scene::GetComponent(astring key)
	{
		return m_components[key];
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::RemoveComponent(astring key)
	{
		auto it = m_components.find(key);
		m_components.erase(it);
		it->second->SetScene(nullptr);
	}
}

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
		: m_layersToDraw(list<LayerPtr>())
		, m_layersToUpdate(list<LayerPtr>())
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
		for (auto& layer : m_layersToDraw)
		{
			layer->SetScene(nullptr);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::UpdateComponents()
	{
		auto beVanished = vector<astring>();
		for (auto& component : m_components)
		{
			component.second->Update();
			if (!component.second->GetIsAlive())
			{
				beVanished.push_back(component.first);
			}
		}

		for (auto& x : beVanished)
		{
			RemoveComponent(x);
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::Update()
	{
		OnUpdating();

		for (auto& layer : m_layersToUpdate)
		{
			layer->BeginUpdateting();
		}

		for (auto& layer : m_layersToUpdate)
		{
			layer->Update();
		}

		for (auto& layer : m_layersToUpdate)
		{
			layer->EndUpdateting();
		}

		UpdateComponents();

		OnUpdated();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::Draw()
	{
		m_layersToDraw.sort([](const Scene::LayerPtr& x, const Scene::LayerPtr& y) -> bool
		{
			return x->GetDrawingPriority() < y->GetDrawingPriority();
		});

		m_coreScene->BeginDrawing();

		for (auto& layer : m_layersToDraw)
		{
			layer->BeginDrawing();
		}

		for (auto& layer : m_layersToDraw)
		{
			layer->Draw();
		}

		for (auto& layer : m_layersToDraw)
		{
			layer->DrawAdditionally();
		}

		for (auto& layer : m_layersToDraw)
		{
			layer->EndDrawing();
		}

		m_coreScene->EndDrawing();
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
		m_layersToDraw.push_back(layer);
		m_layersToUpdate.push_back(layer);
		m_coreScene->AddLayer(layer->GetCoreLayer().get());
		layer->SetScene(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::RemoveLayer(const LayerPtr& layer)
	{
		m_layersToDraw.remove(layer);
		m_layersToUpdate.remove(layer);
		m_coreScene->RemoveLayer(layer->GetCoreLayer().get());
		layer->SetScene(nullptr);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::AddComponent(const ComponentPtr& component, astring key)
	{
		m_components[key] = component;
		component->SetOwner(this);
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
		it->second->SetOwner(nullptr);
		m_components.erase(it);
	}
}

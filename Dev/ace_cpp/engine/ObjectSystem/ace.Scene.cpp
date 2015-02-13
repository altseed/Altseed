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
		: m_layersToDraw(list<Layer::Ptr>())
		, m_layersToUpdate(list<Layer::Ptr>())
		, m_coreScene(nullptr)
		, m_components(map<astring, SceneComponent::Ptr>())
		, m_componentsToBeAdded(map<astring, SceneComponent::Ptr>())
		, alreadyFirstUpdate(false)
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
		for (auto& c : m_componentsToBeAdded)
		{
			m_components.insert(c);
		}
		m_componentsToBeAdded.clear();

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
		executing = true;

		if (!alreadyFirstUpdate)
		{
			OnUpdateForTheFirstTime();
			alreadyFirstUpdate = true;
		}

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

		executing = false;

		CommitChanges();
	}

	void Scene::CommitChanges()
	{
		for (auto layer : addingLayer)
		{
			AddLayer(layer);
		}

		for (auto layer : removingLayer)
		{
			RemoveLayer(layer);
		}
		addingLayer.clear();
		removingLayer.clear();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::Draw()
	{
		executing = true;

		m_layersToDraw.sort([](const Layer::Ptr& x, const Layer::Ptr& y) -> bool
		{
			return x->GetDrawingPriority() < y->GetDrawingPriority();
		});

		for (auto& layer : m_layersToDraw)
		{
			layer->DrawAdditionally();
		}

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
			layer->EndDrawing();
		}

		m_coreScene->EndDrawing();

		executing = false;

		CommitChanges();
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
	void Scene::OnUpdateForTheFirstTime()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::OnTransitionFinished()
	{
	}

	void Scene::OnChanging()
	{
	}

	void Scene::OnDestroy()
	{
	}

	void Scene::CallChanging()
	{
		OnChanging();
	}

	void Scene::CallDestroy()
	{
		OnDestroy();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::CallTransitionFinished()
	{
		OnTransitionFinished();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Scene::GetHDRMode() const
	{
		return m_coreScene->GetHDRMode();
	}

	void Scene::SetHDRMode(bool value)
	{
		m_coreScene->SetHDRMode(value);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::AddLayer(const Layer::Ptr& layer)
	{
		if (executing)
		{
			addingLayer.push_back(layer);
			return;
		}

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
	void Scene::RemoveLayer(const Layer::Ptr& layer)
	{
		if (executing)
		{
			removingLayer.push_back(layer);
			return;
		}

		m_layersToDraw.remove(layer);
		m_layersToUpdate.remove(layer);
		m_coreScene->RemoveLayer(layer->GetCoreLayer().get());
		layer->SetScene(nullptr);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::AddComponent(const SceneComponent::Ptr& component, astring key)
	{
		m_componentsToBeAdded[key] = component;
		component->SetOwner(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	SceneComponent::Ptr& Scene::GetComponent(astring key)
	{
		if (m_components.find(key) != m_components.end())
		{
			return m_components[key];
		}
		else
		{
			return m_componentsToBeAdded[key];
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Scene::RemoveComponent(astring key)
	{
		if (m_components.find(key) != m_components.end())
		{
			m_components[key]->SetOwner(nullptr);
			m_components.erase(key);
			return true;
		}
		else if (m_componentsToBeAdded.find(key) != m_componentsToBeAdded.end())
		{
			m_componentsToBeAdded[key]->SetOwner(nullptr);
			m_componentsToBeAdded.erase(key);
			return true;
		}
		else
		{
			return false;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	std::shared_ptr<RenderTexture2D> Scene::GetEffectedScreen()
	{
		auto target = m_coreScene->GetBaseTarget();
		if (target == nullptr) return std::shared_ptr<RenderTexture2D>();

		SafeAddRef(target);
		return CreateSharedPtrWithReleaseDLL(target);
	}
}

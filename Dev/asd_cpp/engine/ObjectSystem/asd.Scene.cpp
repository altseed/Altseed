#include <exception>
#include <list>
#include "asd.Scene.h"
#include "../asd.Engine.h"
#include "Registration/asd.RegistrationCommand.h"
#include "Registration/asd.EventToManageLayer.h"
#include "Registration/asd.EventToDisposeContent.h"
using namespace std;

namespace asd
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Scene::Scene()
		: m_layersToDraw(list<Layer::Ptr>())
		, m_layersToUpdate(list<Layer::Ptr>())
		, m_coreScene(nullptr)
		, alreadyFirstUpdate(false)
		, m_componentManager(std::make_shared<ComponentManager<SceneComponent>>(this))
		, m_isAlive(true)
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


	// ========
	// 基本イベントハンドラ
	// ========

	void Scene::Dispose()
	{
		Engine::m_changesToCommit.push(make_shared<EventToDisposeContent>(shared_from_this()));
	}

	void Scene::DisposeImmediately()
	{
		if (GetIsAlive())
		{
			m_isAlive = false;
			OnDispose();
			for (auto& component : m_componentManager->GetComponents())
			{
				component.second->RaiseOnDisposed();
			}
			for (auto& l : m_layersToUpdate)
			{
				l->Dispose();
			}
		}
	}

	void Scene::Update()
	{
		m_layersToUpdate.sort([](const Layer::Ptr& x, const Layer::Ptr& y) -> bool
		{
			return x->GetUpdatePriority() < y->GetUpdatePriority();
		});

		OnUpdating();

		for (auto& layer : m_layersToUpdate)
		{
			layer->BeginUpdating();
		}

		for (auto& layer : m_layersToUpdate)
		{
			layer->Update();
		}

		for (auto& layer : m_layersToUpdate)
		{
			layer->EndUpdateting();
		}

		for (auto& component : m_componentManager->GetComponents())
		{
			component.second->Update();
		}

		OnUpdated();
	}

	void Scene::Draw()
	{
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
	}

	void Scene::OnDispose()
	{
	}

	void Scene::OnUpdating()
	{
	}

	void Scene::OnUpdated()
	{
	}

	// ========
	// 登録・登録解除関連イベントハンドラ
	// ========

	void Scene::RaiseOnRegistered()
	{
		OnRegistered();
		for (auto& component : m_componentManager->GetComponents())
		{
			component.second->RaiseOnRegistered();
		}
	}

	void Scene::RaiseOnStartUpdating()
	{
		OnStartUpdating();
		for (auto& component : m_componentManager->GetComponents())
		{
			component.second->RaiseOnStartUpdating();
		}
	}

	void Scene::RaiseOnTransitionFinished()
	{
		OnTransitionFinished();
		for (auto& component : m_componentManager->GetComponents())
		{
			component.second->RaiseOnTransitionFinished();
		}
	}

	void Scene::RaiseOnTransitionBegin()
	{
		OnTransitionBegin();
		for (auto& component : m_componentManager->GetComponents())
		{
			component.second->RaiseOnTransitionBegin();
		}
	}

	void Scene::RaiseOnStopUpdating()
	{
		OnStopUpdating();
		for (auto& component : m_componentManager->GetComponents())
		{
			component.second->RaiseOnStopUpdating();
		}
	}

	void Scene::RaiseOnUnregistered()
	{
		OnUnregistered();
		for (auto& component : m_componentManager->GetComponents())
		{
			component.second->RaiseOnUnregistered();
		}
	}

	void Scene::OnRegistered()
	{
	}

	void Scene::OnStartUpdating()
	{
	}

	void Scene::OnTransitionFinished()
	{
	}

	void Scene::OnTransitionBegin()
	{
	}

	void Scene::OnStopUpdating()
	{
	}

	void Scene::OnUnregistered()
	{
	}


	void Scene::AddLayer(const Layer::Ptr& layer)
	{
		ACE_ASSERT(layer->GetScene() == nullptr, "追加しようとしたレイヤーは、すでに別のシーンに所属しています。");

		asd::Engine::m_changesToCommit.push(
			make_shared<EventToManageLayer>(shared_from_this(), layer, RegistrationCommand::Add, true));
		layer->SetScene(this);
	}

	void Scene::RemoveLayer(const Layer::Ptr& layer)
	{
		asd::Engine::m_changesToCommit.push(
			make_shared<EventToManageLayer>(shared_from_this(), layer, RegistrationCommand::Remove, true));
	}

	void Scene::ImmediatelyAddLayer(const Layer::Ptr& layer, bool raiseEvent)
	{
		m_layersToDraw.push_back(layer);
		m_layersToUpdate.push_back(layer);
		m_coreScene->AddLayer(layer->GetCoreLayer().get());

		if (raiseEvent)
		{
			layer->RaiseOnAdded();
		}
	}

	void Scene::ImmediatelyRemoveLayer(const Layer::Ptr& layer, bool raiseEvent)
	{
		if (raiseEvent)
		{
			layer->RaiseOnRemoved();
		}

		layer->SetScene(nullptr);
		m_layersToDraw.remove(layer);
		m_layersToUpdate.remove(layer);
		m_coreScene->RemoveLayer(layer->GetCoreLayer().get());
	}

	void Scene::AddComponent(const SceneComponent::Ptr& component, astring key)
	{
		m_componentManager->Add(component, key);
	}

	bool Scene::RemoveComponent(astring key)
	{
		return m_componentManager->Remove(key);
	}

	void Scene::ImmediatelyRemoveComponent(astring key)
	{
		m_componentManager->ImmediatelyRemoveComponent(key);
	}

	void Scene::Register(const SceneComponent::Ptr& component)
	{
		component->SetOwner(this);
	}

	void Scene::Unregister(const SceneComponent::Ptr& component)
	{
		component->SetOwner(nullptr);
	}

	const SceneComponent::Ptr& Scene::GetComponent(astring key)
	{
		return m_componentManager->Get(key);
	}


	bool Scene::GetHDRMode() const
	{
		return m_coreScene->GetHDRMode();
	}

	void Scene::SetHDRMode(bool value)
	{
		m_coreScene->SetHDRMode(value);
	}

	std::shared_ptr<RenderTexture2D> Scene::GetEffectedScreen()
	{
		auto target = m_coreScene->GetBaseTarget();
		if (target == nullptr) return std::shared_ptr<RenderTexture2D>();

		SafeAddRef(target);
		return CreateSharedPtrWithReleaseDLL(target);
	}

	const std::list<Layer::Ptr>& Scene::GetLayers() const
	{
		return m_layersToUpdate;
	}

	bool Scene::GetIsAlive() const
	{
		return m_isAlive;
	}
}

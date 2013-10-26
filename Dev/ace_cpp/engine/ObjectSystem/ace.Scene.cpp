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
		: m_layers(list<shared_ptr<Layer>>())
		, m_coreScene(nullptr)
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
	void Scene::AddLayer(const shared_ptr<Layer>& layer)
	{
		if (layer->GetScene() != nullptr)
		{
			//throw invalid_argument( "追加しようとしたレイヤーは、すでに別のシーンに所属しています。" );
			throw "追加しようとしたレイヤーは、すでに別のシーンに所属しています。";
		}
		m_layers.push_back(layer);
		m_coreScene->AddLayer(layer->GetCoreLayer().get());
		layer->SetScene(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Scene::RemoveLayer(const shared_ptr<Layer>& layer)
	{
		m_layers.remove(layer);
		m_coreScene->RemoveLayer(layer->GetCoreLayer().get());
		layer->SetScene(nullptr);
	}
}

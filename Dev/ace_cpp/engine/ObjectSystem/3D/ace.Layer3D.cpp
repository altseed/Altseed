
#include "ace.Layer3D.h"

namespace ace
{

	extern ObjectSystemFactory* g_objectSystemFactory;

	Layer3D::Layer3D()
		: m_coreLayer(nullptr)
	{
		m_coreLayer = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateLayer3D());
		m_commonObject = m_coreLayer;
	}

	Layer3D::~Layer3D()
	{
		for (auto& object : m_objects)
		{
			object->SetLayer(nullptr);
		}
	}

	void Layer3D::Update()
	{
		if (!m_isUpdated)
		{
			return;
		}

		OnUpdating();

		
		auto beVanished = std::vector<ObjectPtr>();

		for (auto& object : m_objects)
		{
			object->Update();
			if (!object->GetIsAlive())
			{
				beVanished.push_back(object);
			}
		}

		for (auto& object : beVanished)
		{
			RemoveObject(object);
		}

		/*
		for (auto& component : m_components)
		{
			component.second->Update();
		}
		*/

		OnUpdated();
	}

	void Layer3D::BeginUpdateting()
	{
		m_coreLayer->BeginUpdating();
	}

	void Layer3D::EndUpdateting()
	{
		m_coreLayer->EndUpdating();
	}

	void Layer3D::DrawAdditionally()
	{
		if (!m_isDrawn)
		{
			return;
		}

		for (auto& object : m_objects)
		{
			object->OnDrawAdditionally();
		}

		OnDrawAdditionally();
	}

	void Layer3D::BeginDrawing()
	{
		m_coreLayer->BeginDrawing();
	}

	void Layer3D::EndDrawing()
	{
		m_coreLayer->EndDrawing();

		/*
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
		*/

		m_coreLayer->EndDrawingAfterEffects();
	}

	void Layer3D::OnUpdating()
	{
	}

	void Layer3D::OnUpdated()
	{
	}

	void Layer3D::OnDrawAdditionally()
	{
	}

	void Layer3D::AddObject(const ObjectPtr& object)
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

	void Layer3D::RemoveObject(const ObjectPtr& object)
	{
		m_objects.remove(object);
		m_coreLayer->RemoveObject(object->GetCoreObject());
		object->SetLayer(nullptr);
	}
};
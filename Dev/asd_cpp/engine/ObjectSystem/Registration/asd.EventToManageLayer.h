#pragma once
#include "ICommitable.h"
#include "../asd.Layer.h"
#include "../asd.Scene.h"
#include "RegistrationCommand.h"

namespace asd
{
	class Scene;

	class EventToManageLayer : public ICommitable
	{
	private:
		asd::Scene::Ptr m_scene;
		Layer::Ptr m_layer;
		RegistrationCommand m_command;
		bool m_raiseEvent;

	public:
		EventToManageLayer(Scene::Ptr scene, Layer::Ptr layer, RegistrationCommand command, bool raiseEvent)
			: m_scene(scene)
			, m_layer(layer)
			, m_command(command)
			, m_raiseEvent(raiseEvent)
		{
		}

		void Commit()
		{
			if (!m_scene->GetIsAlive())
			{
				return;
			}

			switch (m_command)
			{
			case asd::RegistrationCommand::Add:
				m_scene->ImmediatelyAddLayer(m_layer, m_raiseEvent);
				break;

			case asd::RegistrationCommand::Remove:
				m_scene->ImmediatelyRemoveLayer(m_layer, m_raiseEvent);
				break;
			}
		}
	};
}
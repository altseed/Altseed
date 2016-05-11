#pragma once
#include "asd.ICommitable.h"
#include "../asd.Layer.h"
#include "../asd.Scene.h"
#include "asd.RegistrationCommand.h"

namespace asd
{
	class Scene;
	class Layer;

	class EventToManageLayer : public ICommitable
	{
	private:
		std::shared_ptr<Scene> m_scene;
		std::shared_ptr<Layer> m_layer;
		RegistrationCommand m_command;
		bool m_raiseEvent;

	public:
		EventToManageLayer(std::shared_ptr<Scene> scene, std::shared_ptr<Layer> layer, RegistrationCommand command, bool raiseEvent)
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
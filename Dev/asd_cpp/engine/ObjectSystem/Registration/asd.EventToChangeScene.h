#pragma once
#include <memory>
#include "ICommitable.h"
#include "../../asd.Engine.h"
#include "../asd.Scene.h"
#include "../Transition/asd.Transition.h"

namespace asd
{
	class Scene;
	class Transition;

	class EventToChangeScene : public ICommitable
	{
	private:
		std::shared_ptr<Scene> m_nextScene;
		std::shared_ptr<Transition> m_transition;
		bool m_doAutoDispose;

	public:
		EventToChangeScene(std::shared_ptr<Scene> scene, std::shared_ptr<Transition> transition, bool doAutoDispose)
			: m_nextScene(scene)
			, m_transition(transition)
			, m_doAutoDispose(doAutoDispose)
		{
		}

		void Commit()
		{
			if (Engine::GetCurrentScene() != nullptr)
			{
				Engine::GetCurrentScene()->RaiseOnTransitionBegin();
			}
			if (m_nextScene != nullptr)
			{
				m_nextScene->RaiseOnRegistered();
			}
			Engine::m_transitionState->ForceToComplete();

			if (m_transition != nullptr)
			{
				Engine::m_transitionState = std::make_shared<Engine::FadingOutState>(m_transition, m_nextScene, m_doAutoDispose);
			}
			else
			{
				Engine::m_transitionState = std::make_shared<Engine::QuicklyChangingState>(m_nextScene, m_doAutoDispose);
			}
		}
	};
}
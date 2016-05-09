#pragma once
#include <memory>
#include "ICommitable.h"
#include "../../asd.Engine.h"
#include "../asd.Scene.h"
#include "../Transition/asd.Transition.h"

namespace asd
{
	class EventToChangeScene : public ICommitable
	{
	private:
		Scene::Ptr m_nextScene;
		Transition::Ptr m_transition;
		bool m_doAutoDispose;

	public:
		EventToChangeScene(Scene::Ptr scene, Transition::Ptr transition, bool doAutoDispose)
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
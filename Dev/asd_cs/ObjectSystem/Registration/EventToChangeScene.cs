using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	class EventToChangeScene : ICommitable
	{
		public EventToChangeScene(Scene nextScene, Transition transition, bool doAutoDispose)
		{
			NextScene = nextScene;
			Transition = transition;
			DoAutoDispose = doAutoDispose;
		}

		private Scene NextScene { get; set; }
		private Transition Transition { get; set; }
		private bool DoAutoDispose { get; set; }

		public void Commit()
		{
			if (Engine.CurrentScene != null)
			{
				Engine.CurrentScene.RaiseOnTransitionBegin();
			}
			if (NextScene != null)
			{
				NextScene.RaiseOnRegistered();
			}
			Engine.transitionState.ForceToComplete();

			if (Transition != null)
			{
				Engine.transitionState = new FadingOutState(Transition, NextScene, DoAutoDispose);
			}
			else
			{
				Engine.transitionState = new QuicklyChangingState(NextScene, DoAutoDispose);
			}
		}
	}
}

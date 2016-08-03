using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	abstract class SceneTransitionState
	{
		/// <summary>
		/// シーン遷移に関する処理を進行させる。
		/// </summary>
		public virtual void Proceed()
		{
		}
		/// <summary>
		/// シーン遷移処理を更新する。
		/// </summary>
		public virtual void Update()
		{
		}
		/// <summary>
		/// シーンを描画する。
		/// </summary>
		public abstract void Draw();
		/// <summary>
		/// この遷移状態で行うべき処理を強制的に完了させる。
		/// </summary>
		public virtual void ForceToComplete()
		{
		}

		protected bool IsValid(Scene scene)
		{
			return scene != null && scene.IsAlive;
		}
	}

	class NeutralState : SceneTransitionState
	{
		public override void Draw()
		{
			if(Engine.CurrentScene != null)
			{
				Engine.core.DrawSceneToWindow(Engine.CurrentScene.CoreInstance);
			}
		}

		public override void Proceed()
		{
			if(Engine.CurrentScene != null && !Engine.CurrentScene.IsAlive)
			{
				Engine.transitionState = new QuicklyChangingState(null, false);
			}
		}
	}

	class FadingOutState : SceneTransitionState
	{
		private asd.Transition transition;
		private bool doAutoDispose;

		public FadingOutState(asd.Transition transition, Scene nextScene, bool doAutoDispose)
		{
			this.transition = transition;
			Engine.nextScene = nextScene;
			this.doAutoDispose = doAutoDispose;
		}

		public override void Proceed()
		{
			if(transition.IsSceneChanged)
			{
				if(IsValid(Engine.CurrentScene))
				{
					Engine.CurrentScene.RaiseOnStopUpdating();
				}
				if(IsValid(Engine.nextScene))
				{
					Engine.nextScene.RaiseOnStartUpdating();
					Engine.core.ChangeScene(Engine.nextScene.CoreInstance);
				}
				else
				{
					Engine.core.ChangeScene(null);
				}
				Engine.transitionState = new FadingInState(transition, Engine.CurrentScene, doAutoDispose);
				Engine.CurrentScene = Engine.nextScene;
				Engine.nextScene = null;
			}
		}

		public override void Update()
		{
			transition.OnUpdate();
		}

		public override void Draw()
		{
			swig.CoreScene curScene = null;
			if(Engine.CurrentScene != null)
			{
				curScene = Engine.CurrentScene.CoreInstance;
			}

			Engine.core.DrawSceneToWindowWithTransition(null, curScene, transition.coreInstance);
		}
	}

	class FadingInState : SceneTransitionState
	{
		private asd.Transition transition;
		private Scene previousScene;
		private bool doAutoDispose;

		public FadingInState(asd.Transition transition, Scene previousScene, bool doAutoDispose)
		{
			this.transition = transition;
			this.previousScene = previousScene;
			this.doAutoDispose = doAutoDispose;
		}

		public override void Proceed()
		{
			if(transition.IsFinished)
			{
				if(IsValid(previousScene))
				{
					previousScene.RaiseOnUnregistered();
					if(doAutoDispose)
					{
						previousScene.Dispose();
					}
				}
				if(IsValid(Engine.CurrentScene))
				{
					Engine.CurrentScene.RaiseOnTransitionFinished();
				}
				Engine.transitionState = new NeutralState();
			}
		}

		public override void Update()
		{
			transition.OnUpdate();
		}

		public override void Draw()
		{
			swig.CoreScene curScene = null;
			swig.CoreScene prevScene = null;

			if(Engine.CurrentScene != null)
			{
				curScene = Engine.CurrentScene.CoreInstance;
			}

			if(previousScene != null)
			{
				prevScene = previousScene.CoreInstance;
			}

			Engine.core.DrawSceneToWindowWithTransition(curScene, prevScene, transition.coreInstance);
		}

		public override void ForceToComplete()
		{
			if(IsValid(previousScene) && doAutoDispose)
			{
				previousScene.Dispose();
			}
		}
	}

	class QuicklyChangingState : SceneTransitionState
	{
		private bool doAutoDispose;

		public QuicklyChangingState(Scene nextScene, bool doAutoDispose)
		{
			Engine.nextScene = nextScene;
			this.doAutoDispose = doAutoDispose;
		}

		public override void Proceed()
		{
			ForceToComplete();
			Engine.transitionState = new NeutralState();
		}

		public override void Draw()
		{
			if(Engine.CurrentScene != null && Engine.CurrentScene.IsAlive)
			{
				Engine.core.DrawSceneToWindow(Engine.CurrentScene.CoreInstance);
			}
		}

		public override void ForceToComplete()
		{
			if(IsValid(Engine.CurrentScene))
			{
				Engine.CurrentScene.RaiseOnStopUpdating();
				Engine.CurrentScene.RaiseOnUnregistered();
				if(doAutoDispose)
				{
					Engine.CurrentScene.Dispose();
				}
			}
			if(IsValid(Engine.nextScene))
			{
				Engine.nextScene.RaiseOnStartUpdating();
				Engine.nextScene.RaiseOnTransitionFinished();
				Engine.core.ChangeScene(Engine.nextScene.CoreInstance);
			}
			else
			{
				Engine.core.ChangeScene(null);
			}
			Engine.CurrentScene = Engine.nextScene;
		}
	}
}

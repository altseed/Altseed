using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class TimingOfRegisteringLayer : EngineTest
	{
		class CustomLayer : Layer2D
		{
			private bool onAddedHaveBeenInvoked;
			private bool onRemovedHaveBeenInvoked;
			private int time;

			protected override void OnAdded()
			{
				if (Scene == null)
				{
					throw new ApplicationException("Sceneがnullです。");
				}
				if (onAddedHaveBeenInvoked)
				{
					throw new ApplicationException("OnAddedが２回呼ばれました。");
				}
				onAddedHaveBeenInvoked = true;
				Console.WriteLine("OnAdded");
			}

			protected override void OnRemoved()
			{
				if (onRemovedHaveBeenInvoked)
				{
					throw new ApplicationException("OnRemovedが２回呼ばれました。");
				}
				onRemovedHaveBeenInvoked = true;
				Console.WriteLine("OnRemoved");
			}

			protected override void OnUpdated()
			{
				++time;
				if (time == 20)
				{
					Scene.AddLayer(new CustomLayer());
					Scene.RemoveLayer(this);
				}
			}
		}

		public TimingOfRegisteringLayer() : base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new CustomLayer();

			Engine.ChangeScene(scene);
			scene.AddLayer(layer);
		}
	}
}

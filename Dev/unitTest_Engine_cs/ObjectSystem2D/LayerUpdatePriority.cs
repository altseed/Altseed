using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class LayerUpdatePriority : EngineTest
	{
		class MessageLayer : Layer2D
		{
			private string message;

			public MessageLayer(string message)
			{
				this.message = message;
			}

			protected override void OnUpdating()
			{
				Console.WriteLine(message);
				Vanish();
			}
		}

		public LayerUpdatePriority()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			Engine.ChangeScene(scene);

			var layer1 = new MessageLayer("Layer1");
			layer1.UpdatePriority = 1;
			scene.AddLayer(layer1);

			var layer2 = new MessageLayer("Layer2");
			layer2.UpdatePriority = 0;
			scene.AddLayer(layer2);
		}
	}
}

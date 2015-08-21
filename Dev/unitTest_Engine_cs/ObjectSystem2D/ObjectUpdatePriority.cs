using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class ObjectUpdatePriority : EngineTest
	{
		class MessageObject : TextureObject2D
		{
			private string message;

			public MessageObject(string message)
			{
				this.message = message;
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath);
			}

			protected override void OnUpdate()
			{
				Console.WriteLine(message);
				Vanish();
			}
		}

		public ObjectUpdatePriority()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var obj1 = new MessageObject("object1");
			obj1.UpdatePriority = 2;
			Engine.AddObject2D(obj1);

			var obj2 = new MessageObject("object2");
			obj2.UpdatePriority = 1;
			Engine.AddObject2D(obj2);

			var obj3 = new MessageObject("object3");
			Engine.AddObject2D(obj3);
			obj3.UpdatePriority = 0;
		}
	}
}

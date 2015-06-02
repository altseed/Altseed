using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class TransformOutOfUpdate : EngineTest
	{
		private TextureObject2D obj;
		private IEnumerator<object> coroutine;

		public TransformOutOfUpdate()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			obj = new TextureObject2D()
			{
				Position = new Vector2DF(200, 200),
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath),
			};
			Engine.AddObject2D(obj);

			var child = new TextureObject2D()
			{
				Position = new Vector2DF(10, 10),
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath),
			};
            obj.AddChild(child, ChildMode.Position);
			Engine.AddObject2D(child);

			coroutine = GetFlow();
        }

		private IEnumerator<object> GetFlow()
		{
			while(true)
			{
				obj.Position += new Vector2DF(1, 0);
				yield return null;
			}
		}

		protected override void OnUpdated()
		{
			coroutine.MoveNext();
		}
	}
}

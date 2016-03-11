using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.BugFix
{
	class TransformOutOfUpdate : EngineTest
	{
		private TextureObject2D obj;
		private TextureObject2D child;

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

			child = new TextureObject2D()
			{
				Position = new Vector2DF(10, 10),
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath),
			};
            obj.AddChild(child, ChildManagementMode.Nothing, ChildTransformingMode.Position);
			Engine.AddObject2D(child);

			coroutine = GetFlow();
        }

		private IEnumerator<object> GetFlow()
		{
			int loop = 0;

			while(true)
			{
				obj.Position += new Vector2DF(1, 0);
				
				loop++;
				
				if(loop < 9)
				{
					child.Position += new Vector2DF(1, 0);
				}
				if (loop == 10)
				{
					child.Dispose();
				}

				yield return null;
			}
		}

		protected override void OnUpdated()
		{
			coroutine.MoveNext();
		}
	}
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._2D
{
	class SetPositionWhenFontIsNull : EngineTest
	{
		public SetPositionWhenFontIsNull()
			: base(30)
		{
		}

		protected override void OnStart()
		{
			var scene = new asd.Scene();
			var layer = new asd.Layer2D();
			var obj = new asd.TextObject2D();

			asd.Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			layer.AddObject(obj);

			var font = asd.Engine.Graphics.CreateFont("Data/Font/Nac0812.aff");

			obj.Text = "DependencyPropertyは\n依存関係プロパティのこと";
			obj.Font = font;

			//object->SetCenterPosition(Vector2DF(128, 128));
			obj.Position = new asd.Vector2DF(0, 0);
		}
	}
}

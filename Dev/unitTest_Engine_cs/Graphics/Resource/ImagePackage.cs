using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics.Resource
{
	class ImagePackage : EngineTest
	{
		public ImagePackage()
			: base(10)
		{
		}

		protected override void OnStart()
		{
			var scene = new asd.Scene();
			var layer = new asd.Layer2D();
			var imagePackage = asd.Engine.Graphics.CreateImagePackage("Data/ImagePackage/test.aip");
	
			for (int  i = 0; i < imagePackage.ImageCount; i++)
			{
				var obj = new asd.TextureObject2D();
	
				obj.Texture = imagePackage.GetImage(i);
				obj.Position = new asd.Vector2DF(imagePackage.GetImageArea(i).X, imagePackage.GetImageArea(i).Y);
				layer.AddObject(obj);
			}

			scene.AddLayer(layer);
			asd.Engine.ChangeScene(scene);
		}
	}
}

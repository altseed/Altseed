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
			var scene = new ace.Scene();
			var layer = new ace.Layer2D();
			var imagePackage = ace.Engine.Graphics.CreateImagePackage("Data/ImagePackage/test.aip");
	
			for (int  i = 0; i < imagePackage.ImageCount; i++)
			{
				var obj = new ace.TextureObject2D();
	
				obj.Texture = imagePackage.GetImage(i);
				obj.Position = new ace.Vector2DF(imagePackage.GetImageArea(i).X, imagePackage.GetImageArea(i).Y);
				layer.AddObject(obj);
			}

			scene.AddLayer(layer);
			ace.Engine.ChangeScene(scene);
		}
	}
}

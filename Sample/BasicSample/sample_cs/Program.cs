using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace sample_cs
{
	class Program
	{
		static void Main(string[] args)
		{
			ISample[] samples = 
			{
				new Basic_Empty(),

                new Geometry2D_ArcShape(),
                new Geometry2D_PolygonShape(),
                new Geometry2D_RectangleShape(),

                new PostEffect_GrayScale(),
                new PostEffect_Sepia(),
                new PostEffect_GaussianBlur(),
                new PostEffect_LightBloom(),

                new Keyboard(),
                new Mouse(),
                new Joystick(),

				new Sound(),

				new Object2DComponent(),
                new Transition(),

                new CameraObject2D(),
                new EffectObject2D(),
                new TextObject2D(),
                new TextureObject2D(),
                new MapObject2D(),
				new ImagePackage(),
                new GeometryObject2D(),
                
                new Collision2D(),
            };

			var cui = new SampleGuide( samples );
			cui.Show();
		}
	}
}

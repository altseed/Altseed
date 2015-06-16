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

				new TextureObject2D_Simple(),
                new TextureObject2D_Turn(),
                new TextureObject2D_Src(),
				new TextureObject2D_Transform(),
                
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

                new TextObject2D_Simple(),
                new TextObject2D_Rotate(),
                new TextObject2D_Filtered(),

                new MapObject2D_Simple(),
                new MapObject2D_Camera(),

                new GeometryObject2D_ArcShape(),
                new GeometryObject2D_PolygonShape(),
                new GeometryObject2D_RectangleShape(),


                new ImagePackage(),

				new ImagePackage(),
                new Collision2D(),
            };

			var cui = new SampleGuide( samples );
			cui.Show();
		}
	}
}

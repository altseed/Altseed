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

                new SceneAndLayer_Basic(),

				new PostEffect_GrayScale(),
				new PostEffect_Sepia(),
				new PostEffect_GaussianBlur(),
				new PostEffect_LightBloom(),

				new Transition_Fade(),

                new Mouse_Position(),
                new Mouse_Click(),
                new Mouse_State(),
                new Mouse_Wheel(),

                new Keyboard_Basic(),
                new Keyboard_SingularKey(),
                new Keyboard_PuralKeys(),

                new Joystick_Basic(),
                new Joystick_Axis(),

                new Sound_SE(),
				new Sound_BGM(),
				new Sound_Loop(),

				new Object2DComponent(),

                new CameraObject2D(),
                new EffectObject2D_Simple(),

                new TextObject2D_Simple(),
                new TextObject2D_Rotate(),
                new TextObject2D_Filtered(),

                new TextureObject2D_Simple(),
                new TextureObject2D_Transform(),
                new TextureObject2D_Turn(),
                new TextureObject2D_Src(),

                new MapObject2D_Simple(),
                new MapObject2D_Camera(),

                new GeometryObject2D_CircleShape(),
                new GeometryObject2D_ArcShape(),
                new GeometryObject2D_RectangleShape(),
                new GeometryObject2D_PolygonShape(),
                new GeometryObject2D_PolygonShape_Textured(),

                new Collision2D_Basic(),
                new Collision2D_Geometries(),
            };

			var cui = new SampleGuide( samples );
			cui.Show();
		}
	}
}

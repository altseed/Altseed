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
				new Window.Empty(),
				new Window.EmptyExternal(),

				new Log(),

                new Input.Keyboard(),
                new Input.Mouse(),
                new Input.Joystick(),

				new Sound.Sound(),

				new ObjectSystem.Object2DComponent(),
                new ObjectSystem.Transition(),

                new Graphics._2D.CameraObject2D(),
                new Graphics._2D.EffectObject2D(),
                new Graphics._2D.TextObject2D(),
                new Graphics._2D.TextureObject2D(),
                new Graphics._2D.MapObject2D(),

                new Graphics.PostEffect.PostEffect(),
				new Graphics.PostEffect.CustomPostEffect(),
			};

			var cui = new SampleGuide( samples );
			cui.Show();
		}
	}
}

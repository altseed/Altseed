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
				
				new Log(),

                new Keyboard(),
                new Mouse(),
                new Joystick(),

				new Sound.Sound(),

				new ObjectSystem.Object2DComponent(),
                new ObjectSystem.Transition(),

                new CameraObject2D(),
                new EffectObject2D(),
                new TextObject2D(),
                new TextureObject2D(),
                new MapObject2D(),
				new ImagePackage(),

                new PostEffect(),
			};

			var cui = new SampleGuide( samples );
			cui.Show();
		}
	}
}

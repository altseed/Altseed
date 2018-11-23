using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics.Media
{
	class MediaPlayer : EngineTest
	{
		asd.MediaPlayer mp = null;
		asd.Texture2D texture;

		public MediaPlayer()
			: base(20)
		{
		}

		protected override void OnStart()
		{
			var scene = new asd.Scene();
			var layer = new asd.Layer2D();
			var textureObj1 = new asd.TextureObject2D();
			texture = asd.Engine.Graphics.CreateEmptyTexture2D(1280, 720, asd.TextureFormat.R8G8B8A8_UNORM_SRGB);

			layer.AddObject(textureObj1);
			scene.AddLayer(layer);
			asd.Engine.ChangeScene(scene);

			textureObj1.Texture = texture;

			mp = asd.Engine.Graphics.CreateMediaPlayer();
			mp.Load("Data/Movie/Sample1.mp4");
			mp.Play(false);
		}

		protected override void OnUpdating()
		{
			mp.WriteToTexture2D(texture);
		}
	}
}
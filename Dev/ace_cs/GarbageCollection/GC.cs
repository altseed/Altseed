using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class GC
	{
		internal static GarbageCollector Collector { get; private set; }
		internal static IDObjectContainer<Texture2D> Texture2Ds { get; private set; }
		internal static IDObjectContainer<Shader2D> Shader2Ds { get; private set; }
		internal static IDObjectContainer<Material2D> Material2Ds { get; private set; }

		internal static IDObjectContainer<Scene> Scenes { get; private set; }
		internal static IDObjectContainer<Layer2D> Layer2Ds { get; private set; }
		internal static IDObjectContainer<TextureObject2D> TextureObject2Ds { get; private set; }
		internal static IDObjectContainer<PostEffect> PostEffects { get; private set; }

		//internal static IDObjectContainer<Profiler> Profilers { get; private set; }

		internal static void Initialize()
		{
			Collector = new GarbageCollector();
			Texture2Ds = new IDObjectContainer<Texture2D>();
			Shader2Ds = new IDObjectContainer<Shader2D>();
			Material2Ds = new IDObjectContainer<Material2D>();
			Scenes = new IDObjectContainer<Scene>();
			Layer2Ds = new IDObjectContainer<Layer2D>();
			TextureObject2Ds = new IDObjectContainer<TextureObject2D>();
			PostEffects = new IDObjectContainer<PostEffect>();
			//Profilers = new IDObjectContainer<Profiler>();
		}

		internal static void Update()
		{
			Collector.Collect();
		}

		internal static void Terminate()
		{
			for (int loop = 0; loop < 3; loop++)
			{
				Texture2Ds.DestroyAll();
				Shader2Ds.DestroyAll();
				Material2Ds.DestroyAll();
				Scenes.DestroyAll();
				Layer2Ds.DestroyAll();
				TextureObject2Ds.DestroyAll();
				PostEffects.DestroyAll();
				//Profilers.DestroyAll();

				Collector.Collect();
				System.GC.Collect();
				System.GC.WaitForPendingFinalizers();
				System.GC.Collect();
				Collector.Collect();
			}
		}
	}
}

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

		internal static IDObjectContainer<Mesh> Meshs { get; private set; }

		internal static IDObjectContainer<Scene> Scenes { get; private set; }

		internal static IDObjectContainer<Layer2D> Layer2Ds { get; private set; }
		internal static IDObjectContainer<TextureObject2D> TextureObject2Ds { get; private set; }

		internal static IDObjectContainer<Layer3D> Layer3Ds { get; private set; }
		internal static IDObjectContainer<Object3D> Object3Ds { get; private set; }

		internal static IDObjectContainer<PostEffect> PostEffects { get; private set; }

		internal static void Initialize()
		{
			Collector = new GarbageCollector();
			Texture2Ds = new IDObjectContainer<Texture2D>();
			Shader2Ds = new IDObjectContainer<Shader2D>();
			Material2Ds = new IDObjectContainer<Material2D>();
			Meshs = new IDObjectContainer<Mesh>();

			Scenes = new IDObjectContainer<Scene>();

			Layer2Ds = new IDObjectContainer<Layer2D>();
			TextureObject2Ds = new IDObjectContainer<TextureObject2D>();
			Object3Ds = new IDObjectContainer<Object3D>();

			Layer3Ds = new IDObjectContainer<Layer3D>();

			PostEffects = new IDObjectContainer<PostEffect>();
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
				Meshs.DestroyAll();

				Scenes.DestroyAll();

				Layer2Ds.DestroyAll();
				TextureObject2Ds.DestroyAll();

				Layer3Ds.DestroyAll();
				Object3Ds.DestroyAll();

				PostEffects.DestroyAll();
				//Profilers.DestroyAll();

				Collector.Collect();
				System.GC.Collect();
				System.GC.WaitForPendingFinalizers();
				System.GC.Collect();
				Collector.Collect();
			}
		}

		/// <summary>
		/// C++のインスタンスの生成及び取得方法
		/// </summary>
		public enum GenerationType
		{
			/// <summary>
			/// ファイルパス等からC++のインスタンスが生成された場合
			/// 生成時に使い回しをしている場合、カウンタを増やしているので、
			/// 使い回しと判定した場合、カウンタを減らす
			/// </summary>
			Create,

			/// <summary>
			/// C++のインスタンスを取得した場合、
			/// 新規の場合はC#側でC++のインスタンスを保持するのでカウンタを増やす
			/// </summary>
			Get,
		}

		/// <summary>
		/// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
		/// </summary>
		/// <param name="o"></param>
		/// <param name="type"></param>
		internal static Texture2D GenerateTexture2D(swig.Texture2D o, GenerationType type)
		{
			var p = o.GetPtr();

			var existing = GC.Texture2Ds.GetObject(p);
			if (existing != null)
			{
				if (type == GenerationType.Create)
				{
					o.Release();
				}

				return existing;
			}

			if (type == GenerationType.Get)
			{
				o.AddRef();
			}

			var ret = new Texture2D(o);
			GC.Texture2Ds.AddObject(p, ret);
			return ret;
		}

		/// <summary>
		/// ネイティブのインスタンスからラッパー側のインスタンスを生成する。
		/// </summary>
		/// <param name="o"></param>
		/// <param name="type"></param>
		internal static RenderTexture2D GenerateRenderTexture2D(swig.RenderTexture2D o, GenerationType type)
		{
			var p = o.GetPtr();

			var existing = GC.Texture2Ds.GetObject(p);
			if (existing != null)
			{
				if (type == GenerationType.Create)
				{
					o.Release();
				}

				return (RenderTexture2D)existing;
			}

			if (type == GenerationType.Get)
			{
				o.AddRef();
			}

			var ret = new RenderTexture2D(o);
			GC.Texture2Ds.AddObject(p, ret);
			return ret;
		}

		internal static Mesh GenerateMesh(swig.Mesh o)
		{
			var p = o.GetPtr();

			var existing = GC.Meshs.GetObject(p);
			if (existing != null)
			{
				return existing;
			}

			var ret = new Mesh(o);
			GC.Meshs.AddObject(p, ret);
			return ret;
		}

	}
}

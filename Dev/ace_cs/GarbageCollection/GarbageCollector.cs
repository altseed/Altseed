using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 実際にネイティブのオブジェクトのGCを行うクラス
	/// </summary>
	class GarbageCollector
	{
		List<object> objects = new List<object>();

		/// <summary>
		/// 破棄するインスタンスを追加する。
		/// </summary>
		/// <param name="o">インスタンス</param>
		public void AddObject(object o)
		{
			lock(objects)
			{ 
				objects.Add(o);
			}
		}

		/// <summary>
		/// インスタンスを破棄する。
		/// </summary>
		public void Collect()
		{
			lock (objects)
			{
				foreach (var o in objects)
				{
					if (TryRelease<swig.SoundSource>(o)) continue;

					if (TryRelease<swig.Texture2D>(o)) continue;
					if (TryRelease<swig.Texture2D_Imp>(o)) continue;
					if (TryRelease<swig.RenderTexture2D>(o)) continue;
					if (TryRelease<swig.CubemapTexture>(o)) continue;
					if (TryRelease<swig.Shader2D>(o)) continue;
					if (TryRelease<swig.Shader3D>(o)) continue;
					if (TryRelease<swig.Material2D>(o)) continue;
					if (TryRelease<swig.Material3D>(o)) continue;
					if (TryRelease<swig.MaterialPropertyBlock>(o)) continue;

					if (TryRelease<swig.ImagePackage>(o)) continue;

					if (TryRelease<swig.Effect>(o)) continue;
                    if (TryRelease<swig.Chip2D>(o)) continue;
                    if (TryRelease<swig.Font>(o)) continue;

					if (TryRelease<swig.Mesh>(o)) continue;
					if (TryRelease<swig.Deformer>(o)) continue;
					if (TryRelease<swig.Model>(o)) continue;
					if (TryRelease<swig.MassModel>(o)) continue;
					if (TryRelease<swig.Terrain3D>(o)) continue;

					if (TryRelease<swig.KeyframeAnimation>(o)) continue;
					if (TryRelease<swig.AnimationSource>(o)) continue;
					if (TryRelease<swig.AnimationClip>(o)) continue;


					if (TryRelease<swig.CoreScene>(o)) continue;

					if (TryRelease<swig.CoreLayer2D>(o)) continue;
					if (TryRelease<swig.CoreObject2D>(o)) continue;
					if (TryRelease<swig.CoreTextureObject2D>(o)) continue;
                    if (TryRelease<swig.CoreTextObject2D>(o)) continue;
                    if (TryRelease<swig.CoreMapObject2D>(o)) continue;

					if (TryRelease<swig.CoreLayer3D>(o)) continue;
					if (TryRelease<swig.CoreObject3D>(o)) continue;

					if (TryRelease<swig.CorePostEffect>(o)) continue;

					if (TryRelease<swig.CoreTransition>(o)) continue;

                    if (TryRelease<swig.StaticFile>(o)) continue;
					if (TryRelease<swig.StreamFile>(o)) continue;

                    if (TryRelease<swig.CoreShape>(o)) continue;

					Particular.Helper.ThrowUnregisteredClassException(o);
				}

				objects.Clear();
			}
		}

		private bool TryRelease<T>( object obj ) where T : swig.IReference
		{
			var o = obj as T;
			if( o != null )
			{
				o.Release();
				return true;
			}
			return false;
		}
	}
}

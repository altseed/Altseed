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
					if (TryRelease<swig.Shader2D>(o)) continue;
					if (TryRelease<swig.Material2D>(o)) continue;

					if (TryRelease<swig.Effect>(o)) continue;

					if (TryRelease<swig.Mesh>(o)) continue;
					if (TryRelease<swig.Deformer>(o)) continue;
					if (TryRelease<swig.Model>(o)) continue;

					if (TryRelease<swig.KeyframeAnimation>(o)) continue;
					if (TryRelease<swig.AnimationSource>(o)) continue;
					if (TryRelease<swig.AnimationClip>(o)) continue;


					if (TryRelease<swig.CoreScene>(o)) continue;

					if (TryRelease<swig.CoreLayer2D>(o)) continue;
					if (TryRelease<swig.CoreObject2D>(o)) continue;
					if (TryRelease<swig.CoreTextureObject2D>(o)) continue;

					if (TryRelease<swig.CoreLayer3D>(o)) continue;
					if (TryRelease<swig.CoreObject3D>(o)) continue;

					if (TryRelease<swig.CorePostEffect>(o)) continue;
					throw new Exception(string.Format("未登録のクラス{0}を検出しました。", o));
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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
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
            lock (objects)
            {
                objects.Add(o);
            }
        }

        /// <summary>
        /// インスタンスを破棄する。
        /// </summary>
        public bool Collect()
        {
            lock (objects)
            {
                if (objects.Count == 0)
                {
                    return false; 
                }

                foreach (var o in objects)
                {
					if (TryRelease(o as swig.SoundSource)) continue;

					if (TryRelease(o as swig.Cursor)) continue;

					if (TryRelease(o as swig.Texture2D)) continue;
					if (TryRelease(o as swig.Texture2D_Imp)) continue;
					if (TryRelease(o as swig.RenderTexture2D)) continue;
					if (TryRelease(o as swig.CubemapTexture)) continue;
					if (TryRelease(o as swig.Shader2D)) continue;
					if (TryRelease(o as swig.Shader3D)) continue;
					if (TryRelease(o as swig.Material2D)) continue;
					if (TryRelease(o as swig.Material3D)) continue;
					if (TryRelease(o as swig.MaterialPropertyBlock)) continue;

					if (TryRelease(o as swig.ImagePackage)) continue;

					if (TryRelease(o as swig.Effect)) continue;
					if (TryRelease(o as swig.Font)) continue;

					if (TryRelease(o as swig.Mesh)) continue;
					if (TryRelease(o as swig.Deformer)) continue;
					if (TryRelease(o as swig.Model)) continue;
					if (TryRelease(o as swig.MassModel)) continue;
					if (TryRelease(o as swig.Terrain3D)) continue;

					if (TryRelease(o as swig.KeyframeAnimation)) continue;
					if (TryRelease(o as swig.AnimationSource)) continue;
					if (TryRelease(o as swig.AnimationClip)) continue;


					if (TryRelease(o as swig.CoreScene)) continue;

					if (TryRelease(o as swig.CoreChip2D)) continue;
					if (TryRelease(o as swig.CoreLayer2D)) continue;
					if (TryRelease(o as swig.CoreObject2D)) continue;
					if (TryRelease(o as swig.CoreTextureObject2D)) continue;
					if (TryRelease(o as swig.CoreTextObject2D)) continue;
					if (TryRelease(o as swig.CoreMapObject2D)) continue;

					if (TryRelease(o as swig.CoreLayer3D)) continue;
					if (TryRelease(o as swig.CoreObject3D)) continue;

					if (TryRelease(o as swig.CorePostEffect)) continue;

					if (TryRelease(o as swig.CoreTransition)) continue;

					if (TryRelease(o as swig.StaticFile)) continue;
					if (TryRelease(o as swig.StreamFile)) continue;

					if (TryRelease(o as swig.CoreShape)) continue;

                    Particular.Helper.ThrowUnregisteredClassException(o);
                }

                objects.Clear();

                return true;
            }
        }

        private bool TryRelease<T>(T obj) where T : swig.IReference
        {
            var o = obj;
            if (o != null)
            {
                o.Release();
                return true;
            }
            return false;
        }
    }
}

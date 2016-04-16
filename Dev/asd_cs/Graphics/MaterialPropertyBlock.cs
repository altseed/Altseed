using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// マテリアルへのパラメーターの集合
    /// </summary>
    public class MaterialPropertyBlock : IReleasable
    {
        internal swig.MaterialPropertyBlock CoreInstance { get; set; }

        internal MaterialPropertyBlock(swig.MaterialPropertyBlock coreInstance)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.MaterialPropertyBlocks.Contains(coreInstance.GetPtr()))
            {
                Particular.Helper.ThrowException("");
            }
#endif
            CoreInstance = coreInstance;
        }

        ~MaterialPropertyBlock()
        {
            ForceToRelease();
        }

        public bool IsReleased
        {
            get
            {
                return CoreInstance == null;
            }
        }

        /// <summary>
        /// 強制的に使用しているメモリを開放する。
        /// </summary>
        /// <remarks>
        /// 何らかの理由でメモリが不足した場合に実行する。
        /// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
        /// </remarks>
        public void ForceToRelease()
        {
            lock (this)
            {
                if (CoreInstance == null) return;
                GC.Collector.AddObject(CoreInstance);
                CoreInstance = null;
            }
            Particular.GC.SuppressFinalize(this);
        }

        public float GetFloat(string name)
        {
            return CoreInstance.GetFloat(name);
        }

        public void SetFloat(string name, float value)
        {
            CoreInstance.SetFloat(name, value);
        }

        public Vector2DF GetVector2DF(string name)
        {
            return CoreInstance.GetVector2DF(name);
        }

        public void SetVector2DF(string name, Vector2DF value)
        {
            CoreInstance.SetVector2DF(name, value);
        }

        public Vector3DF GetVector3DF(string name)
        {
            return CoreInstance.GetVector3DF(name);
        }

        public void SetVector3DF(string name, Vector3DF value)
        {
            CoreInstance.SetVector3DF(name, value);
        }

        public Vector4DF GetVector4DF(string name)
        {
            return CoreInstance.GetVector4DF(name);
        }

        public void SetVector4DF(string name, Vector4DF value)
        {
            CoreInstance.SetVector4DF(name, value);
        }

        public Texture2D GetTexture2D(string name)
        {
            var swigObj = swig.Accessor.MaterialPropertyBlock_GetTexture2D(CoreInstance, name);
            return GC.GenerateTexture2D(swigObj, GenerationType.Get);
        }

        public void SetTexture2D(string name, Texture2D value)
        {
            if (value == null) return;
            CoreInstance.SetTexture2D(name, IG.GetTexture2D(value));
        }

        public TextureFilterType GetTextureFilterType(string name)
        {
            return (TextureFilterType)CoreInstance.GetTextureFilterType(name);
        }

        public void SetTextureFilterType(string name, TextureFilterType filter)
        {
            CoreInstance.SetTextureFilterType(name, (swig.TextureFilterType)filter);
        }

        public TextureWrapType GetTextureWrapType(string name)
        {
            return (TextureWrapType)CoreInstance.GetTextureWrapType(name);
        }

        public void SetTextureWrapType(string name, TextureWrapType wrap)
        {
            CoreInstance.SetTextureWrapType(name, (swig.TextureWrapType)wrap);
        }
    }
}

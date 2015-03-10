using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
    /// 複数の画像が格納されているクラス
    /// </summary>
    public class ImagePackage : IDestroy
    {
        internal swig.ImagePackage  SwigObject { get; set; }

        internal ImagePackage (swig.ImagePackage  swig)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.ImagePackages.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
            SwigObject = swig;
        }

        ~ImagePackage ()
        {
            Destroy();
        }

        public bool IsDestroyed
        {
            get
            {
                return SwigObject == null;
            }
        }

        public void Destroy()
        {
            lock (this)
            {
                if (SwigObject == null) return;
                GC.Collector.AddObject(SwigObject);
                SwigObject = null;
            }
            System.GC.SuppressFinalize(this);
        }

		/// <summary>
		/// 格納されている画像の枚数を取得する。
		/// </summary>
		public int ImageCount
		{
			get {return SwigObject.GetImageCount();}
		}
 
		/// <summary>
		/// 格納されている画像の名称を取得する。
		/// </summary>
		/// <param name="index">インデックス</param>
		/// <returns>名称</returns>
       public string GetImageName(int index)
	   {
		   return SwigObject.GetImageName(index);
	   }

		/// <summary>
		/// 格納されている画像が配置される領域を取得する。
		/// </summary>
		/// <param name="index">インデックス</param>
		/// <returns>領域</returns>
		public RectI GetImageArea(int index)
		{
			return SwigObject.GetImageArea(index);
		}

		/// <summary>
		/// 画像を取得する。
		/// </summary>
		/// <param name="index">インデックス</param>
		/// <returns>画像</returns>
		public Texture2D GetImage(int index)
		{
			return GC.GenerateTexture2D(swig.Accessor.ImagePackage_GetImage(SwigObject,index), GC.GenerationType.Get);
		}
    }
}

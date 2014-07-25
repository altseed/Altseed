using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Texture2D : Texture, IDestroy
	{
		internal swig.Texture2D SwigObject { get; set; }

		internal Texture2D(swig.Texture2D swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if(GC.Texture2Ds.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~Texture2D()
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
		/// テクスチャをファイルに保存する。
		/// </summary>
		/// <param name="path">出力先</param>
		/// <returns>成否</returns>
		public bool Save(string path)
		{
			return SwigObject.Save(path);
		}

		/// <summary>
		/// テクスチャをロックし編集可能にする。
		/// </summary>
		/// <param name="info">テクスチャ情報</param>
		/// <returns>成否</returns>
		public bool Lock(ref TextureLockInfomation info)
		{
			return SwigObject.Lock(ref info);
		}

		/// <summary>
		/// テクスチャをアンロックする。
		/// </summary>
		public void Unlock()
		{
			SwigObject.Unlock();
		}

		public Vector2DI Size
		{
			get { return SwigObject.GetSize(); }
		}

		/// <summary>
		/// テクスチャのフォーマットを取得する。
		/// </summary>
		public TextureFormat Format
		{
			get { return (TextureFormat)SwigObject.GetFormat(); }
		}
	}
}

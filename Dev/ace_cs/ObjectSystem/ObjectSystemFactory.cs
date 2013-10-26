using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	class ObjectSystemFactory
	{
		/// <summary>
		/// ObjectSystemFactory クラスをインスタンス化します。
		/// </summary>
		/// <param name="factory">ネイティブのラッパー。</param>
		public ObjectSystemFactory(swig.ObjectSystemFactory factory)
		{
			this.factory = factory;
		}
		
		/// <summary>
		/// ネイティブの CoreObject2D クラスをインスタンス化します。
		/// </summary>
		/// <returns>ネイティブのインスタンス。</returns>
		public swig.CoreObject2D CreateObject2D()
		{
			return factory.CreateObject2D();
		}

		/// <summary>
		/// ネイティブの CoreTextureObject2D クラスをインスタンス化します。
		/// </summary>
		/// <returns>ネイティブのインスタンス。</returns>
		public swig.CoreTextureObject2D CreateTextureObject2D()
		{
			return factory.CreateTextureObject2D();
		}

		/// <summary>
		/// ネイティブの CoreLayer2D クラスをインスタンス化します。
		/// </summary>
		/// <returns>ネイティブのインスタンス。</returns>
		public swig.CoreLayer2D CreateLayer2D()
		{
			return factory.CreateLayer2D();
		}

		/// <summary>
		/// ネイティブの CoreScene クラスをインスタンス化します。
		/// </summary>
		/// <returns>ネイティブのインスタンス。</returns>
		public swig.CoreScene CreateScene()
		{
			return factory.CreateScene();
		}

		/// <summary>
		/// ネイティブの PostEffect クラスをインスタンス化する。
		/// </summary>
		/// <returns></returns>
		public swig.CorePostEffect CreatePostEffect()
		{
			return factory.CreatePostEffect();
		}

		private swig.ObjectSystemFactory factory;
	}
}

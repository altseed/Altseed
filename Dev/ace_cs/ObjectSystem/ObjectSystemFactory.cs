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
		/// ObjectSystemFactory クラスをインスタンス化する。
		/// </summary>
		/// <param name="factory">ネイティブのインスタンス</param>
		public ObjectSystemFactory(swig.ObjectSystemFactory factory)
		{
			this.factory = factory;
		}

		/// <summary>
		/// ネイティブのクラスをインスタンス化する。
		/// </summary>
		/// <returns>ネイティブのインスタンス</returns>
		public swig.CoreCameraObject2D CreateCameraObject2D()
		{
			return factory.CreateCameraObject2D();
		}

		/// <summary>
		/// ネイティブのクラスをインスタンス化する。
		/// </summary>
		/// <returns>ネイティブのインスタンス</returns>
		public swig.CoreTextureObject2D CreateTextureObject2D()
		{
			return factory.CreateTextureObject2D();
		}

		/// <summary>
		/// ネイティブのクラスをインスタンス化する。
		/// </summary>
		/// <returns>ネイティブのインスタンス</returns>
		public swig.CoreTextObject2D CreateTextObject2D()
		{
			return factory.CreateTextObject2D();
		}

		/// <summary>
		/// ネイティブのクラスをインスタンス化する。
		/// </summary>
		/// <returns>ネイティブのインスタンス</returns>
		public swig.CoreEffectObject2D CreateEffectObject2D()
		{
			return factory.CreateEffectObject2D();
		}

		/// <summary>
		/// ネイティブの CoreLayer2D クラスをインスタンス化する。
		/// </summary>
		/// <returns>ネイティブのインスタンス。</returns>
		public swig.CoreLayer2D CreateLayer2D()
		{
			return factory.CreateLayer2D();
		}

		/// <summary>
		/// ネイティブの MeshObject3D クラスをインスタンス化する。
		/// </summary>
		/// <returns>ネイティブのインスタンス</returns>
		public swig.CoreModelObject3D CreateModelObject3D()
		{
			return factory.CreateModelObject3D();
		}

		/// <summary>
		/// ネイティブの CameraObject3D クラスをインスタンス化する。
		/// </summary>
		/// <returns>ネイティブのインスタンス</returns>
		public swig.CoreCameraObject3D CreateCameraObject3D()
		{
			return factory.CreateCameraObject3D();
		}

		/// <summary>
		/// ネイティブの EffectObject3D クラスをインスタンス化する。
		/// </summary>
		/// <returns>ネイティブのインスタンス</returns>
		public swig.CoreEffectObject3D CreateEffectObject3D()
		{
			return factory.CreateEffectObject3D();
		}

		/// <summary>
		/// ネイティブの DirectionalLightObject3D クラスをインスタンス化する。
		/// </summary>
		/// <returns>ネイティブのインスタンス</returns>
		public swig.CoreDirectionalLightObject3D CreateDirectionalLightObject3D()
		{
			return factory.CreateDirectionalLightObject3D();
		}

		/// <summary>
		/// ネイティブの CoreLayer2D クラスをインスタンス化する。
		/// </summary>
		/// <param name="settings">設定</param>
		/// <returns>ネイティブのインスタンス</returns>
		public swig.CoreLayer3D CreateLayer3D(swig.RenderSettings settings)
		{
			return factory.CreateLayer3D(settings);
		}

		/// <summary>
		/// ネイティブの CoreScene クラスをインスタンス化する。
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

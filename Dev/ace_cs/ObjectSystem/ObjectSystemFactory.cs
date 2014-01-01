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
		/// ネイティブの CoreTextureObject2D クラスをインスタンス化する。
		/// </summary>
		/// <returns>ネイティブのインスタンス。</returns>
		public swig.CoreTextureObject2D CreateTextureObject2D()
		{
			return factory.CreateTextureObject2D();
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
		public swig.CoreMeshObject3D CreateMeshObject3D()
		{
			return factory.CreateMeshObject3D();
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
		/// <returns>ネイティブのインスタンス</returns>
		public swig.CoreLayer3D CreateLayer3D()
		{
			return factory.CreateLayer3D();
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

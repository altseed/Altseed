using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
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
        /// ネイティブのMapObject2Dをインスタンス化する。
        /// </summary>
        /// <returns>ネイティブのインスタンス</returns>
        public swig.CoreMapObject2D CreateMapObject2D()
        {
            return factory.CreateMapObject2D();
        }


        /// <summary>
        /// ネイティブのChip2Dをインスタンス化する。
        /// </summary>
        /// <returns>ネイティブのインスタンス</returns>
        public swig.CoreChip2D CreateChip2D()
        {
            return factory.CreateChip2D();
        }

        /// <summary>
        /// ネイティブのTextObject2Dをインスタンス化する。
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
        /// ネイティブのクラスをインスタンス化する。
        /// </summary>
        /// <returns>ネイティブのインスタンス</returns>
        public swig.CoreGeometryObject2D CreateGeometryObject2D()
        {
            return factory.CreateGeometryObject2D();
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

		public swig.CoreMassModelObject3D CreateMassModelObject3D()
		{
			return factory.CreateCoreMassModelObject3D();
		}

		public swig.CoreTerrainObject3D CreateTerrainObject3D()
		{
			return factory.CreateCoreTerrainObject3D();
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

		/// <summary>
		/// ネイティブの PostEffect クラスをインスタンス化する。
		/// </summary>
		/// <returns></returns>
		public swig.CoreTransition CreateTransition()
		{
			return factory.CreateTransition();
		}

        public swig.CoreArcShape CreateArcShape()
        {
            return factory.CreateArcShape();
        }

        public swig.CorePolygonShape CreatePolygonShape()
        {
            return factory.CreatePolygonShape();
        }

        public swig.CoreCircleShape CreateCircleShape()
        {
            return factory.CreateCircleShape();
        }

        public swig.CoreLineShape CreateLineShape()
        {
            return factory.CreateLineShape();
        }

        public swig.CoreRectangleShape CreateRectangleShape()
        {
            return factory.CreateRectangleShape();
        }

        public swig.CoreTriangleShape CreateTriangleShape()
        {
            return factory.CreateTriangleShape();
        }

        public swig.CoreCircleCollider CreateCircleCollider()
        {
            return factory.CreateCircleCollider();
        }

        public swig.CoreLineCollider CreateLineCollider()
        {
            return factory.CreateLineCollider();
        }

        public swig.CoreRectangleCollider CreateRectangleCollider()
        {
            return factory.CreateRectangleCollider();
        }

        public swig.CorePolygonCollider CreatePolygonCollider()
        {
            return factory.CreatePolygonCollider();
        }

		private swig.ObjectSystemFactory factory;
	}
}

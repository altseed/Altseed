using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// カメラの機能を提供するクラス
    /// </summary>
    public class CameraObject3D : Object3D
    {
        swig.CoreCameraObject3D coreObject = null;

        public CameraObject3D()
        {
            coreObject = Engine.ObjectSystemFactory.CreateCameraObject3D();
            GC.Object3Ds.AddObject(coreObject.GetPtr(), this);
            commonObject = coreObject;
        }

        public override void ForceToRelease()
        {
            coreObject = null;
            base.ForceToRelease();
        }

        protected override void OnAdded() { }
        protected override void OnUpdate() { }

        internal override void OnUpdateInternal()
        {
            coreObject.StartAddingPostEffect(postEffects.Count());

            int count = 0;
            foreach (var p in postEffects)
            {
                coreObject.BeginPostEffect(p.CoreInstance);

                var src_ = coreObject.GetSrcForPostEffect(count);
                var dst_ = coreObject.GetDstForPostEffect(count);

                RenderTexture2D src = GC.GenerateRenderTexture2D(src_, GenerationType.Get);
                RenderTexture2D dst = GC.GenerateRenderTexture2D(dst_, GenerationType.Get);

                p.Draw(dst, src);

                coreObject.EndPostEffect(p.CoreInstance);

                count++;
            }
        }

        /// <summary>
        /// 描画先がHDRかどうか、取得、または設定する。
        /// </summary>
        public bool HDRMode
        {
            get { return coreObject.GetHDRMode(); }
            set { coreObject.SetHDRMode(value); }
        }

        /// <summary>
        /// 画面サイズを取得または設定する。
        /// </summary>
        public Vector2DI WindowSize
        {
            get { return coreObject.GetWindowSize(); }
            set { coreObject.SetWindowSize(ref value); }
        }

        /// <summary>
        /// カメラが見る座標を取得または設定する。
        /// </summary>
        public Vector3DF Focus
        {
            get { return coreObject.GetFocus(); }
            set { coreObject.SetFocus(ref value); }
        }

        /// <summary>
        /// 画角を取得または設定する。
        /// </summary>
        public float FieldOfView
        {
            get { return coreObject.GetFieldOfView(); }
            set { coreObject.SetFieldOfView(value); }
        }

        /// <summary>
        /// 描画する最遠距離を取得または設定する。
        /// </summary>
        public float ZFar
        {
            get { return coreObject.GetZFar(); }
            set { coreObject.SetZFar(value); }
        }

        /// <summary>
        /// 描画する最近距離を取得または設定する。
        /// </summary>
        public float ZNear
        {
            get { return coreObject.GetZNear(); }
            set { coreObject.SetZNear(value); }
        }

		/// <summary>
		/// プロジェクション行列を直接取得または設定する。
		/// </summary>
		/// <remarks>
		/// この値を直接設定した場合、プロジェクション行列に関わるパラメーターは無効化される。
		/// </remarks>
		public Matrix44 ProjectionMatrixDirectly
		{
			get { return coreObject.GetProjectionMatrixDirectly(); }
			set { coreObject.SetProjectionMatrixDirectly(value); }
		}

        /// <summary>
        /// ポストエフェクトを追加する。
        /// </summary>
        /// <param name="postEffect">ポストエフェクト</param>
        public void AddPostEffect(PostEffect postEffect)
        {
            postEffects.Add(postEffect);
        }

        /// <summary>
        /// ポストエフェクトを全て消去する。
        /// </summary>
        public void ClearPostEffects()
        {
            postEffects.Clear();
        }

        List<PostEffect> postEffects = new List<PostEffect>();
    }
}

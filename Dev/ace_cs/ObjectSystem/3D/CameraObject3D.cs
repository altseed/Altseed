using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
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

		public override void Destroy()
		{
			coreObject = null;
			base.Destroy();
		}

		protected override void OnStart() { }
		protected override void OnUpdate() { }
		internal protected override void OnDrawAdditionally() { }

		protected override void OnUpdateInternal()
		{
			coreObject.SetPostEffectCount(postEffects.Count);

			int count = 0;
			foreach (var p in postEffects)
			{
				coreObject.BeginPostEffect(p.SwigObject);

				var src_ = coreObject.GetSrcForPostEffect(count);
				var dst_ = coreObject.GetDstForPostEffect(count);

				RenderTexture2D src = GC.GenerateRenderTexture2D(src_, GC.GenerationType.Get);
				RenderTexture2D dst = GC.GenerateRenderTexture2D(dst_, GC.GenerationType.Get);

				p.OnDraw(dst, src);

				coreObject.EndPostEffect(p.SwigObject);

				count++;
			}
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

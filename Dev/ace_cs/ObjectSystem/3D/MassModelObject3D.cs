using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 大量にメッシュを表示する3Dオブジェクトの機能を提供するクラス
	/// </summary>
	public class MassModelObject3D : Object3D
	{
		swig.CoreMassModelObject3D coreObject = null;

		public MassModelObject3D()
		{
			coreObject = Engine.ObjectSystemFactory.CreateMassModelObject3D();
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

		/// <summary>
		/// 描画に使用するモデルを設定する。
		/// </summary>
		/// <param name="model">モデル</param>
		public void SetModel(MassModel model)
		{
			coreObject.SetModel(model.SwigObject);
		}

		/// <summary>
		/// アニメーションを再生する。
		/// </summary>
		/// <param name="name">アニメーションの名称</param>
		public void PlayAnimation(string name)
		{
			coreObject.PlayAnimation(name);
		}

		/// <summary>
		/// アニメーションを停止する。
		/// </summary>
		public void StopAnimation()
		{
			coreObject.StopAnimation();
		}

		/// <summary>
		/// アニメーションをクロスフェードで再生する。
		/// </summary>
		/// <param name="name">アニメーションの名称</param>
		/// <param name="time">クロスフェードにかかる時間(秒)</param>
		public void CrossFadeAnimation(string name, float time)
		{
			coreObject.CrossFadeAnimation(name, time);
		}

		/**
		@brief	アニメーションが再生中か取得する。
		@return	再生中か?
		*/
		public bool IsAnimationPlaying()
		{
			return coreObject.IsAnimationPlaying();
		}
	}
}

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
			coreObject.SetModel(model.CoreInstance);
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

		/// <summary>
		/// アニメーションが再生中か取得する。
		/// </summary>
		/// <returns>再生中か?</returns>
		public bool IsAnimationPlaying
		{
			get
			{
				return coreObject.GetIsAnimationPlaying();
			}
		}

		/// <summary>
		/// 材質個別に描画時のパラメーターを設定する。
		/// </summary>
		/// <param name="block">プロパティ</param>
		/// <remarks>
		/// 通常描画する場合、材質に設定されたパラメーターを元に描画するがモデルごとに個別のパラメーターを使用する場合、この関数を使用する。
		/// </remarks>
		public void SetMaterialPropertyBlock(MaterialPropertyBlock block)
		{
			coreObject.SetMaterialPropertyBlock(IG.GetMaterialPropertyBlock(block));
		}
	}
}

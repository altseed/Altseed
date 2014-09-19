using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// メッシュを表示する3Dオブジェクトの機能を提供するクラス
	/// </summary>
	public class ModelObject3D : Object3D
	{
		swig.CoreModelObject3D coreObject = null;

		public ModelObject3D()
		{
			coreObject = Engine.ObjectSystemFactory.CreateModelObject3D();
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

		/// <summary>
		/// 描画に使用するモデルを設定する。
		/// </summary>
		/// <param name="model">モデル</param>
		public void SetModel(Model model)
		{
			coreObject.SetModel(model.SwigObject);
		}

		/// <summary>
		/// 描画に使用するメッシュを追加する。
		/// </summary>
		/// <param name="mesh">メッシュ</param>
		public void AddMesh(Mesh mesh)
		{
			coreObject.AddMesh(mesh.SwigObject);
		}

		/// <summary>
		/// 描画に使用するデフォーマーを設定する。
		/// </summary>
		/// <param name="deformer">デフォーマー</param>
		public void SetDeformer(Deformer deformer)
		{
			coreObject.SetDeformer(deformer.SwigObject);
		}

		/// <summary>
		/// アニメーションクリップを取得する。
		/// </summary>
		/// <param name="name">アニメーションクリップの名称</param>
		/// <returns>アニメーションクリップ</returns>
		public AnimationClip GetAnimationClip(string name)
		{
			return GC.GenerateAnimationClip(coreObject.GetAnimationClip(name), GC.GenerationType.Get);
		}

		/// <summary>
		/// アニメーションを再生する。
		/// </summary>
		/// <param name="index">対象のインデックス</param>
		/// <param name="name">アニメーションの名称</param>
		public void PlayAnimation(int index, string name)
		{
			coreObject.PlayAnimation(index, name);
		}

		/// <summary>
		/// アニメーションを停止する。
		/// </summary>
		/// <param name="index">対象のインデックス</param>
		public void StopAnimation(int index)
		{
			coreObject.StopAnimation(index);
		}

		/// <summary>
		/// アニメーションの合成比率を設定する。
		/// </summary>
		/// <param name="index">対象のインデックス</param>
		/// <param name="weight">合成比率</param>
		public void SetAnimationWeight(int index, float weight)
		{
			coreObject.SetAnimationWeight(index, weight);
		}

		/// <summary>
		/// アニメーションをクロスフェードで再生する。
		/// </summary>
		/// <param name="index">対象のインデックス</param>
		/// <param name="name">アニメーションの名称</param>
		/// <param name="time">クロスフェードにかかる時間(秒)</param>
		public void CrossFade(int index, string name, float time)
		{
			coreObject.CrossFade(index, name, time);
		}

		/**
		@brief	アニメーションが再生中か取得する。
		@param	index	対象のインデックス
		@return	再生中か?
		*/
		public bool IsAnimationPlaying(int index)
		{
			return coreObject.IsAnimationPlaying(index);
		}

		/// <summary>
		/// 材質個別に描画時のパラメーターを設定する。
		/// </summary>
		/// <param name="meshIndex">メッシュのインデックス</param>
		/// <param name="materialIndex">材質のインデックス</param>
		/// <param name="block">プロパティ</param>
		/// <remarks>
		/// 通常描画する場合、材質に設定されたパラメーターを元に描画するがモデルごとに個別のパラメーターを使用する場合、この関数を使用する。
		/// </remarks>
		public void SetMaterialPropertyBlock(int meshIndex, int materialIndex, MaterialPropertyBlock block)
		{
			coreObject.SetMaterialPropertyBlock(meshIndex, materialIndex, IG.GetMaterialPropertyBlock(block));
		}
	}
}

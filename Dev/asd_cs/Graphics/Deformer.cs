using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// メッシュを変形させるためのクラス
	/// </summary>
	public class Deformer : IDestroy
	{
		internal swig.Deformer SwigObject { get; set; }

		internal Deformer(swig.Deformer swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Deformers.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif
			SwigObject = swig;
		}

		~Deformer()
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
			Particular.GC.SuppressFinalize(this);
		}

		/**
			@brief	
			@param	name	
			@param	rotationOrder	
			@param	localMat	
			@param	globalMatInv	
		*/

		/// <summary>
		/// ボーンを追加する。
		/// </summary>
		/// <param name="name">ボーンの名称</param>
		/// <param name="parentBoneIndex">親ボーンのインデックス(親がない場合は-1)</param>
		/// <param name="rotationOrder">ボーンの回転行列の計算方法</param>
		/// <param name="localMat">ボーンのローカル変形行列</param>
		public void AddBone(string name, int parentBoneIndex, RotationOrder rotationOrder, asd.Matrix44 localMat,
			asd.Vector3DF translation, asd.Vector4DF rotation, asd.Vector3DF scaling)
		{
			SwigObject.AddBone(name, parentBoneIndex, (swig.RotationOrder)rotationOrder, ref localMat, ref translation, ref rotation, ref scaling);
		}
	}
}

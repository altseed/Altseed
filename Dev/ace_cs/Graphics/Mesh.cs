using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 3Dメッシュのクラス
	/// </summary>
	public class Mesh : IDestroy
	{
		internal swig.Mesh SwigObject { get; set; }

		internal Mesh(swig.Mesh swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Meshs.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~Mesh()
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
			System.GC.SuppressFinalize(this);
		}

		/// <summary>
		/// 頂点を追加する。
		/// </summary>
		/// <param name="position">座標</param>
		/// <param name="normal">法線</param>
		/// <param name="binormal">従法線</param>
		/// <param name="uv1">UV1</param>
		/// <param name="uv2">UV2</param>
		/// <param name="color">頂点色</param>
		/// <param name="boneWeights">ボーンのウエイト</param>
		/// <param name="boneIndexes">ボーンのインデックス</param>
		public void AddVertex(ace.Vector3DF position, ace.Vector3DF normal, ace.Vector3DF binormal, ace.Vector2DF uv1, ace.Vector2DF uv2, ace.Color color, int boneWeights, int boneIndexes)
		{
			SwigObject.AddVertex(ref position, ref normal, ref binormal, ref uv1, ref uv2, ref color, boneWeights, boneIndexes);
		}

		/// <summary>
		/// 面を追加する。
		/// </summary>
		/// <param name="index1">頂点インデックス1</param>
		/// <param name="index2">頂点インデックス2</param>
		/// <param name="index3">頂点インデックス3</param>
		public void AddFace(int index1, int index2, int index3)
		{
			SwigObject.AddFace(index1, index2, index3);
		}

		/// <summary>
		/// 素材を設定する。
		/// </summary>
		/// <param name="materialIndex">素材のインデックス</param>
		/// <param name="faceCount">面数</param>
		/// <remarks>素材は追加された順に先頭から面数の数だけ割り当てられる。</remarks>
		public void AddMaterialCount(int materialIndex, int faceCount)
		{
			SwigObject.AddMaterialCount(materialIndex, faceCount);
		}

		/// <summary>
		/// 設定した値をGPUに送信する。
		/// </summary>
		public void SendToGPUMemory()
		{
			SwigObject.SendToGPUMemory();
		}

		/// <summary>
		/// デフォーマーを設定する。
		/// </summary>
		/// <param name="deformer">デフォーマー</param>
		public void SetDeformer(Deformer deformer)
		{
			SwigObject.SetDeformer(deformer.SwigObject);
		}
	}
}

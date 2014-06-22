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
		/// <param name="materialIndex">材質インデックス</param>
		public void AddFace(int index1, int index2, int index3, int materialIndex)
		{
			SwigObject.AddFace(index1, index2, index3, materialIndex);
		}

		/// <summary>
		/// 素材を追加する。
		/// </summary>
		/// <returns>材質のインデックス</returns>
		public int AddMaterial()
		{
			return SwigObject.AddMaterial();
		}

		/// <summary>
		/// ボーンとの接続設定を追加する。
		/// </summary>
		/// <param name="targetIndex">対象ボーンインデックス</param>
		/// <param name="boneToMesh">ボーンの行列をメッシュの行列に変換する行列</param>
		public void AddBoneConnector(int targetIndex, Matrix44 boneToMesh)
		{
			SwigObject.AddBoneConnector(targetIndex, ref boneToMesh);
		}

		/// <summary>
		/// 設定した値をGPUに送信する。
		/// </summary>
		public void SendToGPUMemory()
		{
			SwigObject.SendToGPUMemory();
		}

		/// <summary>
		/// 内部シェーダーを使用する場合のカラーテクスチャを設定する。
		/// </summary>
		/// <param name="materialIndex">材質のインデックス</param>
		/// <param name="texture">テクスチャ</param>
		/// <remarks>AddMaterialCountを実行した後でないと無効になる。</remarks>
		public void SetColorTexture(int materialIndex, Texture2D texture)
		{
			SwigObject.SetColorTexture(materialIndex, texture.SwigObject);
		}

		/// <summary>
		/// 内部シェーダーを使用する場合の法線テクスチャを設定する。
		/// </summary>
		/// <param name="materialIndex">材質のインデックス</param>
		/// <param name="texture">テクスチャ</param>
		/// <remarks>AddMaterialCountを実行した後でないと無効になる。</remarks>
		public void SetNormalTexture(int materialIndex, Texture2D texture)
		{
			SwigObject.SetNormalTexture(materialIndex, texture.SwigObject);
		}

		/// <summary>
		/// 内部シェーダーを使用する場合のスペキュラテクスチャを設定する。
		/// </summary>
		/// <param name="materialIndex">材質のインデックス</param>
		/// <param name="texture">テクスチャ</param>
		/// <remarks>AddMaterialCountを実行した後でないと無効になる。</remarks>
		public void SetSpecularTexture(int materialIndex, Texture2D texture)
		{
			SwigObject.SetSpecularTexture(materialIndex, texture.SwigObject);
		}
	}
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 更新・描画処理を行う単位となる3Dオブジェクトの機能を提供するクラス
	/// </summary>
	public abstract class Object3D : IDestroy
	{
		internal swig.CoreObject3D commonObject = null;

		Layer3D owner = null;

		public Object3D()
		{
			IsUpdated = true;
			IsAlive = true;
		}

		#region GC対応
		~Object3D()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get { return commonObject == null; }
		}

		public virtual void Destroy()
		{
			lock (this)
			{
				if (commonObject == null) return;
				GC.Collector.AddObject(commonObject);
				commonObject = null;
			}
			System.GC.SuppressFinalize(this);
		}
		#endregion

		/// <summary>
		/// オブジェクトの更新を実行するかどうか取得または設定する。
		/// </summary>
		public bool IsUpdated { get; set; }

		/// <summary>
		/// オブジェクトを描画するかどうか取得または設定する。
		/// </summary>
		public bool IsDrawn
		{
			get { return CoreObject.GetIsDrawn(); }
			set { CoreObject.SetIsDrawn(value); }
		}

		/// <summary>
		/// オブジェクトが生存しているか取得する。
		/// </summary>
		public bool IsAlive { get; private set; }

		/// <summary>
		/// オブジェクトを破棄する。
		/// </summary>
		void Vanish()
		{
			IsAlive = false;
		}

		internal void Start()
		{
			OnStart();
		}

		internal void Update()
		{
			if (!IsUpdated || !IsAlive)
			{
				return;
			}

			OnUpdate();

			OnUpdateInternal();

			/*
			foreach (var item in components_)
			{
				item.Value.Update();
			}
			*/
		}

		internal void DrawAdditionally()
		{
			if (!IsDrawn || !IsAlive)
			{
				return;
			}
			OnDrawAdditionally();
		}

		internal swig.CoreObject3D CoreObject { get { return commonObject; } }

		/// <summary>
		/// ユーザーはオーバーライドしてはいけない。
		/// </summary>
		protected virtual void OnUpdateInternal() { }

		/// <summary>
		/// オーバーライドして、この3Dオブジェクトの初期化処理を記述することができる。
		/// </summary>
		protected abstract void OnStart();

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトの更新処理を記述することができる。
		/// </summary>
		protected abstract void OnUpdate();

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトに関する追加の描画処理を記述できる。
		/// </summary>
		protected void OnDrawAdditionally() { }

		/// <summary>
		/// このインスタンスを管理している ace.Layer3D クラスのインスタンスを取得する。
		/// </summary>
		public Layer3D Layer
		{
			get { return owner; }
			internal set { owner = value; }
		}

		/// <summary>
		/// このインスタンスの親に対する現在の座標を取得または設定する。
		/// </summary>
		public Vector3DF Position
		{
			get { return commonObject.GetPosition(); }
			set { commonObject.SetPosition(value); }
		}

		/// <summary>
		/// このインスタンスの親に対する現在の角度を取得または設定する。
		/// </summary>
		public Vector3DF Rotation
		{
			get { return commonObject.GetRotation(); }
			set { commonObject.SetRotation(ref value); }
		}

		/// <summary>
		/// このインスタンスの親に対する現在の拡大率を取得または設定する。
		/// </summary>
		public Vector3DF Scale
		{
			get { return commonObject.GetScale(); }
			set { commonObject.SetScale(ref value); }
		}

		/// <summary>
		/// 通常の描画に加えてテクスチャを描画する。
		/// </summary>
		/// <param name="upperLeftPos">テクスチャの左上の描画位置</param>
		/// <param name="upperRightPos">テクスチャの右上の描画位置</param>
		/// <param name="lowerRightPos">テクスチャの右下の描画位置</param>
		/// <param name="lowerLeftPos">テクスチャの左下の描画位置</param>
		/// <param name="upperLeftCol">テクスチャの左上の頂点色</param>
		/// <param name="upperRightCol">テクスチャの右上の頂点色</param>
		/// <param name="lowerRightCol">テクスチャの右下の頂点色</param>
		/// <param name="lowerLeftCol">テクスチャの左下の頂点色</param>
		/// <param name="upperLeftUV">テクスチャの左上のUV値</param>
		/// <param name="upperRightUV">テクスチャの右上のUV値</param>
		/// <param name="lowerRightUV">テクスチャの右下のUV値</param>
		/// <param name="lowerLeftUV">テクスチャの左下のUV値</param>
		/// <param name="texture">描画するテクスチャ</param>
		/// <param name="alphaBlend">アルファブレンドの種類</param>
		/// <remarks>OnDrawAdditionallyの中以外では実行してはいけない。</remarks>
		public void DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Texture2D texture, AlphaBlendMode alphaBlend)
		{
			Layer.DrawSpriteAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, texture, alphaBlend);
		}
	}
}

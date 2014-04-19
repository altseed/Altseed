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
		/// 将来的に削除予定
		/// </summary>
		internal protected abstract void OnDrawAdditionally();

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
	}
}

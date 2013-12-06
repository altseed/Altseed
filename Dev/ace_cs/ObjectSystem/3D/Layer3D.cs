using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Layer3D : Layer, IDestroy
	{
		/// <summary>
		/// ace.Layer2D クラスの新しいインスタンスを初期化します。
		/// </summary>
		public Layer3D()
		{
			coreLayer3D = Engine.ObjectSystemFactory.CreateLayer3D();

			var p = coreLayer3D.GetPtr();
			if (GC.Layer3Ds.GetObject(p) != null)
			{
				throw new Exception();
			}

			GC.Layer3Ds.AddObject(p, this);

			objects_ = new List<Object3D>();

			commonObject = coreLayer3D;
		}

		#region GC対策
		~Layer3D()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get { return coreLayer3D == null; }
		}

		public void Destroy()
		{
			lock (this)
			{
				if (coreLayer3D == null) return;
				GC.Collector.AddObject(coreLayer3D);
				coreLayer3D = null;
			}
			System.GC.SuppressFinalize(this);
		}
		#endregion

		swig.CoreLayer3D coreLayer3D { get; set; }

		/// <summary>
		/// このレイヤーが管理している3Dオブジェクトのコレクションを取得する。
		/// </summary>
		public IEnumerable<Object3D> Objects
		{
			get { return objects_; }
		}

		/// <summary>
		/// このレイヤーに指定した3Dオブジェクトを追加する。
		/// </summary>
		/// <param name="object3D">追加する3Dオブジェクト</param>
		public void AddObject(Object3D object3D)
		{
			if (object3D.Layer != null)
			{
				throw new InvalidOperationException("指定したオブジェクトは既に別のレイヤーに所属しています。");
			}
			objects_.Add(object3D);
			coreLayer3D.AddObject(object3D.CoreObject);
			object3D.Layer = this;
			object3D.Start();
		}

		/// <summary>
		/// このレイヤーから指定した3Dオブジェクトを削除する。
		/// </summary>
		/// <param name="object3D">削除される3Dオブジェクト。</param>
		public void RemoveObject(Object3D object3D)
		{
			objects_.Remove(object3D);
			coreLayer3D.RemoveObject(object3D.CoreObject);
			object3D.Layer = null;
		}

		internal override void Update()
		{
			if (!IsUpdated)
			{
				return;
			}

			OnUpdating();

			foreach (var item in objects_)
			{
				item.Update();
			}
			
			objects_.RemoveAll(_ => !_.IsAlive);

			OnUpdated();
		}

		internal override void DrawAdditionally()
		{
			if (!IsDrawn)
			{
				return;
			}

			foreach (var item in objects_)
			{
				item.OnDrawAdditionally();
			}

			OnDrawAdditionally();
		}

		internal override void BeginDrawing()
		{
			coreLayer3D.BeginDrawing();
		}

		internal override void EndDrawing()
		{
			coreLayer3D.EndDrawing();

			/*
			 ポストエフェクト
			 */

			coreLayer3D.EndDrawingAfterEffects();
		}

		private List<Object3D> objects_ { get; set; }
	}
}

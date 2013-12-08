using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 2Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス。
	/// </summary>
	public class Layer2D : Layer, IDestroy
	{
		/// <summary>
		/// ace.Layer2D クラスの新しいインスタンスを初期化します。
		/// </summary>
		public Layer2D()
		{
			coreLayer2D = Engine.ObjectSystemFactory.CreateLayer2D();

			var p = coreLayer2D.GetPtr();
			if( GC.Layer2Ds.GetObject(p) != null)
			{
				throw new Exception();
			}

			GC.Layer2Ds.AddObject(p, this);

			objects_ = new List<Object2D>();
			
			components_ = new Dictionary<string, Layer2DComponent>();

			commonObject = coreLayer2D;
		}

		#region GC対策
		~Layer2D()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get { return coreLayer2D == null; }
		}

		public void Destroy()
		{
			lock( this )
			{
				if( coreLayer2D == null ) return;
				GC.Collector.AddObject( coreLayer2D );
				coreLayer2D = null;
			}
			System.GC.SuppressFinalize( this );
		} 
		#endregion
		

		/// <summary>
		/// このレイヤーが管理している2Dオブジェクトのコレクションを取得します。
		/// </summary>
		public IEnumerable<Object2D> Objects
		{
			get { return objects_; }
		}

		public IDictionary<string, Layer2DComponent> Components
		{
			get { return components_; }
		}

		/// <summary>
		/// このレイヤーに指定した2Dオブジェクトを追加します。
		/// </summary>
		/// <param name="object2D">追加する2Dオブジェクト。</param>
		public void AddObject(Object2D object2D)
		{
			if( object2D.Layer != null )
			{
				throw new InvalidOperationException( "指定したオブジェクトは既に別のレイヤーに所属しています。" );
			}
			objects_.Add(object2D);
			coreLayer2D.AddObject(object2D.CoreObject);
			object2D.Layer = this;
			object2D.Start();
		}

		/// <summary>
		/// このレイヤーから指定した2Dオブジェクトを削除します。
		/// </summary>
		/// <param name="object2D">削除される2Dオブジェクト。</param>
		public void RemoveObject(Object2D object2D)
		{
			objects_.Remove(object2D);
			coreLayer2D.RemoveObject(object2D.CoreObject);
			object2D.Layer = null;
		}

		public void AddComponent(Layer2DComponent component, string key)
		{
			components_[key] = component;
			component.Owner = this;
		}

		public void RemoveComponent(string key)
		{
			components_[key].Owner = null;
			components_.Remove( key );
		}

		internal override void BeginUpdating()
		{
			coreLayer2D.BeginUpdating();
		}

		internal override void EndUpdating()
		{
			coreLayer2D.EndUpdating();
		}

		internal override void Update()
		{
			if( !IsUpdated )
			{
				return;
			}

			OnUpdating();

			foreach( var item in objects_ )
			{
				item.Update();
			}

			objects_.RemoveAll( _ => !_.IsAlive );

			OnUpdated();
		}

		internal override void DrawAdditionally()
		{
			if( !IsDrawn )
			{
				return;
			}

			foreach( var item in objects_ )
			{
				item.DrawAdditionally();
			}

			OnDrawAdditionally();
		}

		private swig.CoreLayer2D coreLayer2D { get; set; }

		private List<Object2D> objects_ { get; set; }

		private Dictionary<string, Layer2DComponent> components_ { get; set; }
	}
}

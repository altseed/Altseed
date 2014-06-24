using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 2Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス
	/// </summary>
	public class Layer2D : Layer, IDestroy
	{
		/// <summary>
		/// コンストラクタ
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
		/// このレイヤーが管理している2Dオブジェクトのコレクションを取得する。
		/// </summary>
		public IEnumerable<Object2D> Objects
		{
			get { return objects_; }
		}

		/// <summary>
		/// このレイヤーに登録されているコンポーネントのコレクションを取得する。
		/// </summary>
		public IDictionary<string, Layer2DComponent> Components
		{
			get { return components_; }
		}

		/// <summary>
		/// 指定した2Dオブジェクトをこのレイヤーに追加する。
		/// </summary>
		/// <param name="object2D">追加する2Dオブジェクト</param>
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
		/// 指定した2Dオブジェクトをこのレイヤーから削除する。
		/// </summary>
		/// <param name="object2D">削除される2Dオブジェクト</param>
		public void RemoveObject(Object2D object2D)
		{
			objects_.Remove(object2D);
			coreLayer2D.RemoveObject(object2D.CoreObject);
			object2D.Layer = null;
		}

		/// <summary>
		/// 指定したコンポーネントをこのレイヤーに追加する。
		/// </summary>
		/// <param name="component">追加するコンポーネント</param>
		/// <param name="key">コンポーネントに関連付けるキー</param>
		public void AddComponent(Layer2DComponent component, string key)
		{
			components_[key] = component;
			component.Owner = this;
		}

		/// <summary>
		/// 指定したコンポーネントをこのレイヤーから削除する。
		/// </summary>
		/// <param name="key">削除するコンポーネントを示すキー</param>
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
			UpdateObjects();
			UpdateComponents();

			OnUpdated();
		}

		private void UpdateObjects()
		{
			foreach (var item in objects_)
			{
				item.Update();
				if(!item.IsAlive)
				{
					beVanished.Add(item);
				}
			}
			
			foreach(var o in beVanished)
			{
				RemoveObject(o);
			}

			beVanished.Clear();
		}

		private void UpdateComponents()
		{
			var vanished = new List<string>();

			foreach( var item in components_ )
			{
				item.Value.Update();
				if( !item.Value.IsAlive )
				{
					vanished.Add( item.Key );
				}
			}

			foreach( var item in vanished )
			{
				components_.Remove( item );
			}
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

		List<Object2D> beVanished = new List<Object2D>();
	}
}

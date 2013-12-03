using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ace
{
	/// <summary>
	/// 更新・描画処理を行う単位となる2Dオブジェクトの機能を提供する抽象クラス。
	/// </summary>
	public abstract class Object2D
	{
		public Object2D()
		{
			components_ = new Dictionary<string, Object2DComponent>();
			children_ = new List<Object2D>();
			IsUpdated = true;
			IsAlive = true;
		}

		public bool IsUpdated { get; set; }

		public bool IsDrawn
		{
			get { return CoreObject.GetIsDrawn(); }
			set { CoreObject.SetIsDrawn( value ); }
		}

		public bool IsAlive { get; private set; }

		/// <summary>
		/// このインスタンスを管理している ace.Layer2D クラスのインスタンスを取得します。
		/// </summary>
		public abstract Layer2D Layer { get; internal set; }

		public IDictionary<string, Object2DComponent> Components
		{
			get { return components_; }
		}

		public IEnumerable<Object2D> Children
		{
			get { return children_; }
		}

		/// <summary>
		/// この2Dオブジェクトを描画する位置を取得または設定します。
		/// </summary>
		public Vector2DF Position
		{
			get { return CoreObject.GetPosition(); }
			set { CoreObject.SetPosition( value ); }
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の回転角度を取得または設定します。
		/// </summary>
		public float Angle
		{
			get { return CoreObject.GetAngle(); }
			set { CoreObject.SetAngle( value ); }
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の拡大率を取得または設定します。
		/// </summary>
		public Vector2DF Scale
		{
			get { return CoreObject.GetScale(); }
			set { CoreObject.SetScale( value ); }
		}


		public Vector2DF GetGlobalPosition()
		{
			return CoreObject.GetGlobalPosition();
		}

		public void Vanish()
		{
			IsAlive = false;
		}

		public void AddChild( Object2D child, ChildMode mode )
		{
			CoreObject.AddChild( child.CoreObject, (swig.eChildMode)mode );
			children_.Add( child );
		}

		public void RemoveChild( Object2D child )
		{
			CoreObject.RemoveChild( child.CoreObject );
			children_.Remove( child );
		}

		public void AddComponent( Object2DComponent component, string key )
		{
			component.Owner = this;
			components_[key] = component;
		}

		public void RemoveComponent( string key )
		{
			components_[key].Owner = null;
			components_.Remove(key);
		}

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトの初期化処理を記述することができます。
		/// </summary>
		protected abstract void OnStart();

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトの更新処理を記述することができます。
		/// </summary>
		protected abstract void OnUpdate();

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトに関する追加の描画処理を記述することができます。
		/// </summary>
		protected abstract void OnDrawAdditionally();


		internal abstract swig.CoreObject2D CoreObject { get; }

		internal void Start()
		{
			OnStart();
		}

		internal void Update()
		{
			if( !IsUpdated || !IsAlive )
			{
				return;
			}

			OnUpdate();
			foreach( var item in components_ )
			{
				item.Value.Update();
			}
		}

		internal void DrawAdditionally()
		{
			if( !IsDrawn || !IsAlive )
			{
				return;
			}
			OnDrawAdditionally();
		}

		private Dictionary<string, Object2DComponent> components_ { get; set; }

		private List<Object2D> children_ { get; set; }
	}
}

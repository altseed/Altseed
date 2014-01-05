using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// レイヤーの更新と描画を管理するシーン機能を提供するクラス。
	/// </summary>
	public class Scene : IDestroy
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Scene()
		{
			CoreScene = Engine.ObjectSystemFactory.CreateScene();

			var p = CoreScene.GetPtr();

			var existing = GC.Scenes.GetObject( p );
			if( existing != null )
			{
				throw new Exception();
			}

			GC.Scenes.AddObject( p, this );

			layersToDraw_ = new List<Layer>();
			layersToUpdate_ = new List<Layer>();
			components_ = new Dictionary<string, SceneComponent>();
		}

		#region GC対策
		~Scene()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get
			{
				return CoreScene == null;
			}
		}

		public void Destroy()
		{
			lock( this )
			{
				if( CoreScene == null ) return;
				GC.Collector.AddObject( CoreScene );
				CoreScene = null;
			}
			System.GC.SuppressFinalize( this );
		}
		#endregion


		/// <summary>
		/// このシーン クラスが管理するレイヤーのコレクションを取得する。
		/// </summary>
		public IEnumerable<Layer> Layers
		{
			get { return layersToUpdate_; }
		}

		/// <summary>
		/// このシーン クラスに登録されているコンポーネントのコレクションを取得する。
		/// </summary>
		public IDictionary<string, SceneComponent> Components
		{
			get { return components_; }
		}

		/// <summary>
		/// 指定したレイヤーをこのシーンに追加する。
		/// </summary>
		/// <param name="layer">追加されるレイヤー</param>
		public void AddLayer( Layer layer )
		{
			if( layer.Scene != null )
			{
				throw new InvalidOperationException( "指定したレイヤーは、既に別のシーンに所属しています。" );
			}
			layersToDraw_.Add( layer );
			layersToUpdate_.Add( layer );
			CoreScene.AddLayer( layer.CoreLayer );
			layer.Scene = this;
		}

		/// <summary>
		/// 指定したレイヤーをこのシーンから削除する。
		/// </summary>
		/// <param name="layer">削除されるレイヤー</param>
		public void RemoveLayer( Layer layer )
		{
			layersToDraw_.Remove( layer );
			layersToUpdate_.Remove( layer );
			CoreScene.RemoveLayer( layer.CoreLayer );
			layer.Scene = null;
		}

		/// <summary>
		/// 指定したコンポーネントをこのシーンに追加する。
		/// </summary>
		/// <param name="component">追加するコンポーネント</param>
		/// <param name="key">コンポーネントに関連付けるキー</param>
		public void AddComponent( SceneComponent component, string key )
		{
			component.Owner = this;
			components_[key] = component;
		}

		/// <summary>
		/// 指定したコンポーネントをこのシーンから削除する。
		/// </summary>
		/// <param name="key">削除するコンポーネントを示すキー</param>
		public void RemoveComponent( string key )
		{
			components_[key].Owner = null;
			components_.Remove( key );
		}

		/// <summary>
		/// オーバーライドして、Updateの直前に実行する処理を記述する。
		/// </summary>
		protected virtual void OnUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、Updateの直後に実行する処理を記述する。
		/// </summary>
		protected virtual void OnUpdated()
		{
		}


		internal unsafe swig.CoreScene CoreScene { get; private set; }

		internal void Update()
		{
			foreach( var item in layersToUpdate_ )
			{
				item.BeginUpdating();
			}

			OnUpdating();

			foreach( var item in layersToUpdate_ )
			{
				item.Update();
			}

			OnUpdated();

			foreach( var item in layersToUpdate_ )
			{
				item.EndUpdating();
			}

			UpdateComponents();
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

		internal void DrawAdditionally()
		{
			foreach( var item in layersToDraw_ )
			{
				item.DrawAdditionally();
			}
		}

		internal void BeginDrawing()
		{
			layersToDraw_.Sort( ( x, y ) => y.DrawingPriority - x.DrawingPriority );
			foreach( var item in layersToDraw_ )
			{
				item.BeginDrawing();
			}
		}

		internal void EndDrawing()
		{
			foreach( var item in layersToDraw_ )
			{
				item.EndDrawing();
			}
		}


		private List<Layer> layersToDraw_;

		private List<Layer> layersToUpdate_;

		private Dictionary<string, SceneComponent> components_;
	}
}

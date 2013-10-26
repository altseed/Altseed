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
		/// ace.Scene クラスの新しいインスタンスを初期化します。
		/// </summary>
		public Scene()
		{
			CoreScene = Engine.ObjectSystemFactory.CreateScene();

			var p = CoreScene.GetPtr();

			var existing = GC.Scenes.GetObject(p);
			if (existing != null)
			{
				throw new Exception();
			}

			GC.Scenes.AddObject(p, this);

			layers_ = new List<Layer>();
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
		/// このシーン クラスが管理するレイヤーのコレクションを取得します。
		/// </summary>
		public IEnumerable<Layer> Layers
		{
			get { return layers_; }
		}

		/// <summary>
		/// このシーンに指定したレイヤーを追加します。
		/// </summary>
		/// <param name="layer">追加されるレイヤー。</param>
		public void AddLayer( Layer layer )
		{
			if( layer.Scene != null )
			{
				throw new InvalidOperationException( "指定したレイヤーは、既に別のシーンに所属しています。" );
			}
			layers_.Add( layer );
			CoreScene.AddLayer( layer.CoreLayer );
			layer.Scene = this;
		}

		/// <summary>
		/// このシーンから指定したレイヤーを削除します。
		/// </summary>
		/// <param name="layer">削除されるレイヤー。</param>
		public void RemoveLayer( Layer layer )
		{
			layers_.Remove( layer );
			CoreScene.RemoveLayer( layer.CoreLayer );
			layer.Scene = null;
		}

		protected virtual void OnUpdating()
		{ 
		}

		protected virtual void OnUpdated()
		{
		}

		internal unsafe swig.CoreScene CoreScene { get; private set; }

		internal void Update()
		{
			OnUpdating();

			foreach( var item in layers_ )
			{
				item.Update();
			}

			OnUpdated();
		}

		internal void DrawAdditionally()
		{
			layers_.Sort( ( x, y ) => y.DrawingPriority - x.DrawingPriority );
			foreach( var item in layers_ )
			{
				item.DrawAdditionally();
			}
		}

		internal void BeginDrawing()
		{
			layers_.Sort((x, y) => y.DrawingPriority - x.DrawingPriority);
			foreach (var item in layers_)
			{
				item.BeginDrawing();
			}
		}

		internal void EndDrawing()
		{
			layers_.Sort((x, y) => y.DrawingPriority - x.DrawingPriority);
			foreach (var item in layers_)
			{
				item.EndDrawing();
			}
		}

		private List<Layer> layers_;
	}
}

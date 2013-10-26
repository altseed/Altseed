using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 更新・描画処理を行う単位となる2Dオブジェクトの機能を提供するクラス。
	/// </summary>
	public class Object2D : Object2DBase, IDestroy
	{
		/// <summary>
		/// ace.Object2D の新しいインスタンスを初期化します。
		/// </summary>
		public Object2D()
		{
			renderedObject = Engine.ObjectSystemFactory.CreateObject2D();

			var p = renderedObject.GetPtr();
			if( GC.Object2Ds.GetObject( p ) != null )
			{
				throw new Exception();
			}
			GC.Object2Ds.AddObject( p, this );
		}

		#region GC対応
		~Object2D()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get { return renderedObject == null; }
		}

		public void Destroy()
		{
			lock( this )
			{
				if( renderedObject == null ) return;
				GC.Collector.AddObject( renderedObject );
				renderedObject = null;
			}
			System.GC.SuppressFinalize( this );
		} 
		#endregion


		/// <summary>
		/// このインスタンスを管理している ace.Layer2D クラスのインスタンスを取得します。
		/// </summary>
		public override Layer2D Layer { get; internal set; }

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトの初期化処理を記述することができます。
		/// </summary>
		protected override void OnStart()
		{
		}

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトの更新処理を記述することができます。
		/// </summary>
		protected override void OnUpdate()
		{
		}

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトに関する追加の描画処理を記述することができます。
		/// </summary>
		protected override void OnDrawAdditionally()
		{
		}


		internal override swig.ICoreObject2D CoreObject
		{
			get { return renderedObject; }
		}

		private swig.CoreObject2D renderedObject { get; set; }
	}
}

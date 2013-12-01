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

		internal override void Update()
		{
			if (!IsUpdated)
			{
				return;
			}

			OnUpdating();

			//foreach (var item in objects_)
			//{
			//	item.Update();
			//}
			//
			//objects_.RemoveAll(_ => !_.IsAlive);

			OnUpdated();
		}

		internal override void DrawAdditionally()
		{
			if (!IsDrawn)
			{
				return;
			}

			//foreach (var item in objects_)
			//{
			//	item.DrawAdditionally();
			//}

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
	}
}

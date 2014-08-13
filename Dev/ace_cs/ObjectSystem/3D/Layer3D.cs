using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 3Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス
	/// </summary>
	public class Layer3D : Layer, IDestroy
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Layer3D(RenderSettings settings = null)
		{
			swig.RenderSettings settings_ = new swig.RenderSettings();
			if(settings != null)
			{
				settings_.IsLightweightMode = settings.IsLightweightMode;
				settings_.VisalizedBuffer = (swig.VisalizedBufferType)settings.VisualizedBuffer;
			}

			coreLayer3D = Engine.ObjectSystemFactory.CreateLayer3D(settings_);

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
		/// 描画設定を取得、設定する。
		/// </summary>
		public RenderSettings Settings
		{
			get
			{
				return new RenderSettings(coreLayer3D.GetRenderSettings());
			}
			set
			{
				swig.RenderSettings settings_ = new swig.RenderSettings();
				settings_.IsLightweightMode = value.IsLightweightMode;
				settings_.VisalizedBuffer = (swig.VisalizedBufferType)value.VisualizedBuffer;
				coreLayer3D.SetRenderSettings(settings_);
			}
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
		/// <param name="object3D">削除される3Dオブジェクト</param>
		public void RemoveObject(Object3D object3D)
		{
			objects_.Remove(object3D);
			coreLayer3D.RemoveObject(object3D.CoreObject);
			object3D.Layer = null;
		}

		/// <summary>
		/// 空方向の環境光の色を設定する。
		/// </summary>
		/// <param name="color">色</param>
		public void SetSkyAmbientColor(Color color)
		{
			coreLayer3D.SetSkyAmbientColor(color);
		}
	
		/// <summary>
		/// 地面方向の環境光の色を設定する。
		/// </summary>
		/// <param name="color">色</param>
		public void SetGroundAmbientColor(Color color)
		{
			coreLayer3D.SetGroundAmbientColor(color);
		}

		internal override void BeginUpdating()
		{
			coreLayer3D.BeginUpdating();
		}

		internal override void EndUpdating()
		{
			coreLayer3D.EndUpdating();
		}

		internal override void Update()
		{
			if (!IsUpdated)
			{
				return;
			}

			OnUpdating();

			var beVanished = new List<Object3D>();

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

		private List<Object3D> objects_ { get; set; }
	}
}

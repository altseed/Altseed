using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 2Dレイヤーの一部を描画するためのカメラ
	/// </summary>
	public class CameraObject2D : Object2D, IReleasable
	{
		/// <summary>
		/// 描画元の領域を取得、または設定する。
		/// </summary>
		public RectI Src
		{
			get { return coreCameraObject.GetSrc(); }
			set { coreCameraObject.SetSrc( value ); }
		}

		/// <summary>
		/// 描画先の領域を取得、または設定する。
		/// </summary>
		public RectI Dst
		{
			get { return coreCameraObject.GetDst(); }
			set { coreCameraObject.SetDst( value ); }
		}

		internal override swig.CoreObject2D CoreObject
		{
			get { return coreCameraObject; }
		}
		private swig.CoreCameraObject2D coreCameraObject { get; set; }

		public CameraObject2D()
		{
			coreCameraObject = Engine.ObjectSystemFactory.CreateCameraObject2D();

			var p = coreCameraObject.GetPtr();
			if( GC.Object2Ds.GetObject( p ) != null )
			{
				Particular.Helper.ThrowException("");
			}

			GC.Object2Ds.AddObject( p, this );
		}

#region GC対策
		~CameraObject2D()
		{
			ForceToRelease();
		}

		public override bool IsReleased
		{
			get { return coreCameraObject == null; }
		}

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
		public override void ForceToRelease()
		{
			lock( this )
			{
				if( coreCameraObject == null ) return;
				GC.Collector.AddObject( coreCameraObject );
				coreCameraObject = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
#endregion
	}
}

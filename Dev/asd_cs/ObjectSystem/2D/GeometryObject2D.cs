using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd.swig;

namespace asd
{
	/// <summary>
	/// 図形の描画を扱うクラス。
	/// </summary>
	public class GeometryObject2D : DrawnObject2D, IReleasable
	{
		private Shape shape;

		/// <summary>
		/// asd.GeometryObject2D の新しいインスタンスを初期化します。
		/// </summary>
		public GeometryObject2D()
		{
			shape = null;
			coreObject = Engine.ObjectSystemFactory.CreateGeometryObject2D();

			var p = coreObject.GetPtr();
			if(GC.Object2Ds.GetObject(p) != null)
			{
				Particular.Helper.ThrowException("");
			}
			GC.Object2Ds.AddObject(p, this);
		}

		#region GC対応
		~GeometryObject2D()
		{
			ForceToRelease();
		}

		public override bool IsReleased
		{
			get { return coreObject == null; }
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
			lock (this)
			{
				if(coreObject == null) return;
				GC.Collector.AddObject(coreObject);
				coreObject = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
		#endregion

		/// <summary>
		/// この2Dオブジェクトの描画に使用する図形を取得または設定します。
		/// </summary>
		public Shape Shape
		{
			get
			{
				return shape;
			}
			set
			{
				ThrowIfReleased();
				coreObject.SetShape(value.CoreShape);
				shape = value;
			}
		}


		/// <summary>
		/// この2Dオブジェクトの描画に使用するテクスチャを取得または設定します。
		/// </summary>
		public Texture2D Texture
		{
			get
			{
				ThrowIfReleased();
				return GC.GenerateTexture2D(coreObject.GetTexture(), GenerationType.Get);
			}
			set
			{
				ThrowIfReleased();
				coreObject.SetTexture(IG.GetTexture2D(value));
			}
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の描画原点を取得または設定します。描画原点は拡大・回転・描画の中心となる、画像データ内での座標です。
		/// </summary>
		public Vector2DF CenterPosition
		{
			get
			{
				ThrowIfReleased();
				return coreObject.GetCenterPosition();
			}
			set
			{
				ThrowIfReleased();
				coreObject.SetCenterPosition(value);
			}
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際に合成する色を取得または設定します。
		/// </summary>
		public override Color Color
		{
			get
			{
				ThrowIfReleased();
				return coreObject.GetColor();
			}
			set
			{
				ThrowIfReleased();
				coreObject.SetColor(value);
			}
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の描画優先度を取得または設定します。描画優先度が高いほど手前に描画されます。
		/// </summary>
		public override int DrawingPriority
		{
			get
			{
				ThrowIfReleased();
				return coreObject.GetDrawingPriority();
			}
			set
			{
				ThrowIfReleased();
				coreObject.SetDrawingPriority(value);
			}
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際のブレンドモードを取得または設定します。
		/// </summary>
		public AlphaBlendMode AlphaBlend
		{
			get
			{
				ThrowIfReleased();
				return (AlphaBlendMode)coreObject.GetAlphaBlendMode();
			}
			set
			{
				ThrowIfReleased();
				coreObject.SetAlphaBlendMode((swig.AlphaBlendMode)value);
			}
		}

		///<summary>
		/// この2Dオブジェクトを描画する際のテクスチャフィルタを取得または設定する。
		/// </summary>
		public TextureFilterType TextureFilterType
		{
			get
			{
				ThrowIfReleased();
				return (TextureFilterType)coreObject.GetTextureFilterType();
			}
			set
			{
				ThrowIfReleased();
				coreObject.SetTextureFilterType((swig.TextureFilterType)value);
			}
		}

		internal override swig.CoreObject2D CoreObject
		{
			get { return coreObject; }
		}
		internal override CoreDrawnObject2D CoreDrawnObject
		{
			get { return coreObject; }
		}

		private swig.CoreGeometryObject2D coreObject { get; set; }
	}
}

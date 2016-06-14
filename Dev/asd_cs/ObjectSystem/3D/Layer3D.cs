using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 3Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス
	/// </summary>
	public class Layer3D : Layer, IObjectRegisterable<Object3D>
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
				settings_.VisualizedBuffer = (swig.VisualizedBufferType)settings.VisualizedBuffer;
			}

			coreLayer3D = Engine.ObjectSystemFactory.CreateLayer3D(settings_);

			var p = coreLayer3D.GetPtr();
			if (GC.Layer3Ds.GetObject(p) != null)
			{
				Particular.Helper.ThrowException("");
			}

			GC.Layer3Ds.AddObject(p, this);

			ObjectManager = new ObjectManager<Object3D>(this);

			CoreLayer = coreLayer3D;
		}

		#region GC対策
		~Layer3D()
		{
			ForceToRelease();
		}

		public override bool IsReleased
		{
			get { return coreLayer3D == null; }
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
				if (coreLayer3D == null) return;
				GC.Collector.AddObject(coreLayer3D);
				coreLayer3D = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
		#endregion

		swig.CoreLayer3D coreLayer3D { get; set; }

		/// <summary>
		/// このレイヤーが管理している3Dオブジェクトのコレクションを取得する。
		/// </summary>
		public IEnumerable<Object3D> Objects
		{
			get { return ObjectManager.Contents; }
		}

		/// <summary>
		/// このレイヤーが管理している3Dオブジェクトの数を取得する。
		/// </summary>
		public override int ObjectCount
		{
			get { return asd.Particular.Helper.CountIterable(Objects); }
		}

		/// <summary>
		/// 描画設定を取得、設定する。
		/// </summary>
		public RenderSettings Settings
		{
			get
			{
				ThrowIfReleased();
				return new RenderSettings(coreLayer3D.GetRenderSettings());
			}
			set
			{
				ThrowIfReleased();
				swig.RenderSettings settings_ = new swig.RenderSettings();
				settings_.IsLightweightMode = value.IsLightweightMode;
				settings_.VisualizedBuffer = (swig.VisualizedBufferType)value.VisualizedBuffer;
				coreLayer3D.SetRenderSettings(settings_);
			}
		}

		/// <summary>
		/// このレイヤーに指定した3Dオブジェクトを追加する。
		/// </summary>
		/// <param name="object3D">追加する3Dオブジェクト</param>
		/// <remarks><see cref="Objects"/>プロパティの内容などへ実際に追加されるのは、このメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public void AddObject(Object3D object3D)
		{
			ObjectManager.Add(object3D);
		}

		/// <summary>
		/// このレイヤーから指定した3Dオブジェクトを削除する。
		/// </summary>
		/// <param name="object3D">削除される3Dオブジェクト</param>
		/// <remarks><see cref="Objects"/>プロパティの内容などから実際に削除されるのは、このメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public void RemoveObject(Object3D object3D)
		{
			ObjectManager.Remove(object3D, true);
		}

		internal void ImmediatelyRemoveObject(Object3D object3D, bool raiseEvent)
		{
			ObjectManager.Remove(object3D, raiseEvent);
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="obj"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __Register(Object3D obj)
		{
			obj.Layer = this;
			coreLayer3D.AddObject(obj.CoreObject);
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="obj"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __Unregister(Object3D obj)
		{
			obj.Layer = null;
			coreLayer3D.RemoveObject(obj.CoreObject);
		}

		/// <summary>
		/// 通常の描画に加えてテクスチャを描画する。
		/// </summary>
		/// <param name="upperLeftPos">テクスチャの左上の描画位置</param>
		/// <param name="upperRightPos">テクスチャの右上の描画位置</param>
		/// <param name="lowerRightPos">テクスチャの右下の描画位置</param>
		/// <param name="lowerLeftPos">テクスチャの左下の描画位置</param>
		/// <param name="upperLeftCol">テクスチャの左上の頂点色</param>
		/// <param name="upperRightCol">テクスチャの右上の頂点色</param>
		/// <param name="lowerRightCol">テクスチャの右下の頂点色</param>
		/// <param name="lowerLeftCol">テクスチャの左下の頂点色</param>
		/// <param name="upperLeftUV">テクスチャの左上のUV値</param>
		/// <param name="upperRightUV">テクスチャの右上のUV値</param>
		/// <param name="lowerRightUV">テクスチャの右下のUV値</param>
		/// <param name="lowerLeftUV">テクスチャの左下のUV値</param>
		/// <param name="texture">描画するテクスチャ</param>
		/// <param name="alphaBlend">アルファブレンドの種類</param>
		/// <param name="depthWrite">深度を比較するか?</param>
		/// <param name="depthTest">深度を書き込むか?</param>
		/// <remarks>OnDrawAdditionallyの中以外では実行してはいけない。</remarks>
		public void DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Texture2D texture, AlphaBlendMode alphaBlend, bool depthWrite, bool depthTest)
		{
			ThrowIfReleased();
			coreLayer3D.DrawSpriteAdditionally(
				upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, IG.GetTexture2D(texture), (swig.AlphaBlendMode)alphaBlend, depthWrite, depthTest);
		}

		/// <summary>
		/// 環境光の強さを設定する。
		/// </summary>
		/// <param name="ambientColorIntensity">環境光の強さ</param>
		/// <remarks>
		/// デフォルトは1.0である。
		/// </remarks>
		public void SetAmbientColorIntensity(float ambientColorIntensity)
		{
			ThrowIfReleased();
			coreLayer3D.SetAmbientColorIntensity(ambientColorIntensity);
		}

		/// <summary>
		/// 空方向の環境光の色を設定する。
		/// </summary>
		/// <param name="color">色</param>
		public void SetSkyAmbientColor(Color color)
		{
			ThrowIfReleased();
			coreLayer3D.SetSkyAmbientColor(color);
		}
	
		/// <summary>
		/// 地面方向の環境光の色を設定する。
		/// </summary>
		/// <param name="color">色</param>
		public void SetGroundAmbientColor(Color color)
		{
			ThrowIfReleased();
			coreLayer3D.SetGroundAmbientColor(color);
		}

		/// <summary>
		/// テクスチャによる環境の色の強さを設定する。
		/// </summary>
		/// <param name="environmentDiffuseColorIntensity">拡散色の強さ</param>
		/// <param name="environmentSpecularColorIntensity">スペキュラ色の強さ</param>
		/// <remarks>
		/// デフォルトは1.0である。
		/// </remarks>
		public void SetEnvironmentColorIntensity(float environmentDiffuseColorIntensity, float environmentSpecularColorIntensity)
		{
			ThrowIfReleased();
			coreLayer3D.SetEnvironmentColorIntensity(environmentDiffuseColorIntensity, environmentSpecularColorIntensity);
		}

		/// <summary>
		/// テクスチャによる環境の色を設定する。
		/// </summary>
		/// <param name="diffuseColor">拡散色</param>
		/// <param name="specularColor">スペキュラ色</param>
		public void SetEnvironmentColor(CubemapTexture diffuseColor, CubemapTexture specularColor)
		{
			ThrowIfReleased();
			coreLayer3D.SetEnvironmentColor(IG.GetCubemapTexture(diffuseColor), IG.GetCubemapTexture(specularColor));
		}

		/// <summary>
		/// SSAOのサンプリングする半径
		/// </summary>
		public float SSAO_Radius
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetSSAO_Radius(); }
			set { ThrowIfReleased();
				coreLayer3D.SetSSAO_Radius(value); }
		}

		/// <summary>
		/// SSAOのバイアス
		/// </summary>
		public float SSAO_Bias
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetSSAO_Bias(); }
			set { ThrowIfReleased();
				coreLayer3D.SetSSAO_Bias(value); }
		}

		/// <summary>
		/// SSAOの強さ
		/// </summary>
		public float SSAO_Intensity
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetSSAO_Intensity(); }
			set { ThrowIfReleased();
				coreLayer3D.SetSSAO_Intensity(value); }
		}

		/// <summary>
		/// SSAOの最大距離
		/// </summary>
		public float SSAO_FarPlain
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetSSAO_FarPlain(); }
			set { ThrowIfReleased();
				coreLayer3D.SetSSAO_FarPlain(value); }
		}

		/// <summary>
		/// 高さ方向のフォグが有効かどうか、を取得または設定する。
		/// </summary>
		public bool IsHeightFogEnabled
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetIsHeightFogEnabled(); }
			set { ThrowIfReleased();
				coreLayer3D.SetIsHeightFogEnabled(value); }
		}

		/// <summary>
		/// 高さ方向のフォグの濃度を取得または設定する。
		/// </summary>
		public float HeightFogDensity
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetHeightFogDensity(); }
			set { ThrowIfReleased();
				coreLayer3D.SetHeightFogDensity(value); }
		}

		/// <summary>
		/// 高さ方向のフォグの色を取得、または設定する。
		/// </summary>
		public Color HeightFogColor
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetHeightFogColor(); }
			set { ThrowIfReleased();
				coreLayer3D.SetHeightFogColor(value); }
		}

		/// <summary>
		/// 高さ方向のフォグの減衰率を取得、または設定する。
		/// </summary>
		/// <remarks>
		/// この値が大きいほど、高い位置のフォグが薄くなる。
		/// </remarks>
		public float HeightFogFalloff
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetHeightFogFalloff(); }
			set { ThrowIfReleased();
				coreLayer3D.SetHeightFogFalloff(value); }
		}

		/// <summary>
		/// 高さ方向のフォグの開始距離を取得、または設定する。
		/// </summary>
		/// <remarks>
		/// この値よりカメラに対して近い位置には霧が生じなくなる。
		/// </remarks>
		public float HeightFogStartDistance
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetHeightFogStartDistance(); }
			set { ThrowIfReleased();
				coreLayer3D.SetHeightFogStartDistance(value); }
		}

		public bool IsOceanEnabled
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetIsOceanEnabled(); }
			set { ThrowIfReleased();
				coreLayer3D.SetIsOceanEnabled(value); }
		}

		public RectF OceanArea
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetOceanArea(); }
			set { ThrowIfReleased();
				coreLayer3D.SetOceanArea(value); }
		}

		public float OceanGridSize
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetOceanGridSize(); }
			set { ThrowIfReleased();
				coreLayer3D.SetOceanGridSize(value); }
		}

		public float OceanHeight
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetOceanHeight(); }
			set { ThrowIfReleased();
				coreLayer3D.SetOceanHeight(value); }
		}

		public Color OceanColor
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetOceanColor(); }
			set { ThrowIfReleased();
				coreLayer3D.SetOceanColor(value); }
		}

		public float OceanDensity
		{
			get { ThrowIfReleased();
				return coreLayer3D.GetOceanDensity(); }
			set { ThrowIfReleased();
				coreLayer3D.SetOceanDensity(value); }
		}

		public Texture2D OceanNormalMap
		{
			get { ThrowIfReleased();
				return GC.GenerateTexture2D(coreLayer3D.GetOceanNormalMap(), GenerationType.Get); }
			set { ThrowIfReleased();
				coreLayer3D.SetOceanNormalMap(IG.GetTexture2D(value)); }
		}

		public Texture2D OceanMask
		{
			get { ThrowIfReleased();
				return GC.GenerateTexture2D(coreLayer3D.GetOceanMask(), GenerationType.Get); }
			set { ThrowIfReleased();
				coreLayer3D.SetOceanMask(IG.GetTexture2D(value)); }
		}

		public override LayerType LayerType
		{
			get { return LayerType.Layer3D; }
		}

		internal override void BeginUpdating()
		{
			if(!IsAlive)
			{
				return;
			}
			isUpdatedCurrent = IsUpdated;
			coreLayer3D.BeginUpdating(isUpdatedCurrent);
		}

		internal override void EndUpdating()
		{
			if(!IsAlive)
			{
				return;
			}
			coreLayer3D.EndUpdating(isUpdatedCurrent);
		}

		internal override void UpdateInternal()
		{
			ObjectManager.UpdateObjects();
		}

		internal override void DrawAdditionally()
		{
			if (!IsAlive || !IsDrawn)
			{
				return;
			}

			foreach (var item in Objects)
			{
				item.DrawAdditionally();
			}

			OnDrawAdditionally();
		}

		internal override void DisposeContents(bool disposeNative)
		{
			ObjectManager.DisposeObjects(disposeNative);
		}

		private ObjectManager<Object3D> ObjectManager { get; set; }
	}
}

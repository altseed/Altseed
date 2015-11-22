using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 3Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス
	/// </summary>
	public class Layer3D : Layer, IReleasable
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

			contentsManager = new ContentsManager<Object3D>();

			commonObject = coreLayer3D;
		}

		#region GC対策
		~Layer3D()
		{
			ForceToRelease();
		}

		public bool IsReleased
		{
			get { return coreLayer3D == null; }
		}

		public void ForceToRelease()
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
			get { return contentsManager.Contents; }
		}

		public override int ObjectCount
		{
			get { return Objects.Count(); }
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
				settings_.VisualizedBuffer = (swig.VisualizedBufferType)value.VisualizedBuffer;
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
				Particular.Helper.ThrowException("指定したオブジェクトは既に別のレイヤーに所属しています。");
			}
			contentsManager.Add(object3D);
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
			contentsManager.Remove(object3D);
			coreLayer3D.RemoveObject(object3D.CoreObject);
			object3D.Layer = null;
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
			coreLayer3D.SetAmbientColorIntensity(ambientColorIntensity);
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
			coreLayer3D.SetEnvironmentColorIntensity(environmentDiffuseColorIntensity, environmentSpecularColorIntensity);
		}

		/// <summary>
		/// テクスチャによる環境の色を設定する。
		/// </summary>
		/// <param name="diffuseColor">拡散色</param>
		/// <param name="specularColor">スペキュラ色</param>
		public void SetEnvironmentColor(CubemapTexture diffuseColor, CubemapTexture specularColor)
		{
			coreLayer3D.SetEnvironmentColor(IG.GetCubemapTexture(diffuseColor), IG.GetCubemapTexture(specularColor));
		}

		/// <summary>
		/// SSAOのサンプリングする半径
		/// </summary>
		public float SSAO_Radius
		{
			get { return coreLayer3D.GetSSAO_Radius(); }
			set { coreLayer3D.SetSSAO_Radius(value); }
		}

		/// <summary>
		/// SSAOのバイアス
		/// </summary>
		public float SSAO_Bias
		{
			get { return coreLayer3D.GetSSAO_Bias(); }
			set { coreLayer3D.SetSSAO_Bias(value); }
		}

		/// <summary>
		/// SSAOの強さ
		/// </summary>
		public float SSAO_Intensity
		{
			get { return coreLayer3D.GetSSAO_Intensity(); }
			set { coreLayer3D.SetSSAO_Intensity(value); }
		}

		/// <summary>
		/// SSAOの最大距離
		/// </summary>
		public float SSAO_FarPlain
		{
			get { return coreLayer3D.GetSSAO_FarPlain(); }
			set { coreLayer3D.SetSSAO_FarPlain(value); }
		}

		/// <summary>
		/// 高さ方向のフォグが有効かどうか、を取得または設定する。
		/// </summary>
		public bool IsHeightFogEnabled
		{
			get { return coreLayer3D.GetIsHeightFogEnabled(); }
			set { coreLayer3D.SetIsHeightFogEnabled(value); }
		}

		/// <summary>
		/// 高さ方向のフォグの濃度を取得または設定する。
		/// </summary>
		public float HeightFogDensity
		{
			get { return coreLayer3D.GetHeightFogDensity(); }
			set { coreLayer3D.SetHeightFogDensity(value); }
		}

		/// <summary>
		/// 高さ方向のフォグの色を取得、または設定する。
		/// </summary>
		public Color HeightFogColor
		{
			get { return coreLayer3D.GetHeightFogColor(); }
			set { coreLayer3D.SetHeightFogColor(value); }
		}

		/// <summary>
		/// 高さ方向のフォグの減衰率を取得、または設定する。
		/// </summary>
		/// <remarks>
		/// この値が大きいほど、高い位置のフォグが薄くなる。
		/// </remarks>
		public float HeightFogFalloff
		{
			get { return coreLayer3D.GetHeightFogFalloff(); }
			set { coreLayer3D.SetHeightFogFalloff(value); }
		}

		/// <summary>
		/// 高さ方向のフォグの開始距離を取得、または設定する。
		/// </summary>
		/// <remarks>
		/// この値よりカメラに対して近い位置には霧が生じなくなる。
		/// </remarks>
		public float HeightFogStartDistance
		{
			get { return coreLayer3D.GetHeightFogStartDistance(); }
			set { coreLayer3D.SetHeightFogStartDistance(value); }
		}

		public override LayerType LayerType
		{
			get { return LayerType.Layer3D; }
		}

		internal override void BeginUpdating()
		{
			isUpdatedCurrent = IsUpdated;
			coreLayer3D.BeginUpdating(isUpdatedCurrent);
		}

		internal override void EndUpdating()
		{
			coreLayer3D.EndUpdating(isUpdatedCurrent);
		}

		internal override void Update()
		{
			if (!isUpdatedCurrent || !IsAlive)
			{
				return;
			}

			CoreLayer.BeginMeasureUpdateTime();
			OnUpdating();

			contentsManager.Update();

			foreach (var vanishing in contentsManager.VanishingContents)
			{
				RemoveObject(vanishing);
                vanishing.Dispose();
			}
			contentsManager.VanishingContents.Clear();

			OnUpdated();
			CoreLayer.EndMeasureUpdateTime();
		}

		internal override void DrawAdditionally()
		{
			if (!IsDrawn)
			{
				return;
			}

			foreach (var item in contentsManager.Contents)
			{
				item.DrawAdditionally();
			}

			OnDrawAdditionally();
		}

		internal override void Dispose()
		{
			foreach(var item in Objects)
			{
				if(item.IsAlive)
				{
					item.Dispose();
				}
			}
			OnDispose();
		}


		private ContentsManager<Object3D> contentsManager { get;set; }
	}
}

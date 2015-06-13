using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 2Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス
	/// </summary>
	public class Layer2D : Layer, IDestroy
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Layer2D()
		{
			coreLayer2D = Engine.ObjectSystemFactory.CreateLayer2D();

			var p = coreLayer2D.GetPtr();
			if(GC.Layer2Ds.GetObject(p) != null)
			{
				Particular.Helper.ThrowException("");
			}

			GC.Layer2Ds.AddObject(p, this);

			contentsManager = new ContentsManager<Object2D>();
			componentManager = new ComponentManager<Layer2D, Layer2DComponent>(this);

			commonObject = coreLayer2D;
		}

		#region GC対策
		~Layer2D()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get { return coreLayer2D == null; }
		}

		public void Destroy()
		{
			lock (this)
			{
				if(coreLayer2D == null) return;
				GC.Collector.AddObject(coreLayer2D);
				coreLayer2D = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
		#endregion

		public override int ObjectCount
		{
			get { return Objects.Count(); }
		}


		/// <summary>
		/// このレイヤーが管理している2Dオブジェクトのコレクションを取得する。
		/// </summary>
		public IEnumerable<Object2D> Objects
		{
			get { return contentsManager.Contents; }
		}

		/// <summary>
		/// 指定した2Dオブジェクトをこのレイヤーに追加する。
		/// </summary>
		/// <param name="object2D">追加する2Dオブジェクト</param>
		public void AddObject(Object2D object2D)
		{
			if(object2D.Layer != null)
			{
				Particular.Helper.ThrowException("指定したオブジェクトは既に別のレイヤーに所属しています。");
			}

			contentsManager.Add(object2D);
			coreLayer2D.AddObject(object2D.CoreObject);
			object2D.Layer = this;
			object2D.Start();
		}

		/// <summary>
		/// 指定した2Dオブジェクトをこのレイヤーから削除する。
		/// </summary>
		/// <param name="object2D">削除される2Dオブジェクト</param>
		public void RemoveObject(Object2D object2D)
		{
			contentsManager.Remove(object2D);
			coreLayer2D.RemoveObject(object2D.CoreObject);
			object2D.Layer = null;
		}

		/// <summary>
		/// 指定したコンポーネントをこのレイヤーに追加する。
		/// </summary>
		/// <param name="component">追加するコンポーネント</param>
		/// <param name="key">コンポーネントに関連付けるキー</param>
		public void AddComponent(Layer2DComponent component, string key)
		{
			componentManager.Add(component, key);
		}

		/// <summary>
		/// 指定したキーを持つコンポーネントを取得する。
		/// </summary>
		/// <param name="key">取得するコンポーネントのキー</param>
		/// <returns>コンポーネント</returns>
		public Layer2DComponent GetComponent(string key)
		{
			return componentManager.Get(key);
		}

		/// <summary>
		/// 指定したコンポーネントをこのレイヤーから削除する。
		/// </summary>
		/// <param name="key">削除するコンポーネントを示すキー</param>
		/// <returns>削除が成功したか否か。キーに対応するコンポーネントがなかった場合は false。</returns>
		public bool RemoveComponent(string key)
		{
			return componentManager.Remove(key);
		}

		/// <summary>
		/// 保持している全ての2Dオブジェクトをこのレイヤーから削除する。
		/// </summary>
		public void Clear()
		{
			foreach(var obj in contentsManager.Contents)
			{
				obj.Layer = null;
			}
			coreLayer2D.Clear();
			contentsManager.Clear();
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
		/// <param name="priority">描画の優先順位(大きいほど前面に描画される)</param>
		/// <remarks>OnDrawAdditionallyの中以外では実行してはいけない。</remarks>
		public void DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			coreLayer2D.DrawSpriteAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, IG.GetTexture2D(texture), (swig.AlphaBlendMode)alphaBlend, priority);
		}

		/// <summary>
		/// 通常の描画に加えて文字列を描画する。
		/// </summary>
		/// <param name="pos">描画位置</param>
		/// <param name="color">頂点色</param>
		/// <param name="font">フォント</param>
		/// <param name="text">文字列</param>
		/// <param name="writingDirection">行方向</param>
		/// <param name="alphaBlend">アルファブレンドの種類</param>
		/// <param name="priority">描画の優先順位(大きいほど前面に描画される)</param>
		/// <remarks>
		/// OnDrawAdditionallyの中以外では実行してはいけない。
		/// </remarks>
		public void DrawTextAdditionally(Vector2DF pos, Color color, Font font, string text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int priority)
		{
            if (coreLayer2D == null) return;
			coreLayer2D.DrawTextAdditionally(pos, color, IG.GetFont(font), text, (swig.WritingDirection)writingDirection, (swig.AlphaBlendMode)alphaBlend, priority);
		}

        public void DrawRectangleAdditionally(RectF drawingArea, Color color, RectF uv, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
        {
            if (coreLayer2D == null) return;
            coreLayer2D.DrawRectangleAdditionally(drawingArea, color, uv, (texture == null) ? null : texture.SwigObject, (swig.AlphaBlendMode)alphaBlend, priority);
        }

        public void DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
        {
            if (coreLayer2D == null) return;
            coreLayer2D.DrawRotatedRectangleAdditionally(drawingArea, color, rotationCenter, angle, uv, (texture == null) ? null : texture.SwigObject, (swig.AlphaBlendMode)alphaBlend, priority);
        }

        public void DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
        {
            if (coreLayer2D == null) return;
            coreLayer2D.DrawTriangleAdditionally(position1, position2, position3, color, uv1, uv2, uv3, (texture == null) ? null : texture.SwigObject, (swig.AlphaBlendMode)alphaBlend, priority);
        }

        public void DrawCircleAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
        {
            if (coreLayer2D == null) return;
            coreLayer2D.DrawCircleAdditionally(center, outerDiameter, innerDiameter, color, vertNum, angle, (texture==null)?null : texture.SwigObject, (swig.AlphaBlendMode)alphaBlend, priority);
        }

        public void DrawArcAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
        {
            if (coreLayer2D == null) return;
            coreLayer2D.DrawArcAdditionally(center, outerDiameter, innerDiameter, color, vertNum, startingVerticalAngle, endingVerticalAngle, angle, (texture == null) ? null : texture.SwigObject, (swig.AlphaBlendMode)alphaBlend, priority);
        }

        public void DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int priority)
        {
            if (coreLayer2D == null) return;
            coreLayer2D.DrawLineAdditionally(point1, point2, thickness, color, (swig.AlphaBlendMode)alphaBlend, priority);
        }

        public void DrawShapeAdditionally(Shape shape, Color color, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
        {
            if (coreLayer2D == null) return;
            coreLayer2D.DrawShapeAdditionally(shape.CoreShape, color, (texture == null) ? null : texture.SwigObject, (swig.AlphaBlendMode)alphaBlend, priority);
        }

        public override LayerType LayerType
		{
			get { return LayerType.Layer2D; }
		}

        internal override void BeginUpdating()
		{
			isUpdatedCurrent = IsUpdated;
			coreLayer2D.BeginUpdating(isUpdatedCurrent);
		}

		internal override void EndUpdating()
		{
			coreLayer2D.EndUpdating(isUpdatedCurrent);
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
			}
			contentsManager.VanishingContents.Clear();

			componentManager.Update();

			OnUpdated();
			CoreLayer.EndMeasureUpdateTime();
		}

		internal override void DrawAdditionally()
		{
			if(!IsDrawn)
			{
				return;
			}

			foreach(var item in contentsManager.Contents)
			{
				item.DrawAdditionally();
			}

			OnDrawAdditionally();
		}

		internal override void CallDestroy()
		{
			foreach(var item in Objects)
			{
				if(item.IsAlive)
				{
					item.CallDestroy();
				}
			}
			OnDispose();
		}

		private swig.CoreLayer2D coreLayer2D { get; set; }

		private ContentsManager<Object2D> contentsManager { get; set; }

		private ComponentManager<Layer2D, Layer2DComponent> componentManager { get; set; }
	}
}

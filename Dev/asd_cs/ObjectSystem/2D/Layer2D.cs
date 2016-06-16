using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 2Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス
	/// </summary>
	public class Layer2D : Layer, IObjectRegisterable<Object2D>, IComponentRegisterable<Layer2DComponent>
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

			ObjectManager = new ObjectManager<Object2D>(this);
			ComponentManager = new ComponentManager<Layer2DComponent>(this);

			CoreLayer = coreLayer2D;
		}

		#region GC対策
		~Layer2D()
		{
			ForceToRelease();
		}

		public override bool IsReleased
		{
			get { return coreLayer2D == null; }
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
				if(coreLayer2D == null) return;
				GC.Collector.AddObject(coreLayer2D);
				coreLayer2D = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
		#endregion

		/// <summary>
		/// このレイヤーが管理している2Dオブジェクトの数を取得する。
		/// </summary>
		public override int ObjectCount
		{
			get { return asd.Particular.Helper.CountIterable(Objects); }
		}

		/// <summary>
		/// このレイヤーが管理している2Dオブジェクトのコレクションを取得する。
		/// </summary>
		public IEnumerable<Object2D> Objects
		{
			get { return ObjectManager.Contents; }
		}

		#region オブジェクト管理
		/// <summary>
		/// 指定した2Dオブジェクトをこのレイヤーに追加する。
		/// </summary>
		/// <param name="object2D">追加する2Dオブジェクト</param>
		/// <remarks>実際に追加されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public void AddObject(Object2D object2D)
		{
			ObjectManager.Add(object2D);
		}

		/// <summary>
		/// 指定した2Dオブジェクトをこのレイヤーから削除する。
		/// </summary>
		/// <param name="object2D">削除される2Dオブジェクト</param>
		/// <remarks>実際に削除されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public void RemoveObject(Object2D object2D)
		{
			ObjectManager.Remove(object2D, true);
		}

		/// <summary>
		/// 保持している全ての2Dオブジェクトをこのレイヤーから削除する。
		/// </summary>
		/// <remarks>実際に削除されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public void Clear()
		{
			ObjectManager.Clear();
		}

		/// <summary>
		/// 指定した2Dオブジェクトをすぐにこのレイヤーに追加する。
		/// </summary>
		/// <param name="object2D">追加する2Dオブジェクト</param>
		/// <param name="raiseEvent">OnRemovedイベントを呼び出すかどうかの真偽値</param>
		internal void ImmediatelyRemoveObject(Object2D object2D, bool raiseEvent)
		{
			ObjectManager.ImmediatelyRemoveObject(object2D, raiseEvent);
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="obj"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __Register(Object2D obj)
		{
			obj.Layer = this;
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="obj"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __Unregister(Object2D obj)
		{
			obj.Layer = null;
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="obj"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __AddToCore(Object2D obj)
		{
			coreLayer2D.AddObject(obj.CoreObject);
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="obj"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __RemoveFromCore(Object2D obj)
		{
			coreLayer2D.RemoveObject(obj.CoreObject);
		}

		#endregion

		#region コンポーネント管理
		/// <summary>
		/// 指定したコンポーネントをこのレイヤーに追加する。
		/// </summary>
		/// <param name="component">追加するコンポーネント</param>
		/// <param name="key">コンポーネントに関連付けるキー</param>
		/// <remarks>実際に追加されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public void AddComponent(Layer2DComponent component, string key)
		{
			ComponentManager.Add(component, key);
		}

		/// <summary>
		/// 指定したコンポーネントをこのレイヤーから削除する。
		/// </summary>
		/// <param name="key">削除するコンポーネントを示すキー</param>
		/// <returns>削除が成功したか否か。キーに対応するコンポーネントがなかった場合は false。</returns>
		/// <remarks>実際に削除されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public bool RemoveComponent(string key)
		{
			return ComponentManager.Remove(key);
		}

		/// <summary>
		/// 指定したキーを持つコンポーネントを取得する。
		/// </summary>
		/// <param name="key">取得するコンポーネントのキー</param>
		/// <returns>コンポーネント</returns>
		public Layer2DComponent GetComponent(string key)
		{
			return ComponentManager.Get(key);
		}

		internal void ImmediatelyRemoveComponent(string key)
		{
			ComponentManager.ImmediatelyRemoveComponent(key);
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="component"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __Register(Layer2DComponent component)
		{
			component.Owner = this;
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="component"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __Unregister(Layer2DComponent component)
		{
			component.Owner = null;
		}
		#endregion

		#region DrawAdditionally
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
		/// 通常の描画に加えて素材を用いて2Dスプライトを描画する。
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
		/// <param name="material">表示に使用するマテリアル</param>
		/// <param name="alphaBlend">アルファブレンドの種類</param>
		/// <param name="priority">描画の優先順位(大きいほど前面に描画される)</param>
		/// <remarks>OnDrawAdditionallyの中以外では実行してはいけない。</remarks>
		public void DrawSpriteWithMaterialAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Material2D material, AlphaBlendMode alphaBlend, int priority)
		{
			coreLayer2D.DrawSpriteWithMaterialAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, IG.GetMaterial2D(material), (swig.AlphaBlendMode)alphaBlend, priority);
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
			coreLayer2D.DrawTextAdditionally(pos, color, IG.GetFont(font), text, (swig.WritingDirection)writingDirection, (swig.AlphaBlendMode)alphaBlend, priority);
		}

		public void DrawRectangleAdditionally(RectF drawingArea, Color color, RectF uv, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			coreLayer2D.DrawRectangleAdditionally(drawingArea, color, uv, IG.GetTexture2D(texture), (swig.AlphaBlendMode)alphaBlend, priority);
		}

		public void DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			coreLayer2D.DrawRotatedRectangleAdditionally(drawingArea, color, rotationCenter, angle, uv, IG.GetTexture2D(texture), (swig.AlphaBlendMode)alphaBlend, priority);
		}

		public void DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			coreLayer2D.DrawTriangleAdditionally(position1, position2, position3, color, uv1, uv2, uv3, IG.GetTexture2D(texture), (swig.AlphaBlendMode)alphaBlend, priority);
		}

		public void DrawCircleAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			coreLayer2D.DrawCircleAdditionally(center, outerDiameter, innerDiameter, color, vertNum, angle, IG.GetTexture2D(texture), (swig.AlphaBlendMode)alphaBlend, priority);
		}

		public void DrawArcAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			coreLayer2D.DrawArcAdditionally(center, outerDiameter, innerDiameter, color, vertNum, startingVerticalAngle, endingVerticalAngle, angle, IG.GetTexture2D(texture), (swig.AlphaBlendMode)alphaBlend, priority);
		}

		public void DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int priority)
		{
			coreLayer2D.DrawLineAdditionally(point1, point2, thickness, color, (swig.AlphaBlendMode)alphaBlend, priority);
		}

		public void DrawShapeAdditionally(Shape shape, Color color, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			coreLayer2D.DrawShapeAdditionally(shape.CoreShape, color, IG.GetTexture2D(texture), (swig.AlphaBlendMode)alphaBlend, priority);
		}

		#endregion

		internal override void BeginUpdating()
		{
			if(!IsAlive)
			{
				return;
			}
			isUpdatedCurrent = IsUpdated;
			coreLayer2D.BeginUpdating(isUpdatedCurrent);
		}

		internal override void EndUpdating()
		{
			if(!IsAlive)
			{
				return;
			}
			coreLayer2D.EndUpdating(isUpdatedCurrent);
		}

		internal override void UpdateInternal()
		{
			foreach(var component in ComponentManager.Components)
			{
				component.RaiseOnUpdating();
			}

			ObjectManager.UpdateObjects();

			foreach(var component in ComponentManager.Components)
			{
				component.RaiseOnUpdated();
			}
		}

		internal override void DrawAdditionally()
		{
			if(!IsAlive || !IsDrawn)
			{
				return;
			}

			foreach(var item in Objects)
			{
				item.DrawAdditionally();
			}

			OnDrawAdditionally();
		}

		internal override void DisposeSpecifics(bool disposeNative)
		{
			ObjectManager.DisposeObjects(disposeNative);
			foreach (var component in ComponentManager.Components)
			{
				component.RaiseOnDisposed();
			}
		}

		protected override void OnAdded()
		{
			base.OnAdded();
			foreach(var component in ComponentManager.Components)
			{
				component.RaiesOnAdded();
			}
		}

		protected override void OnRemoved()
		{
			foreach(var component in ComponentManager.Components)
			{
				component.RaiesOnRemoved();
			}
			base.OnRemoved();
		}


		public override LayerType LayerType
		{
			get { return LayerType.Layer2D; }
		}

		private swig.CoreLayer2D coreLayer2D_;

		private swig.CoreLayer2D coreLayer2D
		{
			get { return coreLayer2D_; }
			set { coreLayer2D_ = value; }
		}

		private ObjectManager<Object2D> ObjectManager { get; set; }

		private ComponentManager<Layer2DComponent> ComponentManager { get; set; }
	}
}

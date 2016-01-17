using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace asd
{
	internal class ParentInfo2D
	{
		public Object2D Parent { get; private set; }
		public ChildManagementMode ManagementMode { get; private set; }

		public ParentInfo2D(Object2D parent, ChildManagementMode managementMode)
		{
			Parent = parent;
			ManagementMode = managementMode;
		}
	}

	/// <summary>
	/// 更新・描画処理を行う単位となる2Dオブジェクトの機能を提供する抽象クラス。
	/// </summary>
	public abstract class Object2D : Content, IReleasable
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Object2D()
		{
			componentManager_ = new ComponentManager<Object2D, Object2DComponent>(this);
			ChildrenList = new List<Object2D>();
			IsUpdated = true;
		}


		#region パラメータ
		/// <summary>
		/// カメラに表示するグループを取得する。
		/// </summary>
		/// <remarks>
		/// カメラのグループと他のオブジェクトのグループで&amp;でビット演算して1以上だった場合、そのカメラにオブジェクトが描画されるようになる。
		/// </remarks>
		public int CameraGroup
		{
			get { return CoreObject.GetCameraGroup(); }
			set { CoreObject.SetCameraGroup(value); }
		}

		/// <summary>
		/// このオブジェクトが更新されるかどうかを取得または設定する。
		/// </summary>
		public bool IsUpdated { get; set; }

		/// <summary>
		/// このオブジェクトが描画されるかどうかを取得または設定する。
		/// </summary>
		public bool IsDrawn
		{
			get { return CoreObject.GetIsDrawn(); }
			set { CoreObject.SetIsDrawn(value); }
		}

		/// <summary>
		/// このオブジェクトが実行中かどうかを取得する。Vanishメソッドにより削除された時に false を返す。
		/// </summary>
		public bool IsAlive
		{
			get { return CoreObject.GetIsAlive(); }
			private set { CoreObject.SetIsAlive(value); }
		}

		/// <summary>
		/// このインスタンスを管理している asd.Layer2D クラスのインスタンスを取得する。
		/// </summary>
		public Layer2D Layer { get; internal set; }

		/// <summary>
		/// このオブジェクトの親オブジェクトを取得する。
		/// </summary>
		public Object2D Parent
		{
			get { return ParentInfo.Parent; }
		}

		/// <summary>
		/// このオブジェクトが持っている子オブジェクトのコレクションを取得する。
		/// </summary>
		public IEnumerable<Object2D> Children
		{
			get { return ChildrenList; }
		}

		/// <summary>
		/// この2Dオブジェクトを描画する位置を取得または設定する。
		/// </summary>
		public Vector2DF Position
		{
			get { return CoreObject.GetPosition(); }
			set { CoreObject.SetPosition(value); }
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の回転角度を取得または設定する。
		/// </summary>
		/// <remarks>
		/// 回転角度は度で表記され、回転方向は時計回りである。
		/// </remarks>
		public float Angle
		{
			get { return CoreObject.GetAngle(); }
			set { CoreObject.SetAngle(value); }
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の拡大率を取得または設定する。
		/// </summary>
		public Vector2DF Scale
		{
			get { return CoreObject.GetScale(); }
			set { CoreObject.SetScale(value); }
		}

		/// <summary>
		/// このオブジェクトが親子関係を考慮して最終的に更新されるかどうかの真偽値を取得します。
		/// </summary>
		public bool AbsoluteBeingUpdated
		{
			get
			{
				return IsUpdated
					&& !(ParentInfo != null
					&& (ParentInfo.ManagementMode & ChildManagementMode.IsUpdated) != 0
					&& !ParentInfo.Parent.AbsoluteBeingUpdated);
			}
		}

		/// <summary>
		/// このオブジェクトが親子関係を考慮して最終的に描画されるかどうかの真偽値を取得します。
		/// </summary>
		public bool AbsoluteBeingDrawn
		{
			get { return CoreObject.GetAbsoluteBeingDrawn(); }
		}
		#endregion


		/// <summary>
		/// この2Dオブジェクトを描画する際の実際の位置を取得または設定する。親子関係がある場合に、親の位置を考慮した位置を取得できる。
		/// </summary>
		/// <returns>この2Dオブジェクトの位置。</returns>
		public Vector2DF GetGlobalPosition()
		{
			return CoreObject.GetGlobalPosition();
		}

		/// <summary>
		/// 指定した2Dオブジェクトを子オブジェクトとしてこのインスタンスに追加する。
		/// </summary>
		/// <param name="child">追加する子オブジェクト</param>
		/// <param name="managementMode">子オブジェクトの管理に関する同期設定。フラグをOR演算でつなげて複数指定することができる。</param>
		/// <param name="transformingMode">子オブジェクトの変形に関する同期設定。</param>
		public void AddChild(Object2D child, ChildManagementMode managementMode, ChildTransformingMode transformingMode)
		{
			CoreObject.AddChild(child.CoreObject, (int)managementMode, (swig.ChildTransformingMode)transformingMode);
			ChildrenList.Add(child);
			child.ParentInfo = new ParentInfo2D(this, managementMode);
			if((managementMode & ChildManagementMode.RegistrationToLayer) != 0)
			{
				if(child.Layer != Layer && child.Layer != null)
				{
					child.Layer.RemoveObject(child);
				}
				if(Layer != null)
				{
					Layer.AddObject(child);
				}
			}
		}

		/// <summary>
		/// 指定した子オブジェクトをこのインスタンスから削除する。
		/// </summary>
		/// <param name="child"></param>
		public void RemoveChild(Object2D child)
		{
			CoreObject.RemoveChild(child.CoreObject);
			ChildrenList.Remove(child);
			child.ParentInfo = null;
		}

		/// <summary>
		/// 指定したコンポーネントをこの2Dオブジェクトに追加する。
		/// </summary>
		/// <param name="component">追加するコンポーネント</param>
		/// <param name="key">コンポーネントに関連付けるキー</param>
		public void AddComponent(Object2DComponent component, string key)
		{
			componentManager_.Add(component, key);
		}

		/// <summary>
		/// 指定したキーを持つコンポーネントを取得する。
		/// </summary>
		/// <param name="key">取得するコンポーネントのキー</param>
		/// <returns>コンポーネント</returns>
		public Object2DComponent GetComponent(string key)
		{
			return componentManager_.Get(key);
		}

		/// <summary>
		/// 指定したコンポーネントをこの2Dオブジェクトから削除する。
		/// </summary>
		/// <param name="key">削除するコンポーネントを示すキー</param>
		/// <returns>削除が成功したか否か。キーに対応するコンポーネントがなかった場合は false。</returns>
		public bool RemoveComponent(string key)
		{
			return componentManager_.Remove(key);
		}


		#region 追加描画
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
		protected void DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			Layer.DrawSpriteAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, texture, alphaBlend, priority);
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
		protected void DrawTextAdditionally(Vector2DF pos, Color color, Font font, string text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int priority)
		{
			Layer.DrawTextAdditionally(pos, color, font, text, writingDirection, alphaBlend, priority);
		}

		protected void DrawRectangleAdditionally(RectF drawingArea, Color color, RectF uv, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			Layer.DrawRectangleAdditionally(drawingArea, color, uv, texture, alphaBlend, priority);
		}

		protected void DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			Layer.DrawRotatedRectangleAdditionally(drawingArea, color, rotationCenter, angle, uv, texture, alphaBlend, priority);
		}

		protected void DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			Layer.DrawTriangleAdditionally(position1, position2, position3, color, uv1, uv2, uv3, texture, alphaBlend, priority);
		}

		protected void DrawCircleAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			Layer.DrawCircleAdditionally(center, outerDiameter, innerDiameter, color, vertNum, angle, texture, alphaBlend, priority);
		}

		protected void DrawArcAdditionally(Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			Layer.DrawArcAdditionally(center, outerDiameter, innerDiameter, color, vertNum, startingVerticalAngle, endingVerticalAngle, angle, texture, alphaBlend, priority);
		}

		protected void DrawLineAdditionally(Vector2DF point1, Vector2DF point2, Color color, float thickness, AlphaBlendMode alphaBlend, int priority)
		{
			Layer.DrawLineAdditionally(point1, point2, thickness, color, alphaBlend, priority);
		}

		protected void DrawShapeAdditionally(Shape shape, Color color, Texture2D texture, AlphaBlendMode alphaBlend, int priority)
		{
			Layer.DrawShapeAdditionally(shape, color, texture, alphaBlend, priority);
		}
		#endregion


		#region イベントハンドラ
		internal void RaiseOnAdded()
		{
			OnAdded();
			foreach(var item in ChildrenList)
			{
				if(item.IsInheriting(ChildManagementMode.RegistrationToLayer))
				{
					Layer.AddObject(item);
				}
			}
		}

		internal void RaiseOnRemoved()
		{
			foreach(var item in ChildrenList)
			{
				if(item.IsInheriting(ChildManagementMode.RegistrationToLayer))
				{
					Layer.RemoveObject(item);
				}
				item.ParentInfo = null;
			}
			OnRemoved();
		}

		public void Dispose()
		{
			foreach(var item in ChildrenList)
			{
				CoreObject.RemoveChild(item.CoreObject);
				item.ParentInfo = null;
				if(item.IsInheriting(ChildManagementMode.Vanishment))
				{
					item.Dispose();
				}
			}
			IsAlive = false;
			OnDispose();
			ForceToRelease();
		}

		internal override void Update()
		{
			if(AbsoluteBeingUpdated && IsAlive)
			{
				OnUpdate();
				componentManager_.Update();
			}
		}

		internal void DrawAdditionally()
		{
			if(AbsoluteBeingDrawn && IsAlive)
			{
				OnDrawAdditionally();
			}
		}

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトの初期化処理を記述できる。
		/// </summary>
		protected virtual void OnAdded() { }

		protected virtual void OnRemoved() { }

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトが破棄される際の処理を記述できる。
		/// </summary>
		protected virtual void OnDispose() { }

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトの更新処理を記述できる。
		/// </summary>
		protected virtual void OnUpdate() { }

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトに関する追加の描画処理を記述できる。
		/// </summary>
		protected virtual void OnDrawAdditionally() { }
		#endregion


		private bool IsInheriting(ChildManagementMode mode)
		{
			return ParentInfo != null && (ParentInfo.ManagementMode & mode) != 0;
		}

		private ComponentManager<Object2D, Object2DComponent> componentManager_ { get; set; }

		internal abstract swig.CoreObject2D CoreObject { get; }

		internal override bool GetIsAlive()
		{
			return IsAlive;
		}

		internal List<Object2D> ChildrenList { get; set; }

		internal ParentInfo2D ParentInfo { get; set; }

		public abstract bool IsReleased { get; }

		public abstract void ForceToRelease();
	}
}

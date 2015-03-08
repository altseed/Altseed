using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ace
{
	/// <summary>
	/// 更新・描画処理を行う単位となる2Dオブジェクトの機能を提供する抽象クラス。
	/// </summary>
	public abstract class Object2D : Content, IDestroy
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Object2D()
		{
			componentManager_ = new ComponentManager<Object2D, Object2DComponent>(this);
			children_ = new List<Object2D>();
			IsUpdated = true;
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
		/// このインスタンスを管理している ace.Layer2D クラスのインスタンスを取得する。
		/// </summary>
		public Layer2D Layer { get; internal set; }


		/// <summary>
		/// このオブジェクトが持っている子オブジェクトのコレクションを取得する。
		/// </summary>
		public IEnumerable<Object2D> Children
		{
			get { return children_; }
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
		/// この2Dオブジェクトを描画する際の実際の位置を取得または設定する。親子関係がある場合に、親の位置を考慮した位置を取得できる。
		/// </summary>
		/// <returns>この2Dオブジェクトの位置。</returns>
		public Vector2DF GetGlobalPosition()
		{
			return CoreObject.GetGlobalPosition();
		}

		/// <summary>
		/// この2Dオブジェクトを破棄します。
		/// </summary>
		public void Vanish()
		{
			IsAlive = false;
			OnVanish();
		}

		/// <summary>
		/// 指定した2Dオブジェクトを子オブジェクトとしてこのインスタンスに追加する。
		/// </summary>
		/// <param name="child">追加する子オブジェクト</param>
		/// <param name="mode">子オブジェクトの同期モード</param>
		public void AddChild(Object2D child, ChildMode mode)
		{
			CoreObject.AddChild(child.CoreObject, (swig.ChildMode)mode);
			children_.Add(child);
		}

		/// <summary>
		/// 指定した子オブジェクトをこのインスタンスから削除する。
		/// </summary>
		/// <param name="child"></param>
		public void RemoveChild(Object2D child)
		{
			CoreObject.RemoveChild(child.CoreObject);
			children_.Remove(child);
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
		public void DrawTextAdditionally(Vector2DF pos, Color color, Font font, string text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int priority)
		{
			Layer.DrawTextAdditionally(pos, color, font, text, writingDirection, alphaBlend, priority);
		}

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトの初期化処理を記述できる。
		/// </summary>
		protected virtual void OnStart() { }

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトの更新処理を記述できる。
		/// </summary>
		protected virtual void OnUpdate() { }

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトに関する追加の描画処理を記述できる。
		/// </summary>
		protected virtual void OnDrawAdditionally() { }

		/// <summary>
		/// オーバーライドして、この2DオブジェクトがVanishメソッドによって破棄される際の処理を記述できる。
		/// </summary>
		protected virtual void OnVanish()
		{
		}

		/// <summary>
		/// オーバーライドして、この2Dオブジェクトが破棄される際の処理を記述できる。
		/// </summary>
		protected virtual void OnDispose()
		{
		}


		internal abstract swig.CoreObject2D CoreObject { get; }

		internal void Start()
		{
			OnStart();
		}

		internal override bool GetIsAlive()
		{
			return IsAlive;
		}

		internal override void Update()
		{
			if(IsUpdated && IsAlive)
			{
				OnUpdate();
				componentManager_.Update();
			}
		}

		internal void DrawAdditionally()
		{
			if(!IsDrawn || !IsAlive)
			{
				return;
			}
			OnDrawAdditionally();
		}

		internal void CallDestroy()
		{
			OnDispose();
		}

		private ComponentManager<Object2D, Object2DComponent> componentManager_ { get; set; }

		private List<Object2D> children_ { get; set; }

		public abstract bool IsDestroyed { get; }

		public abstract void Destroy();
	}
}

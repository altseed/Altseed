﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace asd
{
    public class ParentInfo2D
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
    public abstract class Object2D : AltseedObject, IReleasable, IDisposable, IBeingAbleToDisposeNative, IComponentRegisterable<Object2DComponent>
    {
        /// <summary>
        /// 現時点での衝突情報の一覧を取得する
        /// </summary>
        public List<Collision2DInfo> Collisions2DInfo { get; private set; }

        private HashSet<Collider2D> myColliders;



        /// <summary>
        /// コンストラクタ
        /// </summary>
        public Object2D()
        {
            componentManager_ = new ComponentManager<Object2DComponent>(this);
            ChildrenList = new List<Object2D>();
            IsUpdated = true;
            Collisions2DInfo = new List<Collision2DInfo>();
            myColliders = new HashSet<Collider2D>();
        }


        /// <summary>
        /// このオブジェクトの親オブジェクトを取得する。
        /// </summary>
        public Object2D Parent
        {
            get
            {
                if (ParentInfo != null)
                {
                    return ParentInfo.Parent;
                }
                return null;
            }
        }

        /// <summary>
        /// このオブジェクトが持っている子オブジェクトのコレクションを取得する。
        /// </summary>
        public IEnumerable<Object2D> Children
        {
            get { return ChildrenList; }
        }

        /// <summary>
        /// このオブジェクトが登録されているレイヤーを取得します。
        /// </summary>
        public Layer2D Layer { get; internal set; }

        public abstract bool IsReleased { get; }

        public abstract void ForceToRelease();

        #region パラメータ
        /// <summary>
        /// カメラに表示するグループを取得する。
        /// </summary>
        /// <remarks>
        /// カメラのグループと他のオブジェクトのグループで&amp;でビット演算して1以上だった場合、そのカメラにオブジェクトが描画されるようになる。
        /// </remarks>
        public int CameraGroup
        {
            get
            {
                ThrowIfReleased();
                return CoreObject.GetCameraGroup();
            }
            set
            {
                ThrowIfReleased();
                CoreObject.SetCameraGroup(value);
            }
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
            get
            {
                return CoreObject.GetIsDrawn();
            }
            set
            {
                CoreObject.SetIsDrawn(value);
            }
        }

        /// <summary>
        /// このオブジェクトが実行中かどうかを取得する。破棄されている場合に false を返す。
        /// </summary>
        public bool IsAlive
        {
            get { return CoreObject != null && CoreObject.GetIsAlive(); }
            private set { CoreObject.SetIsAlive(value); }
        }

        /// <summary>
        /// この2Dオブジェクトを描画する位置を取得または設定する。
        /// </summary>
        public Vector2DF Position
        {
            get
            {
                return CoreObject.GetPosition();
            }
            set
            {
                CoreObject.SetPosition(value);
            }
        }

        /// <summary>
        /// この2Dオブジェクトを描画する際の回転角度を取得または設定する。
        /// </summary>
        /// <remarks>
        /// 回転角度は度で表記され、回転方向は時計回りである。
        /// </remarks>
        public float Angle
        {
            get
            {
                return CoreObject.GetAngle();
            }
            set
            {
                CoreObject.SetAngle(value);
            }
        }

        /// <summary>
        /// この2Dオブジェクトを描画する際の拡大率を取得または設定する。
        /// </summary>
        public Vector2DF Scale
        {
            get
            {
                return CoreObject.GetScale();
            }
            set
            {
                CoreObject.SetScale(value);
            }
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
                    && (asd.Particular.ChildManagementMode.And(ParentInfo.ManagementMode, ChildManagementMode.IsUpdated) != 0)
                    && !ParentInfo.Parent.AbsoluteBeingUpdated);
            }
        }

        /// <summary>
        /// このオブジェクトが親子関係を考慮して最終的に描画されるかどうかの真偽値を取得します。
        /// </summary>
        public bool AbsoluteBeingDrawn
        {
            get
            {
                return CoreObject.GetAbsoluteBeingDrawn();
            }
        }

        /// <summary>
        /// この2Dオブジェクトを描画する際の実際の位置を取得または設定する。親子関係がある場合に、親の位置を考慮した位置を取得できる。
        /// </summary>
        /// <returns>この2Dオブジェクトの位置。</returns>
        public Vector2DF GetGlobalPosition()
        {
            return CoreObject.GetGlobalPosition();
        }
        #endregion

        #region Management
        /// <summary>
        /// この2Dオブジェクトを破棄する。
        /// </summary>
        public void Dispose()
        {
            Dispose(false);
        }

        /// <summary>
        /// この2Dオブジェクトを破棄する。
        /// </summary>
        /// <param name="disposeNative">ネイティブ リソースも即解放するかどうかの真偽値。</param>
        /// <remarks>破棄状況を同期している子オブジェクトもすべて破棄するが、子オブジェクトの破棄はこのメソッドを呼び出したフレームの最後に実行されるので注意が必要。</remarks>
        public override void Dispose(bool disposeNative)
        {
            Engine.ChangesToBeCommited.Enqueue(new EventToDisposeContent(this, disposeNative));
        }

        internal void SyncContainerWithChild(Object2D child)
        {
            if (child.IsInheriting(ChildManagementMode.RegistrationToLayer))
            {
                if (child.Layer != null && child.Layer != Layer)
                {
                    child.Layer.RemoveObject(child);
                }
                if (child.Layer == null && Layer != null)
                {
                    Layer.AddObject(child);
                }
            }
        }

        /// <summary>
        /// 指定した2Dオブジェクトを子オブジェクトとしてこのインスタンスに追加する。
        /// </summary>
        /// <param name="child">追加する子オブジェクト</param>
        /// <param name="managementMode">子オブジェクトの管理に関する同期設定。フラグをOR演算でつなげて複数指定することができる。</param>
        /// <param name="transformingMode">子オブジェクトの変形に関する同期設定。</param>
        /// <remarks>実際に追加されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
        public void AddChild(Object2D child, ChildManagementMode managementMode, ChildTransformingMode transformingMode)
        {
            var e = new EventToManageFamilyship2D(this, child);
            e.SetUpAsAddEvent(managementMode, transformingMode);
            Engine.ChangesToBeCommited.Enqueue(e);

            child.ParentInfo = new ParentInfo2D(this, managementMode);
            SyncContainerWithChild(child);
        }

        /// <summary>
        /// 指定した子オブジェクトをこのインスタンスから削除する。
        /// </summary>
        /// <param name="child">削除する子オブジェクト</param>
        /// <remarks>実際に削除されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
        public void RemoveChild(Object2D child)
        {
            var e = new EventToManageFamilyship2D(this, child);
            e.SetUpAsRemoveEvent();
            Engine.ChangesToBeCommited.Enqueue(e);

            if (child.IsInheriting(ChildManagementMode.RegistrationToLayer))
            {
                Layer.RemoveObject(child);
            }
            child.ParentInfo = null;
        }

        /// <summary>
        /// 指定したコンポーネントをこの2Dオブジェクトに追加する。
        /// </summary>
        /// <param name="component">追加するコンポーネント</param>
        /// <param name="key">コンポーネントに関連付けるキー</param>
        /// <remarks>実際に追加されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
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
        /// <remarks>実際に削除されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
        public bool RemoveComponent(string key)
        {
            return componentManager_.Remove(key);
        }


        internal void ImmediatelyAddChild(
            Object2D child,
            ChildManagementMode managementMode,
            ChildTransformingMode transformingMode)
        {
            CoreObject.AddChild(
                child.CoreObject,
                asd.Particular.ChildManagementMode.ToInt(managementMode),
                (swig.ChildTransformingMode)transformingMode);
            ChildrenList.Add(child);
        }

        internal void ImmediatelyRemoveChild(Object2D child)
        {
            if (IsAlive)
            {
                CoreObject.RemoveChild(child.CoreObject);
                ChildrenList.Remove(child);
            }
        }

        internal void ImmediatelyRemoveComponent(string key)
        {
            componentManager_.ImmediatelyRemoveComponent(key);
        }

        /// <summary>
        /// 内部用のメソッドで、ユーザーは呼び出してはいけない。
        /// </summary>
        /// <param name="disposeNative"></param>
        [EditorBrowsable(EditorBrowsableState.Never)]
        public void __DisposeImmediately(bool disposeNative)
        {
            if (IsAlive)
            {
                IsAlive = false;
                OnDispose();
                foreach (var component in componentManager_.Components)
                {
                    component.RaiseOnDisposed();
                }
                foreach (var item in asd.Particular.Lambda.FilterDeadObject(ChildrenList))
                {
                    CoreObject.RemoveChild(item.CoreObject);
                    if (item.IsInheriting(ChildManagementMode.Disposal))
                    {
                        item.Dispose();
                    }
                    item.ParentInfo = null;
                }
                if (Parent != null)
                {
                    Parent.ImmediatelyRemoveChild(this);
                }
                if (Layer != null)
                {
                    Layer.ImmediatelyRemoveObject(this, false);
                }
                if (disposeNative)
                {
                    ForceToRelease();
                }
            }
        }

        /// <summary>
        /// 内部用のメソッドで、ユーザーは呼び出してはいけない。
        /// </summary>
        /// <param name="component"></param>
        [EditorBrowsable(EditorBrowsableState.Never)]
        public void __Register(Object2DComponent component)
        {
            component.Owner = this;
        }

        /// <summary>
        /// 内部用のメソッドで、ユーザーは呼び出してはいけない。
        /// </summary>
        /// <param name="component"></param>
        [EditorBrowsable(EditorBrowsableState.Never)]
        public void __Unregister(Object2DComponent component)
        {
            component.Owner = null;
        }
        #endregion

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
        /// 通常の描画に加えて専用のシェーダーを用いて2Dスプライトを描画する。
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
        /// <remarks>
        /// 専用のシェーダーを用いてスプライトを描画する。
        /// OnDrawAdditionallyの中以外では実行してはいけない。
        /// </remarks>
        public void DrawSpriteWithMaterialAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
            Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
            Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
            Material2D material, AlphaBlendMode alphaBlend, int priority)
        {
            Layer.DrawSpriteWithMaterialAdditionally(upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos, upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol, upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV, material, alphaBlend, priority);
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
        internal override void RaiseOnAdded()
        {
            OnAdded();

            foreach (var component in componentManager_.Components)
            {
                component.RaiseOnAdded();
            }

            foreach (var child in Children)
            {
                SyncContainerWithChild(child);
            }

            foreach(var collider in myColliders)
            {
                if (!Layer.CollidersMap.ContainsKey(collider.CoreCollider.GetCreationId()))
                {
                    Layer.CollidersMap.Add(collider.CoreCollider.GetCreationId(), collider);
                }
            }
        }

        internal override void RaiseOnRemoved()
        {
            foreach (var child in Children)
            {
                SyncContainerWithChild(child);
            }

            foreach (var component in componentManager_.Components)
            {
                component.RaiseOnRemoved();
            }

            foreach (var collider in myColliders)
            {
                if (Layer.CollidersMap.ContainsKey(collider.CoreCollider.GetCreationId()))
                {
                    Layer.CollidersMap.Remove(collider.CoreCollider.GetCreationId());
                }
            }

            OnRemoved();
        }

        internal void RaiseOnCollisionCallbacks()
        {
            var collisionEventsNum = CoreObject.GetCollisionEventsNum();
            Collisions2DInfo.Clear();

            for (int index = 0; index < collisionEventsNum; index++) {
                var collisionEvent = CoreObject.GetCollisionEvent(index);
                var collision = collisionEvent.GetCollision();
                
                var colliderA = asd.Particular.Dictionary.Get(Layer.CollidersMap, collision.GetColliderA().GetCreationId());
                var colliderB = asd.Particular.Dictionary.Get(Layer.CollidersMap, collision.GetColliderB().GetCreationId());

                Collider2D myCollider = null;
                Collider2D theirCollider = null;

                if (myColliders.Contains(colliderA))
                {
                    myCollider = colliderA;
                    theirCollider = colliderB;
                }
                else
                {
                    myCollider = colliderB;
                    theirCollider = colliderA;
                }

                var collisionType = collisionEvent.GetCollisionType();

                var collisionInfo = new Collision2DInfo(myCollider, theirCollider, (CollisionType)collisionType);

                if(collisionType == swig.CollisionType.Enter)
                {
                    OnCollisionEnter(collisionInfo);
                }
                else if(collisionType == swig.CollisionType.Stay)
                {
                    OnCollisionStay(collisionInfo);
                }
                else if(collisionType == swig.CollisionType.Exit)
                {
                    OnCollisionExit(collisionInfo);
                }

                Collisions2DInfo.Add(collisionInfo);
            }

        }

        internal override void Update()
        {
            if (IsAlive && AbsoluteBeingUpdated)
            {
                OnUpdate();

                foreach (var component in componentManager_.Components)
                {
                    component.RaiseOnUpdate();
                }
                RaiseOnCollisionCallbacks();
            }
        }

        internal void DrawAdditionally()
        {
            if (IsAlive && AbsoluteBeingDrawn)
            {
                OnDrawAdditionally();
            }
        }

        /// <summary>
        /// オーバーライドして、この2Dオブジェクトがレイヤーに登録されたときの処理を記述できる。
        /// </summary>
        protected virtual void OnAdded() { }

        /// <summary>
        /// オーバーライドして、この2Dオブジェクトがレイヤーから登録解除されたときの処理を記述できる。
        /// </summary>
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


        /// <summary>
        /// オーバーライドして、この2Dオブジェクトが別の2Dオブジェクトのコライダと衝突開始する際の処理を記述できる。
        /// </summary>
        /// <param name="collisionInfo">衝突に関する情報</param>
        protected virtual void OnCollisionEnter(Collision2DInfo collisionInfo) { }

        /// <summary>
        /// オーバーライドして、この2Dオブジェクトが別の2Dオブジェクトのコライダと衝突が継続している際の処理を記述できる。
        /// </summary>
        /// <param name="collisionInfo">衝突に関する情報</param>
        protected virtual void OnCollisionStay(Collision2DInfo collisionInfo) { }

        /// <summary>
        /// オーバーライドして、この2Dオブジェクトが別の2Dオブジェクトのコライダと衝突終了した際の処理を記述できる。
        /// </summary>
        /// <param name="collisionInfo">衝突に関する情報</param>
        protected virtual void OnCollisionExit(Collision2DInfo collisionInfo) { }
        #endregion


        private bool IsInheriting(ChildManagementMode mode)
        {
            return ParentInfo != null && asd.Particular.ChildManagementMode.And(ParentInfo.ManagementMode, mode) != 0;
        }

        internal void ThrowIfDisposed()
        {
            if (!IsAlive)
            {
                throw new ObjectDisposedException(GetType().FullName);
            }
        }

        internal void ThrowIfReleased()
        {
            if (IsReleased)
            {
                throw new ObjectDisposedException(GetType().FullName);
            }
        }

        internal override bool GetIsAlive()
        {
            return IsAlive;
        }


        private ComponentManager<Object2DComponent> componentManager_ { get; set; }

        internal abstract swig.CoreObject2D CoreObject { get; }

        internal override bool IsRegisteredToLayer
        {
            get { return Layer != null; }
        }

        internal List<Object2D> ChildrenList { get; set; }

        internal ParentInfo2D ParentInfo { get; set; }

        /// <summary>
        /// コライダを追加する
        /// </summary>
        /// <param name="collider">追加するコライダ</param>
        public void AddCollider(Collider2D collider)
        {
            collider.OwnerObject = this;
            myColliders.Add(collider);
            if(Layer != null)
            {
                Layer.CollidersMap.Add(collider.CoreCollider.GetCreationId(), collider);
            }
            CoreObject.AddCollider(collider.CoreCollider);
        }

        /// <summary>
        /// コライダを削除する
        /// </summary>
        /// <param name="collider">削除するコライダ</param>
        public void RemoveCollider(Collider2D collider)
        {
            collider.OwnerObject = null;
            myColliders.Remove(collider);
            if(Layer != null)
            {
                Layer.CollidersMap.Remove(collider.CoreCollider.GetCreationId());
            }
            CoreObject.RemoveCollider(collider.CoreCollider);

        }
    }
}

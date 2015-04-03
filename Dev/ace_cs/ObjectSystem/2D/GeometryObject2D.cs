using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    /// <summary>
    /// テクスチャと描画パラメータを用いて、更新・描画処理を行う単位となる2Dオブジェクトの機能を提供するクラス
    /// </summary>
    public class GeometryObject2D : Object2D, IDestroy
    {
        private Shape shape;

        /// <summary>
        /// ace.GeometryObject2D の新しいインスタンスを初期化します。
        /// </summary>
        public GeometryObject2D()
        {
            shape = null;
            coreObject = Engine.ObjectSystemFactory.CreateGeometryObject2D();

            var p = coreObject.GetPtr();
            if (GC.Object2Ds.GetObject(p) != null)
            {
                throw new Exception();
            }
            GC.Object2Ds.AddObject(p, this);
        }

        #region GC対応
        ~GeometryObject2D()
        {
            Destroy();
        }

        public override bool IsDestroyed
        {
            get { return coreObject == null; }
        }

        public override void Destroy()
        {
            lock (this)
            {
                if (coreObject == null) return;
                GC.Collector.AddObject(coreObject);
                coreObject = null;
            }
            System.GC.SuppressFinalize(this);
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
                coreObject.SetShape(value.CoreShape);
                shape = value;
            }
        }


        /// <summary>
        /// この2Dオブジェクトの描画に使用するテクスチャを取得または設定します。
        /// </summary>
        public Texture2D Texture
        {
            get { return GC.GenerateTexture2D(coreObject.GetTexture(), GC.GenerationType.Get); }
            set { coreObject.SetTexture(IG.GetTexture2D(value)); }
        }

        /// <summary>
        /// この2Dオブジェクトを描画する際の描画原点を取得または設定します。描画原点は拡大・回転・描画の中心となる、画像データ内での座標です。
        /// </summary>
        public Vector2DF CenterPosition
        {
            get { return coreObject.GetCenterPosition(); }
            set { coreObject.SetCenterPosition(value); }
        }

        /// <summary>
        /// この2Dオブジェクトを描画する際に合成する色を取得または設定します。
        /// </summary>
        public Color Color
        {
            get { return coreObject.GetColor(); }
            set { coreObject.SetColor(value); }
        }

        /// <summary>
        /// この2Dオブジェクトを描画する際の描画優先度を取得または設定します。描画優先度が高いほど手前に描画されます。
        /// </summary>
        public int DrawingPriority
        {
            get { return coreObject.GetDrawingPriority(); }
            set { coreObject.SetDrawingPriority(value); }
        }

        /// <summary>
        /// この2Dオブジェクトを描画する際のブレンドモードを取得または設定します。
        /// </summary>
        public AlphaBlendMode AlphaBlend
        {
            get { return (AlphaBlendMode)coreObject.GetAlphaBlendMode(); }
            set { coreObject.SetAlphaBlendMode((swig.AlphaBlend)value); }
        }

        ///<summary>
        /// この2Dオブジェクトを描画する際のテクスチャフィルタを取得または設定する。
        /// </summary>
        public TextureFilterType TextureFilterType
        {
            get { return (TextureFilterType)coreObject.GetTextureFilterType(); }
            set { coreObject.SetTextureFilterType((swig.TextureFilterType)value); }
        }

        internal override swig.CoreObject2D CoreObject
        {
            get { return coreObject; }
        }

        private swig.CoreGeometryObject2D coreObject { get; set; }
    }
}

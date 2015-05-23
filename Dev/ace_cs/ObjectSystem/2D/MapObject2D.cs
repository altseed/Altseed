using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class MapObject2D : Object2D, IDestroy
    {

        internal override swig.CoreObject2D CoreObject
        {
            get { return coreMapObject; }
        }
        private swig.CoreMapObject2D coreMapObject { get; set; }

        public MapObject2D()
        {
            coreMapObject = Engine.ObjectSystemFactory.CreateMapObject2D();

            var p = coreMapObject.GetPtr();
            if (GC.Object2Ds.GetObject(p) != null)
            {
                throw new Exception();
            }

            GC.Object2Ds.AddObject(p, this);
        }

        #region GC対策
        ~MapObject2D()
        {
            Destroy();
        }

        public override bool IsDestroyed
        {
            get { return coreMapObject == null; }
        }

        public override void Destroy()
        {
            lock (this)
            {
                if (coreMapObject == null) return;
                GC.Collector.AddObject(coreMapObject);
                coreMapObject = null;
            }
            Particular.GC.SuppressFinalize(this);
        }
        #endregion


        /// <summary>
        /// この2Dオブジェクトを描画する際の描画原点を取得または設定する。描画原点は拡大・回転・描画の中心となる、画像データ内での座標。
        /// </summary>
        public Vector2DF CenterPosition
        {
            get { return coreMapObject.GetCenterPosition(); }
            set { coreMapObject.SetCenterPosition(value); }
        }

        /// <summary>
        /// この2Dオブジェクトを描画する際の描画優先度を取得または設定する。描画優先度が高いほど手前に描画される。
        /// </summary>
        public int DrawingPriority
        {
            get { return coreMapObject.GetDrawingPriority(); }
            set { coreMapObject.SetDrawingPriority(value); }
        }

        /// <summary>
        /// この2Dオブジェクトにチップを追加する。
        /// </summary>
        public bool AddChip(Chip2D chip)
        {
            return coreMapObject.AddChip(IG.GetChip2D(chip));
        }

        /// <summary>
        /// この2Dオブジェクトからチップを削除する。
        /// </summary>
        public bool RemoveChip(Chip2D chip)
        {
            return coreMapObject.RemoveChip(IG.GetChip2D(chip));
        }

        /// <summary>
        /// この2Dオブジェクトに追加されている全てのチップを削除する。
        /// </summary>
        public void Clear()
        {
            coreMapObject.Clear();
        }
    }
}

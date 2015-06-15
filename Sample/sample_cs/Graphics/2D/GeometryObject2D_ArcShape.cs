﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Graphics._2D
{
    /// <summary>
    /// 図形を表示する。
    /// </summary>
    class GeometryObject2D_ArcShape : ISample
    {
        public void Run()
        {
            // aceを初期化する
            asd.Engine.Initialize("GeometryObject2D_ArcShape", 640, 480, new asd.EngineOption());

            var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

            //図形描画クラスのコンストラクタを呼び出す
            var geometryObj = new asd.GeometryObject2D();

            //図形描画クラスをレイヤーに追加する。
            asd.Engine.AddObject2D(geometryObj);

            // aceが進行可能かチェックする。
            while (asd.Engine.DoEvents())
            {
                // aceを更新する。
                asd.Engine.Update();
            }

            // aceを終了する。
            asd.Engine.Terminate();
        }
    }
}
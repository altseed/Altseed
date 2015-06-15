﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Graphics._2D
{
    /// <summary>
    /// マップを表示する。
    /// </summary>
    class MapObject2D_Default : ISample
    {
        public void Run()
        {
            // aceを初期化する
            asd.Engine.Initialize("MapObject2D_Default", 640, 480, new asd.EngineOption());

            {
                //マップオブジェクトを生成する。
                var mapObject = new asd.MapObject2D();

                var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample2.png");

                //マップオブジェクトに50*50=2500個のチップを登録する。
                for (int i = 0; i < 50; ++i)
                {
                    for (int j = 0; j < 50; ++j)
                    {
                        //チップを生成する。
                        var chip = new asd.Chip2D();

                        //チップにテクスチャを設定する。
                        chip.Texture = texture;

                        //チップの描画先を指定する。
                        chip.Position = new asd.Vector2DF(i * 40 - 1000, j * 40 - 1000);

                        //マップオブジェクトにチップを追加する。
                        mapObject.AddChip(chip);
                    }
                }

                //レイヤーにマップオブジェクトを追加する。
                asd.Engine.AddObject2D(mapObject);
            }

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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// マップを表示するサンプル。
/// </summary>
class MapObject2D_Basic : ISample
{
	public string Description
	{
		get { return "マップチップを並べて描画するサンプルです。"; }
	}

	public string Title
	{
		get { return "マップチップの描画"; }
	}

	public string ClassName
	{
		get { return "MapObject2D_Basic"; }
	}

	public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("MapObject2D_Basic", 640, 480, new asd.EngineOption());

        {
            //マップオブジェクトを生成する。
            var mapObject = new asd.MapObject2D();

            var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Chip1.png");

            //マップオブジェクトに16*12=184個のチップを登録する。
            for (int i = 0; i < 16; ++i)
            {
                for (int j = 0; j < 12; ++j)
                {
                    //チップを生成する。
                    var chip = new asd.Chip2D();

                    //チップにテクスチャを設定する。
                    chip.Texture = texture;

                    //チップの描画先を指定する。
                    chip.Position = new asd.Vector2DF(i * 40, j * 40);

                    //マップオブジェクトにチップを追加する。
                    mapObject.AddChip(chip);
                }
            }

            //レイヤーにマップオブジェクトを追加する。
            asd.Engine.AddObject2D(mapObject);
        }

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // Altseedを更新する。
            asd.Engine.Update();
            Recorder.TakeScreenShot("MapObject2D_Basic", 30);
        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
